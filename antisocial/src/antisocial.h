
#import <UIKit/UIKit.h>
#import <MetalKit/MetalKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface antisocial : UIViewController < UIImagePickerControllerDelegate, UINavigationControllerDelegate, MTKViewDelegate >

+ ( void )set_presented:( BOOL )value;

@end

NS_ASSUME_NONNULL_END
