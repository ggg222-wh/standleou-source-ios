#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ImageSelector : NSObject < UIImagePickerControllerDelegate, UINavigationControllerDelegate >

- ( instancetype )initWithViewController:( UIViewController* )viewController;

- ( void )selectImageWithFilename:( NSString* )filename;

@end

NS_ASSUME_NONNULL_END
