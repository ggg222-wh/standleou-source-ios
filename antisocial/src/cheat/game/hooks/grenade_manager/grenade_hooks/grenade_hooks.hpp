#pragma once

// я видел как ты на глазах сгораешь
// я стоял и не мог сделать ничего

#include <cstdint>

#include "game/sdk/unity/unity.hpp"

struct vec3_t;

namespace hk::grenade_manager::grenade_hooks
{
    inline void ( *og_update )( uintptr_t a );
    extern void hk_update( uintptr_t a );

    inline void ( *og_throw )( uintptr_t a, unsigned char b, vec3_t c, vec3_t d, float e, int f );
    extern void hk_throw( uintptr_t a, unsigned char b, vec3_t c, vec3_t d, float e, int f );

    inline void ( *og_throw_dropped )( uintptr_t a, vec3_t b, vec3_t c, int d, float e, double f );
    extern void hk_throw_dropped( uintptr_t a, vec3_t b, vec3_t c, int d, float e, double f );

    inline void ( *og_detonated )( uintptr_t a, vec3_t b );
    extern void hk_detonated( uintptr_t a, vec3_t b );

    extern void hook( );
} // namespace hk::grenade_manager::grenade_hooks