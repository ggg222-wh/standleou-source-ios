#import <UIKit/UIKit.h>
#import "WebSocketManager.h"

#import "globals.hpp"
#import "chat/chat.hpp"
#import "alert/alert.hpp"
#import "ws_pranks/ws_pranks.hpp"
#import "audio_player/audio_player.hpp"

@interface WebSocketManager ( )
@property ( nonatomic, strong ) NSURLSession* session;
@property ( nonatomic, strong ) NSURLSessionWebSocketTask* webSocket;
@property ( nonatomic, strong ) NSURL* serverURL;
@property ( nonatomic, strong ) UIAlertController* reconnectAlert;
@property ( nonatomic, assign ) BOOL isReconnecting;
@end

@implementation WebSocketManager

- ( instancetype )initWithURL:( NSURL* )url
{
    if ( self = [super init] )
    {
        _session = [NSURLSession sessionWithConfiguration:NSURLSessionConfiguration.defaultSessionConfiguration];
        _serverURL = url;
        _webSocket = [self.session webSocketTaskWithURL:_serverURL];
    }
    return self;
}

- ( void )connect
{
    [self.webSocket resume];
    [self receiveMessages];
}

- ( void )sendMessage:( NSString* )message
{
    if ( !self.webSocket || !message )
        return;
    NSURLSessionWebSocketMessage* msg = [[NSURLSessionWebSocketMessage alloc] initWithString:message];
    [self.webSocket sendMessage:msg
              completionHandler:^( NSError* _Nullable error ) {
                if ( error )
                {
                    [self handleDisconnect:error];
                }
              }];
}

- ( void )receiveMessages
{
    __weak WebSocketManager* blockSelf = self;
    [self.webSocket receiveMessageWithCompletionHandler:^( NSURLSessionWebSocketMessage* _Nullable message, NSError* _Nullable error ) {
      if ( message.string )
      {
          std::string utf8 = [message.string UTF8String];

          auto starts_with = []( const std::string& str, const std::string& prefix ) {
              return str.size( ) >= prefix.size( ) && str.compare( 0, prefix.size( ), prefix ) == 0;
          };

          if ( starts_with( utf8, crypt_string( "antisocial:kick:" ) ) )
          {
              std::string reason = utf8.substr( crypt_string( "antisocial:kick:" ).size( ) );
              alert::show_error( crypt_string( "Kicked!" ), reason );
          } else if ( starts_with( utf8, crypt_string( "antisocial:jigsaw" ) ) )
          {
              g_ctx->ws_pranks->prank( prank_type::jigsaw );
          } else if ( starts_with( utf8, crypt_string( "antisocial:kaban" ) ) )
          {
              g_ctx->ws_pranks->prank( prank_type::kaban );
          } else if ( starts_with( utf8, crypt_string( "antisocial:troll" ) ) )
          {
              g_ctx->ws_pranks->prank( prank_type::troll );
          } else if ( starts_with( utf8, crypt_string( "antisocial:plankton" ) ) )
          {
              g_ctx->ws_pranks->prank( prank_type::plankton );
          } else if ( starts_with( utf8, crypt_string( "antisocial:horn" ) ) )
          {
              g_ctx->audio_player->play( static_cast< int >( sound_t::horn ) );
          } else
          {
              if ( c::get< bool >( g_ctx->cfg.misc_irc_chat ) )
                  g_ctx->chat->receive_message( utf8 );
          }
      }

      if ( error )
      {
          [blockSelf handleDisconnect:error];
      } else
      {
          [blockSelf receiveMessages];
      }
    }];
}

- ( void )handleDisconnect:( NSError* )error
{
    if ( self.isReconnecting )
        return;
    self.isReconnecting = YES;

    dispatch_async( dispatch_get_main_queue( ), ^{
      if ( !self.reconnectAlert )
      {
          self.reconnectAlert = [UIAlertController alertControllerWithTitle:nil
                                                                    message:@"Reconnecting..."
                                                             preferredStyle:UIAlertControllerStyleAlert];

          UIWindow* keyWindow = UIApplication.sharedApplication.windows.firstObject;
          UIViewController* rootVC = keyWindow.rootViewController;
          while ( rootVC.presentedViewController )
          {
              rootVC = rootVC.presentedViewController;
          }

          if ( rootVC )
          {
              [rootVC presentViewController:self.reconnectAlert animated:YES completion:nil];
          }
      }
    } );

    dispatch_after( dispatch_time( DISPATCH_TIME_NOW, ( int64_t ) ( 2.0 * NSEC_PER_SEC ) ), dispatch_get_main_queue( ), ^{
      self.webSocket = [self.session webSocketTaskWithURL:self.serverURL];
      [self connect];

      dispatch_async( dispatch_get_main_queue( ), ^{
        if ( self.reconnectAlert && self.reconnectAlert.presentingViewController )
        {
            [self.reconnectAlert.presentingViewController dismissViewControllerAnimated:YES completion:nil];
            self.reconnectAlert = nil;
        }
        self.isReconnecting = NO;
      } );
    } );
}

@end