#import <UIKit/UIKit.h>

@interface TextInputManager : NSObject < UITextFieldDelegate >

@property ( nonatomic, strong ) UITextField* textField;
@property ( nonatomic, copy ) NSString* inputText;
@property ( nonatomic ) char* inputBuffer;
@property ( nonatomic ) size_t bufferSize;

+ ( instancetype )shared;

- ( void )showKeyboard;
- ( void )hideKeyboard;

@end
