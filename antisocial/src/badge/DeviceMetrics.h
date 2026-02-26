#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "DeviceModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface DeviceMetrics : NSObject

- ( CGFloat )sgAppBadgeOffset;
@property ( nonatomic, readonly ) BOOL sgIsDisplayZoomed;
@property ( nonatomic, readonly ) CGFloat sgScaleFactor;
@property ( nonatomic, readonly ) BOOL sgShowAppBadge;

@end

NS_ASSUME_NONNULL_END