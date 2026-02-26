#pragma once

#include <cstdint>

namespace hk::player_hit_controller::hit
{
    inline void ( *og_hit )( uintptr_t, uintptr_t, uintptr_t, int, int );
    extern void hk_hit( uintptr_t, uintptr_t, uintptr_t, int, int );

    extern void hook( );
} // namespace hk::player_hit_controller::hit