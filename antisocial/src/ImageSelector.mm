#import "ImageSelector.h"

@interface ImageSelector ( )
@property ( nonatomic, weak ) UIViewController* parentViewController;
@property ( nonatomic, strong ) UIImagePickerController* picker;
@property ( nonatomic, strong ) NSString* currentFilename;
@end

@implementation ImageSelector

- ( instancetype )initWithViewController:( UIViewController* )viewController
{
    self = [super init];
    if ( self )
    {
        _parentViewController = viewController;
    }
    return self;
}

- ( void )selectImageWithFilename:( NSString* )filename
{
    if ( !_parentViewController )
        return;

    self.currentFilename = filename;
    self.picker = [[UIImagePickerController alloc] init];
    self.picker.delegate = self;

    dispatch_async( dispatch_get_main_queue( ), ^{
      [_parentViewController presentViewController:self.picker animated:YES completion:nil];
    } );
}

- ( void )imagePickerController:( UIImagePickerController* )picker didFinishPickingMediaWithInfo:( NSDictionary< UIImagePickerControllerInfoKey, id >* )info
{
    UIImage* pickedImage = info[ UIImagePickerControllerOriginalImage ];
    if ( !pickedImage )
    {
        [picker dismissViewControllerAnimated:YES completion:nil];
        return;
    }

    NSString* documentsDirectory = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ).firstObject;
    NSString* filePath = [documentsDirectory stringByAppendingPathComponent:self.currentFilename];

    NSData* imageData = UIImagePNGRepresentation( pickedImage );
    [imageData writeToFile:filePath atomically:YES];

    [picker dismissViewControllerAnimated:YES completion:nil];
    self.picker = nil;
    self.currentFilename = nil;
}

- ( void )imagePickerControllerDidCancel:( UIImagePickerController* )picker
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    self.picker = nil;
    self.currentFilename = nil;
}

@end
