#import "DeviceModel.h"
#import <sys/utsname.h>

#import "globals.hpp"

@implementation DeviceModelHelper

+ ( DeviceModel )currentDeviceModel
{
    NSString* identifier = [self deviceModelIdentifier];

    if ( [identifier isEqualToString:@"iPhone10,3"] || [identifier isEqualToString:@"iPhone10,6"] )
        return DeviceModeliPhoneX; // iPhone X (Global, GSM)
    if ( [identifier isEqualToString:@"iPhone11,2"] )
        return DeviceModeliPhoneXS; // iPhone XS
    if ( [identifier isEqualToString:@"iPhone11,4"] || [identifier isEqualToString:@"iPhone11,6"] )
        return DeviceModeliPhoneXSMax; // iPhone XS Max
    if ( [identifier isEqualToString:@"iPhone11,8"] )
        return DeviceModeliPhoneXR; // iPhone XR
    if ( [identifier isEqualToString:@"iPhone12,1"] )
        return DeviceModeliPhone11; // iPhone 11
    if ( [identifier isEqualToString:@"iPhone12,3"] )
        return DeviceModeliPhone11Pro; // iPhone 11 Pro
    if ( [identifier isEqualToString:@"iPhone12,5"] )
        return DeviceModeliPhone11ProMax; // iPhone SE 2nd Gen
    if ( [identifier isEqualToString:@"iPhone13,1"] )
        return DeviceModeliPhone12Mini; // iPhone 12 Mini
    if ( [identifier isEqualToString:@"iPhone13,2"] )
        return DeviceModeliPhone12; // iPhone 12
    if ( [identifier isEqualToString:@"iPhone13,3"] )
        return DeviceModeliPhone12Pro; // iPhone 12 Pro
    if ( [identifier isEqualToString:@"iPhone13,4"] )
        return DeviceModeliPhone12ProMax; // iPhone 12 Pro Max
    if ( [identifier isEqualToString:@"iPhone14,2"] )
        return DeviceModeliPhone13Pro; // iPhone 13 Pro
    if ( [identifier isEqualToString:@"iPhone14,3"] )
        return DeviceModeliPhone13ProMax; // iPhone 13 Pro Max
    if ( [identifier isEqualToString:@"iPhone14,4"] )
        return DeviceModeliPhone13Mini; // iPhone 13 Mini
    if ( [identifier isEqualToString:@"iPhone14,5"] )
        return DeviceModeliPhone13; // iPhone SE 3rd Gen
    if ( [identifier isEqualToString:@"iPhone14,7"] )
        return DeviceModeliPhone14; // iPhone 14
    if ( [identifier isEqualToString:@"iPhone14,8"] )
        return DeviceModeliPhone14Plus; // iPhone 14 Plus
    if ( [identifier isEqualToString:@"iPhone15,2"] )
        return DeviceModeliPhone14Pro; // iPhone 14 Pro
    if ( [identifier isEqualToString:@"iPhone15,3"] )
        return DeviceModeliPhone14ProMax; // iPhone 14 Pro Max
    if ( [identifier isEqualToString:@"iPhone15,4"] )
        return DeviceModeliPhone15; // iPhone 15
    if ( [identifier isEqualToString:@"iPhone15,5"] )
        return DeviceModeliPhone15Plus; // iPhone 15 Plus
    if ( [identifier isEqualToString:@"iPhone16,1"] )
        return DeviceModeliPhone15Pro; // iPhone 15 Pro
    if ( [identifier isEqualToString:@"iPhone16,2"] )
        return DeviceModeliPhone15ProMax; // iPhone 15 Pro Max
    if ( [identifier isEqualToString:@"iPhone17,1"] )
        return DeviceModeliPhone16Pro; // iPhone 16 Pro
    if ( [identifier isEqualToString:@"iPhone17,2"] )
        return DeviceModeliPhone16ProMax; // iPhone 16 Pro Max
    if ( [identifier isEqualToString:@"iPhone17,3"] )
        return DeviceModeliPhone16; // iPhone 16
    if ( [identifier isEqualToString:@"iPhone17,4"] )
        return DeviceModeliPhone16Plus; // iPhone 16 Plus
    if ( [identifier isEqualToString:@"iPhone17,5"] )
        return DeviceModeliPhone16e; // iPhone 16e

    return DeviceModelUnknown;
}

+ ( NSString* )deviceModelIdentifier
{
    // struct utsname systemInfo;
    // uname( &systemInfo );
    return [NSString stringWithCString:g_ctx->backup_model encoding:NSUTF8StringEncoding];
}

@end
