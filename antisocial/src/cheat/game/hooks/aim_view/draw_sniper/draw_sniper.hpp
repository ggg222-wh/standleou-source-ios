
#pragma once

#include <cstdint>

namespace hk::aim_view::draw_sniper
{
    inline void ( *og_draw )( uintptr_t, uintptr_t );
    extern void hk_draw( uintptr_t, uintptr_t );

    inline void ( *og_draw1 )( uintptr_t, uintptr_t );
    extern void hk_draw1( uintptr_t, uintptr_t );

    inline void ( *og_draw2 )( uintptr_t, uintptr_t );
    extern void hk_draw2( uintptr_t, uintptr_t );

    extern void hook( );
} // namespace hk::aim_view::draw_sniper
