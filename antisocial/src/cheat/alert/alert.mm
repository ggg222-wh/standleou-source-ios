#import "alert.hpp"
#import "alert.h"
#import <objc/runtime.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

char* NStoCString( NSString* v1 )
{
    return ( char* ) ( [v1 UTF8String] );
}

std::string NStoCPPString( NSString* v1 )
{
    return std::string( NStoCString( v1 ) );
}

NSString* CPPtoNSString( const std::string& s )
{
    return [[NSString alloc] initWithUTF8String:s.c_str( )];
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
alert::show_error( std::string alertTitle, std::string description )
{
    dispatch_async( dispatch_get_main_queue( ), ^{
      UIAlertController* alertController = [UIAlertController
          alertControllerWithTitle:CPPtoNSString( alertTitle )
                           message:CPPtoNSString( description )
                    preferredStyle:UIAlertControllerStyleAlert];

      UIAlertAction* okAction = [UIAlertAction actionWithTitle:@"OK"
                                                         style:UIAlertActionStyleDefault
                                                       handler:^( UIAlertAction* _Nonnull action ) {
                                                         exit( 0 );
                                                       }];

      [alertController addAction:okAction];
      UIViewController* rootViewController =
          [[[UIApplication sharedApplication] keyWindow] rootViewController];
      [rootViewController presentViewController:alertController
                                       animated:YES
                                     completion:nil];
    } );
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
alert::create( std::string alertTitle, std::string okActionName,
               std::string placeholderName, std::string& ret )
{
    NSString* buf_obj;
    UIAlertController* alertController = [UIAlertController
        alertControllerWithTitle:CPPtoNSString( alertTitle )
                         message:buf_obj
                  preferredStyle:UIAlertControllerStyleAlert];

    UIAlertAction* okAction = [UIAlertAction
        actionWithTitle:CPPtoNSString( okActionName )
                  style:UIAlertActionStyleDefault
                handler:^( UIAlertAction* action ) {
                  ret = NStoCPPString( alertController.textFields[ 0 ].text );
                }];
    [alertController addTextFieldWithConfigurationHandler:^(
                         UITextField* _Nonnull textField ) {
      textField.placeholder = CPPtoNSString( placeholderName );
    }];

    [alertController addAction:okAction];
    UIViewController* rootViewController =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [rootViewController presentViewController:alertController
                                     animated:YES
                                   completion:nil];
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
alert::create_with_task( std::string alertTitle, std::string okActionName,
                         std::string placeholderName, std::function< void( const std::string& key ) > task )
{
    NSString* buf_obj;
    UIAlertController* alertController = [UIAlertController
        alertControllerWithTitle:CPPtoNSString( alertTitle )
                         message:buf_obj
                  preferredStyle:UIAlertControllerStyleAlert];

    UIAlertAction* okAction = [UIAlertAction
        actionWithTitle:CPPtoNSString( okActionName )
                  style:UIAlertActionStyleDefault
                handler:^( UIAlertAction* action ) {
                  task( NStoCPPString( alertController.textFields[ 0 ].text ) );
                }];
    [alertController addTextFieldWithConfigurationHandler:^(
                         UITextField* _Nonnull textField ) {
      textField.placeholder = CPPtoNSString( placeholderName );
    }];

    [alertController addAction:okAction];
    UIViewController* rootViewController =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [rootViewController presentViewController:alertController
                                     animated:YES
                                   completion:nil];
}

@implementation alert_objc

+ ( void )create_with_task:( NSString* )alertTitle
                  okAction:( NSString* )okActionName
               placeholder:( NSString* )placeholderName
                      task:( void ( ^)( const std::string* key ) )task
{
    UIAlertController* alertController = [UIAlertController
        alertControllerWithTitle:alertTitle
                         message:@""
                  preferredStyle:UIAlertControllerStyleAlert];

    [alertController addTextFieldWithConfigurationHandler:^( UITextField* _Nonnull textField ) {
      textField.placeholder = placeholderName;
    }];

    UIAlertAction* okAction = [UIAlertAction
        actionWithTitle:okActionName
                  style:UIAlertActionStyleDefault
                handler:^( UIAlertAction* _Nonnull action ) {
                  UITextField* field = alertController.textFields.firstObject;
                  if ( field )
                  {
                      std::string key = [field.text UTF8String];
                      if ( task )
                          task( &key );
                  }
                }];

    [alertController addAction:okAction];

    UIViewController* rootVC = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [rootVC presentViewController:alertController animated:YES completion:nil];
}

+ ( void )show_error:( NSString* )title message:( NSString* )message
{
    UIAlertController* alertController = [UIAlertController
        alertControllerWithTitle:title
                         message:message
                  preferredStyle:UIAlertControllerStyleAlert];

    UIAlertAction* okAction = [UIAlertAction
        actionWithTitle:@"OK"
                  style:UIAlertActionStyleDefault
                handler:nil];

    [alertController addAction:okAction];

    UIViewController* rootVC = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [rootVC presentViewController:alertController animated:YES completion:nil];
}

@end
