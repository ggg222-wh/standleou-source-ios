
#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#import <mach-o/dyld.h>
#import <mach-o/getsect.h>
#import <mach/mach.h>
#import <mach/mach_traps.h>
#import <sys/sysctl.h>
#import <sys/utsname.h>
#import <string>
#import <vector>
#import <stdexcept>
#import <fstream>
#import <filesystem>
#import <thread>

#import "texture_manager/texture_manager.hpp"
#import "texture_manager/device_decl.h"

#import "imgui.h"
#import "imgui_internal.h"

#import "antisocial.h"
#import "util/backend/imgui_impl_metal.h"

#import "memory_manager/memory_manager.hpp"

#import "globals.hpp"
#import "interface/interface.hpp"

#import "game/sdk/il2cpp/il2cpp.hpp"
#import "game/entities/entities.hpp"

#import "game/hooks/hooks.hpp"

#import "game/features/ragebot/ragebot.hpp"
#import "game/features/inventory/inventory.hpp"

#import "badge/AppBadgeView.h"
#import "badge/DeviceMetrics.h"

#import "networking/networking.hpp"

#import "SoundManager.h"
#import "audio_player/audio_player.hpp"

#import "WebSocketManager.h"
#import "chat/chat.hpp"
#import "ws_pranks/ws_pranks.hpp"

#import "interface/widgets/widgets.hpp"

#import "alert/alert.hpp"

#import "json/json.hpp"

#import "image_selector/image_selector.hpp"
#import "ImageSelector.h"

BOOL am_i_being_debugged( void )
{
    struct kinfo_proc info;
    size_t size = sizeof( info );
    int mib[ 4 ] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid( ) };

    if ( sysctl( mib, 4, &info, &size, NULL, 0 ) != 0 )
    {
        return NO;
    }

    return ( info.kp_proc.p_flag & P_TRACED ) != 0;
}

static const std::string base64_chars = crypt_string( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" );

inline bool is_base64( unsigned char c )
{
    return ( isalnum( c ) || ( c == '+' ) || ( c == '/' ) );
}

std::vector< unsigned char > base64_decode( const std::string& encoded_string )
{
    int in_len = ( int ) encoded_string.size( );
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[ 4 ], char_array_3[ 3 ];
    std::vector< unsigned char > ret;

    while ( in_len-- && ( encoded_string[ in_ ] != '=' ) && is_base64( encoded_string[ in_ ] ) )
    {
        char_array_4[ i++ ] = encoded_string[ in_ ];
        in_++;
        if ( i == 4 )
        {
            for ( i = 0; i < 4; i++ )
                char_array_4[ i ] = ( unsigned char ) base64_chars.find( char_array_4[ i ] );

            char_array_3[ 0 ] = ( char_array_4[ 0 ] << 2 ) + ( ( char_array_4[ 1 ] & 0x30 ) >> 4 );
            char_array_3[ 1 ] = ( ( char_array_4[ 1 ] & 0xf ) << 4 ) + ( ( char_array_4[ 2 ] & 0x3c ) >> 2 );
            char_array_3[ 2 ] = ( ( char_array_4[ 2 ] & 0x3 ) << 6 ) + char_array_4[ 3 ];

            for ( i = 0; i < 3; i++ )
                ret.push_back( char_array_3[ i ] );
            i = 0;
        }
    }

    if ( i )
    {
        for ( j = i; j < 4; j++ )
            char_array_4[ j ] = 0;

        for ( j = 0; j < 4; j++ )
            char_array_4[ j ] = ( unsigned char ) base64_chars.find( char_array_4[ j ] );

        char_array_3[ 0 ] = ( char_array_4[ 0 ] << 2 ) + ( ( char_array_4[ 1 ] & 0x30 ) >> 4 );
        char_array_3[ 1 ] = ( ( char_array_4[ 1 ] & 0xf ) << 4 ) + ( ( char_array_4[ 2 ] & 0x3c ) >> 2 );
        char_array_3[ 2 ] = ( ( char_array_4[ 2 ] & 0x3 ) << 6 ) + char_array_4[ 3 ];

        for ( j = 0; j < i - 1; j++ )
            ret.push_back( char_array_3[ j ] );
    }

    return ret;
}

#pragma region cpp_defs
globals_t* g_ctx = new globals_t( );

WebSocketManager* wsManager;

void debug_log( const std::string& str )
{
    NSLog( [[NSString alloc] initWithUTF8String:str.c_str( )] );
}

void c_chat::ws_send( const std::string& text ) const
{
    if ( !wsManager || text.empty( ) )
        return;

    NSString* nsText = [[NSString alloc] initWithUTF8String:text.c_str( )];
    if ( !nsText )
    {
        return;
    }

    dispatch_async( dispatch_get_main_queue( ), ^{
      [wsManager sendMessage:nsText];
    } );
}

float get_framebuffer_scale( )
{
    // utsname systemInfo;
    // uname( &systemInfo );
    if ( !g_ctx->backup_model )
        return 0.f;

    const char* model = g_ctx->backup_model; // systemInfo.machine;

    if ( !strcmp( model, "iPhone9,2" ) || !strcmp( model, "iPhone9,4" ) ||
         !strcmp( model, "iPhone10,2" ) || !strcmp( model, "iPhone10,5" ) )
    {
        return 2.61f;
    } else if ( !strcmp( model, "iPhone14,4" ) || !strcmp( model, "iPhone13,1" ) )
    {
        return 2.88f;
    } else
    {
        return ( float ) UIScreen.mainScreen.scale;
    }
}

void c_audio_player::play( int idx )
{
    if ( @available( iOS 15, * ) )
    {
        [[SoundManager sharedManager] playSoundAtIndex:idx];
    }
}

std::string network::c_requests::get_hardware( ) const
{
    return g_ctx->backup_hardware;
    // NSUUID* identifierForVendor = [[UIDevice currentDevice] identifierForVendor];
    // return [[identifierForVendor UUIDString] UTF8String];
}

@interface antisocial ( )
@property ( nonatomic, strong ) id< MTLDevice > device;
@property ( nonatomic, strong ) id< MTLCommandQueue > commandQueue;

@property ( nonatomic, strong ) DeviceMetrics* metrics;
@end

@implementation antisocial

ImageSelector* selector;

void image::select( const std::string& file )
{
    if ( !selector )
        return;

    [selector selectImageWithFilename:[NSString stringWithUTF8String:file.c_str( )]];
}

void create_directory( const std::string& what )
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
    NSString* documentsDirectory = [paths firstObject];
    NSString* path = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithUTF8String:what.c_str( )]];

    NSFileManager* fileManager = [NSFileManager defaultManager];
    if ( ![fileManager fileExistsAtPath:path] )
    {
        NSError* error = nil;
        [fileManager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:&error];
    }
}

- ( instancetype )initWithNibName:( nullable NSString* )nibNameOrNil bundle:( nullable NSBundle* )nibBundleOrNil
{
    // NSLog( @"$$$gang: initwithnibname" );
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];

    _device = MTLCreateSystemDefaultDevice( );
    _commandQueue = [_device newCommandQueue];

    if ( !self.device )
        abort( );

    // NSLog( @"$$$gang: imgui init $.." );
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );

    ImGui::StyleColorsDark( );

    NSArray* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
    NSString* documentsDirectory = [paths objectAtIndex:0];
    NSString* filePath = [documentsDirectory stringByAppendingPathComponent:ns_encrypt( "antisocial.png" )];

    g_ctx->documents_directory = [documentsDirectory UTF8String];

    create_directory( crypt_string( "sounds" ) );
    create_directory( crypt_string( "previews" ) );
    create_directory( crypt_string( "previews/girls" ) );
    create_directory( crypt_string( "previews/girls/ayanami_rei" ) );
    create_directory( crypt_string( "previews/girls/stella" ) );

    bool const is_network_connected = network::requests( )->check_network( crypt_string( "80" ) );
    if ( !is_network_connected )
    {
        alert::show_error( crypt_string( "No connection!" ), crypt_string( "Could not establish connection!" ) );
    }

    g_ctx->interface->init( get_framebuffer_scale( ) );

    ImGui_ImplMetal_Init( _device );

    g_ctx->features.init( );

    g_ctx->interface->m_queue.push_back( [] {
        c_players_database::get( )->update( );
        c_dropped_items_database::get( )->update_weapons( );
        c_dropped_items_database::get( )->update_grenades( );
    } );

    return self;
}

+ ( void )set_presented:( BOOL )value
{
    g_ctx->audio_player->play( static_cast< int >( sound_t::button ) );
    g_ctx->is_presented = value;
}

- ( MTKView* )mtkView
{
    return ( MTKView* ) self.view;
}

- ( void )loadView
{
    CGFloat w = [UIApplication sharedApplication].windows[ 0 ].rootViewController.view.frame.size.width;
    CGFloat h = [UIApplication sharedApplication].windows[ 0 ].rootViewController.view.frame.size.height;
    self.view = [[MTKView alloc] initWithFrame:CGRectMake( 0, 0, w, h )];
}

- ( void )viewDidLoad
{
    [super viewDidLoad];

    self.mtkView.device = self.device;
    self.mtkView.delegate = self;
    self.mtkView.clearColor = MTLClearColorMake( 0, 0, 0, 0 );
    self.mtkView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0];
    self.mtkView.clipsToBounds = YES;
    self.mtkView.preferredFramesPerSecond = 120;

    printf( R"(
    ----------------------------------------
    я потерял себя
    от безысходности я хотел убить себя
    еще один день я схожу с ума
    ----------------------------------------
    )" );

    network::requests( )->download_file( crypt_string( "http://62.60.249.46:80/api/assets/antisocial.png" ), g_ctx->documents_directory + crypt_string( "/antisocial.png" ) );

    auto const download_file = [ & ]( const std::string& filename ) {
        std::string const path = g_ctx->documents_directory + "/" + filename;
        if ( !std::filesystem::exists( path ) )
            network::requests( )->download_file( crypt_string( "http://62.60.249.46:80/api/assets/" ) + filename, path );
    };

    self_device = self.device;

    selector = [[ImageSelector alloc] initWithViewController:self];

    self.metrics = [[DeviceMetrics alloc] init];

    [AppBadgeView updateBadgeWithMetrics:self.metrics];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector( orientationChanged: )
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];

    dispatch_after( dispatch_time( DISPATCH_TIME_NOW, 3 * NSEC_PER_SEC ), dispatch_get_main_queue( ), ^{
      namespace fs = std::filesystem;
      static const fs::path key_file = fs::path( g_ctx->documents_directory + crypt_string( "/key" ) );

      auto const get_saved_key = [ & ]( ) -> std::optional< std::string > {
          if ( !fs::exists( key_file ) )
              return std::nullopt;

          std::ifstream ifs( key_file, std::ios::in );
          if ( !ifs.is_open( ) )
              return std::nullopt;

          std::string key;
          std::getline( ifs, key );
          ifs.close( );
          return key.empty( ) ? std::nullopt : std::optional< std::string > { key };
      };

      auto const save_key = [ & ]( const std::string& key ) {
          std::ofstream ofs( key_file, std::ios::out | std::ios::trunc );
          if ( ofs.is_open( ) )
          {
              ofs << key;
              ofs.close( );
          }
      };

      auto authenticate_with_key = [ & ]( const std::string& key ) {
          auto const server_response = network::requests( )->auth( key, network::requests( )->get_hardware( ) );

          if ( server_response.find( crypt_string( "success" ) ) != std::string::npos )
          {
              try
              {
                  nlohmann::json response = nlohmann::json::parse( server_response );

                  g_ctx->user_data.username =
                      response[ crypt_string( "data" ) ][ crypt_string( "user_username" ) ].get< std::string >( );
                  g_ctx->user_data.premium =
                      response[ crypt_string( "data" ) ][ crypt_string( "subscription_type" ) ].get< std::string >( ) ==
                      crypt_string( "premium" );

                  std::string hitsound { };

                  if ( response[ crypt_string( "data" ) ].contains( crypt_string( "hitsound_base64" ) ) && !response[ crypt_string( "data" ) ][ crypt_string( "hitsound_base64" ) ].is_null( ) )
                      hitsound = response[ crypt_string( "data" ) ][ crypt_string( "hitsound_base64" ) ].get< std::string >( );
                  else
                      hitsound = crypt_string( "none" );

                  if ( hitsound != crypt_string( "none" ) )
                  {
                      auto base64_decode = []( const std::string& input ) -> std::vector< uint8_t > {
                          static constexpr unsigned char decode_table[ 256 ] = {
                              64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                              64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                              64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
                              52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 0, 64, 64,
                              64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                              15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
                              64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                              41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
                              //
                          };

                          std::vector< uint8_t > output;
                          output.reserve( input.size( ) * 3 / 4 );

                          uint32_t buffer = 0;
                          int bits_collected = -8;

                          for ( unsigned char c : input )
                          {
                              if ( decode_table[ c ] == 64 )
                                  continue;
                              buffer = ( buffer << 6 ) + decode_table[ c ];
                              bits_collected += 6;
                              if ( bits_collected >= 0 )
                              {
                                  output.push_back( static_cast< char >( ( buffer >> bits_collected ) & 0xFF ) );
                                  bits_collected -= 8;
                              }
                          }
                          return output;
                      };

                      auto save_hitsound = [ & ]( const std::string& hitsound_base64, const std::string& documents_dir ) -> void {
                          auto const decoded = base64_decode( hitsound_base64 );

                          std::string const path = documents_dir + crypt_string( "/sounds/hitsound.wav" );
                          std::ofstream file( path, std::ios::binary );
                          if ( !file.is_open( ) )
                              return;

                          file.write( reinterpret_cast< const char* >( decoded.data( ) ), decoded.size( ) );
                      };

                      save_hitsound( hitsound, g_ctx->documents_directory );
                  } else
                      download_file( crypt_string( "sounds/hitsound.wav" ) );

                  g_ctx->user_data.telegram_id = std::to_string( response[ crypt_string( "data" ) ][ crypt_string( "user_id" ) ].get< uint64_t >( ) );

                  std::string const b64 =
                      response[ crypt_string( "data" ) ][ crypt_string( "avatar_base64" ) ].get< std::string >( );

                  if ( !b64.c_str( ) || b64.size( ) == 0 )
                  {
                      return;
                  }

                  std::vector< unsigned char > decoded_data = base64_decode( std::string( b64.c_str( ), b64.size( ) ) );
                  if ( decoded_data.empty( ) )
                  {
                      return;
                  }

                  g_ctx->user_data.profile_picture_raw = decoded_data;
                  // g_ctx->texture_manager->load( decoded_data.data( ), decoded_data.size( ), &g_ctx->user_data.profile_picture, crypt_string( "internal_profile_picture" ) );

                  save_key( key );

                  wsManager = [[WebSocketManager alloc] initWithURL:[NSURL URLWithString:ns_encrypt( "ws://62.60.249.46:80/ws" )]];
                  [wsManager connect];

                  std::thread( [ & ]( ) -> void {
                      g_ctx->is_loading.store( true );
                      g_ctx->passed.store( true );

                      download_file( crypt_string( "previews/girls/ayanami_rei/body.png" ) );
                      download_file( crypt_string( "previews/girls/ayanami_rei/head.png" ) );
                      download_file( crypt_string( "previews/girls/ayanami_rei/arms.png" ) );
                      download_file( crypt_string( "previews/girls/ayanami_rei/legs.png" ) );

                      download_file( crypt_string( "previews/girls/stella/body.png" ) );
                      download_file( crypt_string( "previews/girls/stella/head.png" ) );
                      download_file( crypt_string( "previews/girls/stella/arms.png" ) );
                      download_file( crypt_string( "previews/girls/stella/legs.png" ) );

                      download_file( crypt_string( "previews/girls/background.png" ) );

                      download_file( crypt_string( "jigsaw.jpeg" ) );
                      download_file( crypt_string( "kaban.jpg" ) );
                      download_file( crypt_string( "plankton.png" ) );
                      download_file( crypt_string( "troll.jpg" ) );

                      download_file( crypt_string( "previews/dropped_weapons.png" ) );
                      download_file( crypt_string( "previews/bomb.png" ) );
                      download_file( crypt_string( "previews/thrown_grenades.png" ) );

                      download_file( crypt_string( "sounds/button.caf" ) );
                      download_file( crypt_string( "sounds/hitlog.caf" ) );
                      download_file( crypt_string( "sounds/manual_aa_change.caf" ) );
                      download_file( crypt_string( "sounds/peek_end.caf" ) );
                      download_file( crypt_string( "sounds/peek_start.caf" ) );
                      download_file( crypt_string( "sounds/toggle.caf" ) );

                      download_file( crypt_string( "sounds/horn.wav" ) );

                      if ( @available( iOS 15, * ) )
                      {
                          [[SoundManager sharedManager] preloadSounds];
                      }

                      g_ctx->is_loading.store( false );
                      g_ctx->is_initialized.store( true );
                  } ).detach( );

                  network::requests( )->m_user_key = key;
              } catch ( const nlohmann::json::parse_error& )
              {
                  alert::show_error( crypt_string( "Error!" ), crypt_string( "Unknown error occurred!" ) );
              }
          } else
          {
              if ( server_response.find( crypt_string( "detail" ) ) != std::string::npos )
              {
                  try
                  {
                      nlohmann::json response = nlohmann::json::parse( server_response );
                      std::string const parsed = response[ crypt_string( "detail" ) ].get< std::string >( );

                      alert::show_error( crypt_string( "Error!" ), parsed );

                      if ( fs::exists( key_file ) )
                      {
                          fs::remove( key_file );
                      }
                  } catch ( const nlohmann::json::parse_error& )
                  {
                      alert::show_error( crypt_string( "Error!" ), crypt_string( "Unknown error occurred!" ) );
                  }
              } else
              {
                  alert::show_error( crypt_string( "Error!" ), crypt_string( "Unknown error occurred!" ) );
              }
          }
      };

      if ( auto saved_key = get_saved_key( ) )
          authenticate_with_key( *saved_key );
      else
      {
          alert::create_with_task(
              crypt_string( "@antisocial_pub" ),
              crypt_string( "OK" ),
              crypt_string( "Key" ),
              [ & ]( const std::string& key ) {
                  authenticate_with_key( key );
              } );
      }
    } );
}

- ( void )orientationChanged:( NSNotification* )notification
{
    if ( @available( iOS 15, * ) )
    {
        [AppBadgeView updateBadgeWithMetrics:self.metrics];
    }
}

#pragma mark - Interaction

- ( void )updateIOWithTouchEvent:( UIEvent* )event
{
    ImGuiIO& io = ImGui::GetIO( );

    UITouch* anyTouch = event.allTouches.anyObject;
    if ( anyTouch )
    {
        CGPoint pos = [anyTouch locationInView:self.view];
        io.MousePos = ImVec2( pos.x, pos.y );
    } else
    {
        io.MousePos = ImVec2( -FLT_MAX, -FLT_MAX );
    }

    BOOL hasActiveTouch = NO;
    for ( UITouch* touch in event.allTouches )
    {
        if ( touch.phase != UITouchPhaseEnded && touch.phase != UITouchPhaseCancelled )
        {
            hasActiveTouch = YES;
            break;
        }
    }

    io.MouseDown[ 0 ] = hasActiveTouch;
}

- ( void )touchesBegan:( NSSet< UITouch* >* )touches withEvent:( UIEvent* )event
{
    [self updateIOWithTouchEvent:event];
}

- ( void )touchesMoved:( NSSet< UITouch* >* )touches withEvent:( UIEvent* )event
{
    [self updateIOWithTouchEvent:event];
}

- ( void )touchesCancelled:( NSSet< UITouch* >* )touches withEvent:( UIEvent* )event
{
    [self updateIOWithTouchEvent:event];
}

- ( void )touchesEnded:( NSSet< UITouch* >* )touches withEvent:( UIEvent* )event
{
    [self updateIOWithTouchEvent:event];
}

#pragma mark - MTKViewDelegate

- ( void )drawInMTKView:( MTKView* )view
{
    ImGuiIO& io = ImGui::GetIO( );

    io.DisplaySize = [ & ]( ) -> ImVec2 {
        CGSize size = UIScreen.mainScreen.bounds.size;
        return { ( float ) size.width, ( float ) size.height };
    }( );

    auto const framebufferScale = get_framebuffer_scale( );

    io.DisplayFramebufferScale = ImVec2( framebufferScale, framebufferScale );

    io.IniFilename = nullptr;

    static auto last_time = std::chrono::steady_clock::now( );
    auto now = std::chrono::steady_clock::now( );
    io.DeltaTime = std::chrono::duration< float >( now - last_time ).count( );
    last_time = now;

    id< MTLCommandBuffer > command_buffer = [self.commandQueue commandBuffer];

    [self.view setUserInteractionEnabled:g_ctx->is_presented];

    static dispatch_once_t init_token;
    dispatch_once( &init_token, ^{
      memory_manager::base = memory_manager::get_base( );
      if ( !memory_manager::base )
          init_token = 0;

      g_ctx->features.ragebot->m_doubletap_time_point = std::chrono::steady_clock::now( );

      g_ctx->il2cpp->initialize( );

      g_ctx->hooks->hook( );
    } );

    // any ideas?

    MTLRenderPassDescriptor* render_pass_descriptor = view.currentRenderPassDescriptor;
    if ( command_buffer != nil )
    {
        id< MTLRenderCommandEncoder > render_encoder = [command_buffer renderCommandEncoderWithDescriptor:render_pass_descriptor];
        [render_encoder pushDebugGroup:@"ImGui Jane"];

#if DEBUG
#else
        if ( am_i_being_debugged( ) )
        {
            UIScreen* screen = [UIScreen mainScreen];

            while ( 1 )
            {
                UIView* snapshot = [screen snapshotViewAfterScreenUpdates:YES];

                ( void ) snapshot;
            }
        }
#endif

        ImGui_ImplMetal_NewFrame( render_pass_descriptor );

        ImGui::NewFrame( );

        ImGui::GetStyle( ).WindowShadowSize = 0.f;

        // static dispatch_once_t widgets_token;
        // dispatch_once( &widgets_token, ^{
        //   g_ctx->widgets->update( );
        // } );

        if ( g_ctx->passed.load( ) && g_ctx->is_initialized.load( ) )
        {
            g_ctx->texture_manager->init_internal( );
            g_ctx->ws_pranks->init( );
            g_ctx->widgets->render( g_ctx->is_presented );
            g_ctx->interface->render( );
            g_ctx->ws_pranks->render( );
        }

        if ( g_ctx->is_loading.load( ) || !g_ctx->passed.load( ) )
            g_ctx->interface->show_loading_screen( );

        ImGui::Render( );

        ImDrawData* draw_data = ImGui::GetDrawData( );
        ImGui_ImplMetal_RenderDrawData( draw_data, command_buffer, render_encoder );

        [render_encoder popDebugGroup];
        [render_encoder endEncoding];

        [command_buffer presentDrawable:view.currentDrawable];
    }

    [command_buffer commit];
}

- ( void )mtkView:( MTKView* )view drawableSizeWillChange:( CGSize )size
{
    if ( @available( iOS 15, * ) )
    {
        [AppBadgeView updateBadgeWithMetrics:self.metrics];
    }
}

@end
