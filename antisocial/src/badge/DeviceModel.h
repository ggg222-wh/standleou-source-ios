#import <Foundation/Foundation.h>

typedef NS_ENUM( NSInteger, DeviceModel ) {
    DeviceModeliPhoneX,
    DeviceModeliPhoneXS,
    DeviceModeliPhoneXSMax,
    DeviceModeliPhoneXR,
    DeviceModeliPhone11,
    DeviceModeliPhone11Pro,
    DeviceModeliPhone11ProMax,
    DeviceModeliPhone12,
    DeviceModeliPhone12Mini,
    DeviceModeliPhone12Pro,
    DeviceModeliPhone12ProMax,
    DeviceModeliPhone13,
    DeviceModeliPhone13Mini,
    DeviceModeliPhone13Pro,
    DeviceModeliPhone13ProMax,
    DeviceModeliPhone14,
    DeviceModeliPhone14Plus,
    DeviceModeliPhone14Pro,
    DeviceModeliPhone14ProMax,
    DeviceModeliPhone15,
    DeviceModeliPhone15Plus,
    DeviceModeliPhone15Pro,
    DeviceModeliPhone15ProMax,
    DeviceModeliPhone16,
    DeviceModeliPhone16Plus,
    DeviceModeliPhone16Pro,
    DeviceModeliPhone16ProMax,
    DeviceModeliPhone16e,
    DeviceModelUnknown
};

@interface DeviceModelHelper : NSObject

+ ( DeviceModel )currentDeviceModel;
+ ( NSString* )deviceModelIdentifier;

@end
