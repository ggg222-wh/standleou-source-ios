#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface alert_objc : NSObject

+ ( void )create_with_task:( NSString* )alertTitle
                  okAction:( NSString* )okActionName
               placeholder:( NSString* )placeholderName
                      task:( void ( ^)( const std::string* key ) )task;

@end

NS_ASSUME_NONNULL_END
