#import <UIKit/UIKit.h>
#import "DeviceModel.h"
#import "DeviceMetrics.h"

static NSArray* DEVICE_MODELS_WITH_APP_BADGE_SUPPORT = nil;

@implementation DeviceMetrics

+ ( void )initialize
{
    if ( self == [DeviceMetrics class] )
    {
        DEVICE_MODELS_WITH_APP_BADGE_SUPPORT = @[
            @( DeviceModeliPhoneX ),
            @( DeviceModeliPhoneXS ),
            @( DeviceModeliPhoneXSMax ),
            @( DeviceModeliPhoneXR ),
            @( DeviceModeliPhone11 ),
            @( DeviceModeliPhone11Pro ),
            @( DeviceModeliPhone11ProMax ),
            @( DeviceModeliPhone12 ),
            @( DeviceModeliPhone12Mini ),
            @( DeviceModeliPhone12Pro ),
            @( DeviceModeliPhone12ProMax ),
            @( DeviceModeliPhone13 ),
            @( DeviceModeliPhone13Mini ),
            @( DeviceModeliPhone13Pro ),
            @( DeviceModeliPhone13ProMax ),
            @( DeviceModeliPhone14 ),
            @( DeviceModeliPhone14Plus ),
            @( DeviceModeliPhone14Pro ),
            @( DeviceModeliPhone14ProMax ),
            @( DeviceModeliPhone15 ),
            @( DeviceModeliPhone15Plus ),
            @( DeviceModeliPhone15Pro ),
            @( DeviceModeliPhone15ProMax ),
            @( DeviceModeliPhone16 ),
            @( DeviceModeliPhone16Plus ),
            @( DeviceModeliPhone16Pro ),
            @( DeviceModeliPhone16ProMax ),
            @( DeviceModeliPhone16e )
        ];
    }
}

- ( CGFloat )sgAppBadgeOffset
{
    DeviceModel currentDevice = [DeviceModelHelper currentDeviceModel];
    CGFloat defaultOffset = 0.0;

    switch ( currentDevice )
    {
    case DeviceModeliPhoneX:
    case DeviceModeliPhoneXS:
    case DeviceModeliPhone11Pro:
    case DeviceModeliPhone12Mini:
    case DeviceModeliPhone13Mini:
        defaultOffset = 2.0;
        break;

    case DeviceModeliPhone11:
    case DeviceModeliPhoneXR:
        defaultOffset = 6.0;
        break;

    case DeviceModeliPhone11ProMax:
    case DeviceModeliPhoneXSMax:
        defaultOffset = 4.0;
        break;

    case DeviceModeliPhone12:
    case DeviceModeliPhone12Pro:
    case DeviceModeliPhone13:
    case DeviceModeliPhone13Pro:
    case DeviceModeliPhone14:
    case DeviceModeliPhone16e:
        defaultOffset = 4.0;
        break;

    case DeviceModeliPhone12ProMax:
    case DeviceModeliPhone13ProMax:
    case DeviceModeliPhone14Plus:
        defaultOffset = 6.0;
        break;

    case DeviceModeliPhone14Pro:
    case DeviceModeliPhone15:
    case DeviceModeliPhone15Pro:
    case DeviceModeliPhone16:
        defaultOffset = 18.0;
        break;

    case DeviceModeliPhone14ProMax:
    case DeviceModeliPhone15Plus:
    case DeviceModeliPhone15ProMax:
    case DeviceModeliPhone16Plus:
        defaultOffset = 19.0;
        break;

    case DeviceModeliPhone16Pro:
    case DeviceModeliPhone16ProMax:
        defaultOffset = 22.0;
        break;

    default:
        defaultOffset = 0.0; // Any device in 2025+ should be like iPhone 14 Pro or better
    }

    CGFloat offset = defaultOffset;

    // #if DEBUG
    //     NSLog( @"deviceMetrics %@. deviceModel: %ld. sgIsDisplayZoomed: %d. sgScaleFactor: %f defaultOffset: %f, offset: %f",
    //            self,
    //            ( long ) currentDevice,
    //            [self sgIsDisplayZoomed],
    //            [self sgScaleFactor],
    //            defaultOffset,
    //            offset );
    // #endif

    return offset;
}

- ( BOOL )sgIsDisplayZoomed
{
    return [UIScreen mainScreen].scale < [UIScreen mainScreen].nativeScale;
}

- ( CGFloat )sgScaleFactor
{
    return [UIScreen mainScreen].scale / [UIScreen mainScreen].nativeScale;
}

- ( BOOL )sgShowAppBadge
{
    DeviceModel currentDevice = [DeviceModelHelper currentDeviceModel];
    return [DEVICE_MODELS_WITH_APP_BADGE_SUPPORT containsObject:@( currentDevice )];
}

@end