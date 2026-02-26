#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface SoundManager : NSObject

+ ( instancetype )sharedManager;

- ( void )preloadSounds;
- ( void )playSoundAtIndex:( NSInteger )index;

@property ( nonatomic, assign, readonly ) BOOL soundsReady;

@end

NS_ASSUME_NONNULL_END
