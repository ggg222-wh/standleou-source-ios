#import "SoundManager.h"

@interface SoundManager ( )

@property ( nonatomic, strong ) NSMutableDictionary< NSString*, AVAudioPlayer* >* audioPlayers;
@property ( nonatomic, strong ) NSArray< NSString* >* soundFileNames;
@property ( nonatomic, readwrite ) BOOL soundsReady;

@end

@implementation SoundManager

+ ( instancetype )sharedManager
{
    static SoundManager* instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once( &onceToken, ^{
      instance = [[SoundManager alloc] init];
      [instance setupAudioSession];
    } );
    return instance;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        _audioPlayers = [NSMutableDictionary new];
        _soundFileNames = @[
            @"button.caf",
            @"hitlog.caf",
            @"hitsound.wav",
            @"manual_aa_change.caf",
            @"peek_end.caf",
            @"peek_start.caf",
            @"toggle.caf",
            @"horn.wav"
        ];
        _soundsReady = NO;
    }
    return self;
}

- ( void )setupAudioSession
{
    NSError* sessionError = nil;
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback
                                           error:&sessionError];
    if ( sessionError )
    {
        NSLog( @"Audio session category error: %@", sessionError.localizedDescription );
    }
    [[AVAudioSession sharedInstance] setActive:YES error:&sessionError];
    if ( sessionError )
    {
        NSLog( @"Audio session activation error: %@", sessionError.localizedDescription );
    }
}

- ( void )preloadSounds
{
    dispatch_async( dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ), ^{
      NSArray< NSString* >* paths = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
      NSString* documentsDirectory = [paths firstObject];
      NSString* soundsPath = [documentsDirectory stringByAppendingPathComponent:@"sounds"];

      for ( NSString* fileName in self.soundFileNames )
      {
          NSString* filePath = [soundsPath stringByAppendingPathComponent:fileName];
          if ( ![[NSFileManager defaultManager] fileExistsAtPath:filePath] )
          {
              NSLog( @"Sound file not found: %@", filePath );
              continue;
          }

          NSURL* fileURL = [NSURL fileURLWithPath:filePath];
          NSError* error = nil;
          AVAudioPlayer* player = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:&error];
          if ( error || !player )
          {
              NSLog( @"Failed to create player for %@: %@", fileName, error.localizedDescription );
              continue;
          }
          [player prepareToPlay]; // предзагрузка
          self.audioPlayers[ fileName ] = player;
      }

      // Готово к воспроизведению
      dispatch_async( dispatch_get_main_queue( ), ^{
        self.soundsReady = YES;
      } );
    } );
}

- ( void )playSoundAtIndex:( NSInteger )index
{
    if ( !self.soundsReady )
        return;
    if ( index < 0 || index >= self.soundFileNames.count )
        return;

    NSString* fileName = self.soundFileNames[ index ];
    AVAudioPlayer* player = self.audioPlayers[ fileName ];
    if ( !player )
        return;

    dispatch_async( dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ), ^{
      // dispatch_async( dispatch_get_main_queue( ), ^{
      if ( player.isPlaying )
      {
          [player stop];
      }
      player.currentTime = 0;
      [player play];
    } );
}

@end
