#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface WebSocketManager : NSObject

- ( instancetype )initWithURL:( NSURL* )url;
- ( void )connect;
- ( void )sendMessage:( NSString* )message;

@end

NS_ASSUME_NONNULL_END
