#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#import <mach-o/dyld.h>
#import <mach/mach.h>
#import <mach/mach_traps.h>
#import <sys/utsname.h>

#import <thread>

#import "memory_manager.hpp"
#import "globals.hpp"

// #define is_ipa

#ifdef is_ipa
#import "dobby/dobby.hpp"
#import "dobby/dependencies/h5gg_hook.h"
#import "dobby/dependencies/mem.h"
#else
#import <substrate.h>
#endif

uintptr_t memory_manager::get_base( )
{
    for ( int i = 0; i < _dyld_image_count( ); i++ )
    {
        const char* name = _dyld_get_image_name( i );
        if ( !name )
            continue;

        if ( std::strstr( name, ay_obfuscate( "UnityFramework.framework/UnityFramework" ) ) )
        {
            return static_cast< uintptr_t >( _dyld_get_image_vmaddr_slide( i ) );
        }
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    return 0;
}

#ifdef is_ipa
#define static_inline_hook( x, y, z )                                                                                              \
    NSString* result_##y = StaticInlineHookPatch( "Frameworks/UnityFramework.framework/UnityFramework", x, nullptr, ( void* ) y ); \
    if ( result_##y )                                                                                                              \
    {                                                                                                                              \
        void* result = StaticInlineHookFunction( "Frameworks/UnityFramework.framework/UnityFramework", x, ( void* ) y );           \
        *( void** ) ( z ) = ( void* ) result;                                                                                      \
    }
#endif

void memory_manager::hook( uintptr_t relative, void* hk, void** og )
{
    // relative cause ipa hooks finds it by itself
#ifdef is_ipa
    static_inline_hook( relative, hk, og );
#else
    // dobby is fucked up
    MSHookFunction( reinterpret_cast< void* >( memory_manager::get_absolute( relative ) ), hk, og );
#endif
}
#pragma endregion
