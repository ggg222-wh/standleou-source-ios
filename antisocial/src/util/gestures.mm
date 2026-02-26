
#import "gestures.h"
#import <UIKit/UIKit.h>

#import "../antisocial.h"

#import "game/features/misc/peek_assist/peek_assist.hpp"
#import "game/features/ragebot/ragebot.hpp"
#import "audio_player/audio_player.hpp"

#import "globals.hpp"

@interface gestures ( )
@property ( nonatomic, strong ) antisocial* antisocial_view;
@end

@implementation gestures

static bool ui_toggled = true;

static gestures* extra_info;
static UIWindow* main_window;

+ ( void )load
{
    [super load];

    dispatch_after( dispatch_time( DISPATCH_TIME_NOW, ( int64_t ) ( 3 * NSEC_PER_SEC ) ), dispatch_get_main_queue( ), ^{
      main_window = [UIApplication sharedApplication].keyWindow;
      extra_info = [gestures new];
      [extra_info init_menu_gesture];
      [extra_info init_menu_logic];
      [extra_info init_peek_gesture];
    } );
}

- ( void )init_menu_gesture
{
    UITapGestureRecognizer* tap = [[UITapGestureRecognizer alloc] init];
    tap.numberOfTapsRequired = 2;
    tap.numberOfTouchesRequired = 3;

    UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    [rootViewController.view addGestureRecognizer:tap];

    [tap addTarget:self action:@selector( init_menu_logic )];
}

- ( void )init_menu_logic
{
    ui_toggled = !ui_toggled;

    if ( !_antisocial_view )
    {
        antisocial* vc = [[antisocial alloc] init];
        _antisocial_view = vc;
    }

    [antisocial set_presented:ui_toggled];

    if ( !ui_toggled )
    {
#if DEBUG
        [[UIApplication sharedApplication].windows[ 0 ].rootViewController.view addSubview:_antisocial_view.view];
#else
        UIView* pmtview = [UIView new];
        pmtview.frame = [UIApplication sharedApplication].keyWindow.frame;
        [pmtview setBackgroundColor:[UIColor clearColor]];
        [pmtview setUserInteractionEnabled:NO];
        UITextField* pmtfield = [[UITextField alloc] init];
        pmtfield.secureTextEntry = true;
        [pmtview addSubview:pmtfield];
        pmtview = pmtfield.layer.sublayers.firstObject.delegate;
        [pmtview addSubview:_antisocial_view.view];

        [[UIApplication sharedApplication].windows[ 0 ].rootViewController.view addSubview:pmtview];
#endif
    } else
    {
        [[UIApplication sharedApplication].windows[ 0 ].rootViewController.view addSubview:_antisocial_view.view];
    }
}

- ( void )init_peek_gesture
{
    UITapGestureRecognizer* tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector( init_peek_logic: )];
    tap.numberOfTapsRequired = 2;
    tap.numberOfTouchesRequired = 2;

    UIViewController* rootVC = [UIApplication sharedApplication].keyWindow.rootViewController;
    [rootVC.view addGestureRecognizer:tap];
}

- ( void )init_peek_logic:( UITapGestureRecognizer* )tap
{
    UIView* rootView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
    CGFloat midX = rootView.bounds.size.width / 2.0;

    BOOL leftTouched = NO;
    BOOL rightTouched = NO;

    for ( NSInteger i = 0; i < tap.numberOfTouches; i++ )
    {
        CGPoint point = [tap locationOfTouch:i inView:rootView];
        if ( point.x < midX )
        {
            leftTouched = YES;
        } else
        {
            rightTouched = YES;
        }
    }

    if ( leftTouched && rightTouched )
    {
        if ( c::get< bool >( g_ctx->cfg.misc_peek_assist ) )
        {
            g_ctx->features.peek_assist->m_peeking = !g_ctx->features.peek_assist->m_peeking;
            if ( g_ctx->features.peek_assist->m_peeking )
                g_ctx->audio_player->play( static_cast< int >( sound_t::peekstart ) );
            else
                g_ctx->audio_player->play( static_cast< int >( sound_t::peekend ) );
        }
    } else if ( leftTouched )
    {
        g_ctx->features.ragebot->manual_antiaim( antiaim_t::left );
    } else if ( rightTouched )
    {
        g_ctx->features.ragebot->manual_antiaim( antiaim_t::right );
    }
    // g_ctx->features.peek_assist->m_peeking = !g_ctx->features.peek_assist->m_peeking;
}
@end
