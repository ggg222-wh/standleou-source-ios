#import "TextInputManager.h"

#import "interface/custom_ui/advanced/elements.hpp"

#import "imgui.h"
#import "imgui_internal.h"

@implementation TextInputManager

+ ( instancetype )shared
{
    static TextInputManager* instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once( &onceToken, ^{
      instance = [[TextInputManager alloc] init];
    } );
    return instance;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        self.textField = [[UITextField alloc] initWithFrame:CGRectZero];
        self.textField.delegate = self;
        self.textField.hidden = YES;
        [[UIApplication sharedApplication].keyWindow addSubview:self.textField];

        self.bufferSize = 256;
        self.inputBuffer = ( char* ) malloc( self.bufferSize );
        memset( self.inputBuffer, 0, self.bufferSize );
    }
    return self;
}

- ( void )dealloc
{
    free( self.inputBuffer );
}

- ( void )showKeyboard
{
    [self.textField becomeFirstResponder];
}

- ( void )hideKeyboard
{
    [self.textField resignFirstResponder];
}

- ( BOOL )textField:( UITextField* )textField shouldChangeCharactersInRange:( NSRange )range replacementString:( NSString* )string
{
    NSString* newText = [textField.text stringByReplacingCharactersInRange:range withString:string];
    self.inputText = newText;
    strncpy( self.inputBuffer, [newText UTF8String], self.bufferSize );
    return YES;
}

@end

bool elem::input_text( const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags )
{
    ImGuiInputTextFlags final_flags = flags | ImGuiInputTextFlags_NoUndoRedo | ImGuiInputTextFlags_ReadOnly;
    IM_ASSERT( !( final_flags & ImGuiInputTextFlags_Multiline ) );
    bool value_changed = ImGui::InputTextEx( label, label, buf, buf_size, ImVec2( 0, 0 ), final_flags );
    // bool value_changed = ImGui::InputTextEx( label, buf, buf_size, final_flags );
    if ( ImGui::IsItemActivated( ) )
    {
        [[TextInputManager shared] showKeyboard];
        [TextInputManager shared].inputText = [NSString stringWithUTF8String:buf];
        strncpy( [TextInputManager shared].inputBuffer, buf, [TextInputManager shared].bufferSize );
    }

    if ( ImGui::IsItemDeactivated( ) )
    {
        [[TextInputManager shared] hideKeyboard];
    }

    if ( [TextInputManager shared].inputText )
    {
        strncpy( buf, [TextInputManager shared].inputBuffer, buf_size );
        value_changed = true;
    }
    return value_changed;
}
