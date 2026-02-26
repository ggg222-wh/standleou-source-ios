#import "AppBadgeView.h"
#import "DeviceMetrics.h"
#import <objc/message.h>

#import "globals.hpp"

@interface AppBadgeView ( )
@property ( nonatomic, strong ) UIImageView* imageView;
@end

@implementation AppBadgeView

- ( instancetype )initWithFrame:( CGRect )frame image:( UIImage* )image
{
    self = [super initWithFrame:frame];
    if ( self )
    {
        self.imageView = [[UIImageView alloc] initWithImage:image];
        self.imageView.frame = self.bounds;
        self.imageView.contentMode = UIViewContentModeScaleAspectFit;
        [self addSubview:self.imageView];
    }
    return self;
}

+ ( void )updateBadgeWithMetrics:( DeviceMetrics* )metrics
{
    if ( !metrics )
        return;

    UIWindow* window = UIApplication.sharedApplication.windows.firstObject;
    if ( !window )
        return;

    BOOL showBadge = [metrics sgShowAppBadge];
    if ( !showBadge )
        return;

    CGFloat badgeOffset = [metrics sgAppBadgeOffset];

    NSString* imagePath = [NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ).firstObject
        stringByAppendingPathComponent:ns_encrypt( "antisocial.png" )];

    if ( ![[NSFileManager defaultManager] fileExistsAtPath:imagePath] )
    {
        NSLog( @"Image not found! 0x1b" );
        return;
    }

    UIImage* imageRaw = [UIImage imageWithContentsOfFile:imagePath];
    if ( !imageRaw )
    {
        NSLog( @"Can't load image! 0x1c" );
        return;
    }

    UIImage* originalImage = [UIImage imageWithCGImage:imageRaw.CGImage scale:3.0 orientation:imageRaw.imageOrientation];

    UIInterfaceOrientation orientation = UIApplication.sharedApplication.statusBarOrientation;
    BOOL isLandscape = UIInterfaceOrientationIsLandscape( orientation );

    CGFloat rotationDegrees = 0;
    BOOL placeRight = NO;

    if ( orientation == UIInterfaceOrientationLandscapeLeft )
    {
        rotationDegrees = 90;
        placeRight = NO;
    } else if ( orientation == UIInterfaceOrientationLandscapeRight )
    {
        rotationDegrees = -90;
        placeRight = YES;
    }

    UIImage* badgeImage = isLandscape ? [self rotatedImage:originalImage byDegrees:rotationDegrees] : originalImage;

    for ( UIView* subview in window.subviews )
    {
        if ( [subview isKindOfClass:[AppBadgeView class]] )
        {
            [subview removeFromSuperview];
        }
    }

    CGRect frame = [self makeBadgeFrameWithImageSize:badgeImage.size
                                          screenSize:UIScreen.mainScreen.bounds.size
                                      verticalOffset:badgeOffset
                                          placeRight:placeRight];

    AppBadgeView* badge = [[AppBadgeView alloc] initWithFrame:frame image:badgeImage];
    [window addSubview:badge];
}

+ ( UIImage* )rotatedImage:( UIImage* )image byDegrees:( CGFloat )degrees
{
    CGFloat radians = degrees * M_PI / 180.0;

    CGSize rotatedSize = CGSizeMake( image.size.height, image.size.width );
    UIGraphicsBeginImageContextWithOptions( rotatedSize, NO, image.scale );
    CGContextRef context = UIGraphicsGetCurrentContext( );

    CGContextTranslateCTM( context, rotatedSize.width / 2, rotatedSize.height / 2 );
    CGContextRotateCTM( context, radians );

    [image drawInRect:CGRectMake( -image.size.width / 2,
                                  -image.size.height / 2,
                                  image.size.width,
                                  image.size.height )];

    UIImage* rotated = UIGraphicsGetImageFromCurrentImageContext( );
    UIGraphicsEndImageContext( );
    return rotated;
}

+ ( CGRect )makeBadgeFrameWithImageSize:( CGSize )imageSize
                             screenSize:( CGSize )screenSize
                         verticalOffset:( CGFloat )offset
                             placeRight:( BOOL )placeRight
{

    UIInterfaceOrientation orientation = UIApplication.sharedApplication.statusBarOrientation;
    CGFloat scale = UIScreen.mainScreen.scale;

    CGFloat originX = 0;
    CGFloat originY = 0;

    if ( UIInterfaceOrientationIsLandscape( orientation ) )
    {
        originX = placeRight
                      ? round( offset * scale ) / scale
                      : round( ( screenSize.width - imageSize.width - offset ) * scale ) / scale;
        originY = round( ( screenSize.height - imageSize.height ) / 2.0 * scale ) / scale;
    } else
    {
        originX = round( ( screenSize.width - imageSize.width ) / 2.0 * scale ) / scale;
        originY = round( offset * scale ) / scale;
    }

    return CGRectMake( originX, originY, imageSize.width, imageSize.height );
}

@end
