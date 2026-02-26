#import <UIKit/UIKit.h>

@class DeviceMetrics;

@interface AppBadgeView : UIView

+ ( void )updateBadgeWithMetrics:( DeviceMetrics* )metrics;

@end
