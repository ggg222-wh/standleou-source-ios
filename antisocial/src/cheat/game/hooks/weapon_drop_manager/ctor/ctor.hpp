#pragma once

#include <cstdint>

#include "game/sdk/unity/unity.hpp"

namespace hk::weapon_drop_manager::ctor
{
    inline void ( *og_ctor )( uintptr_t );
    extern void hk_ctor( uintptr_t );

    extern void hook( );
} // namespace hk::weapon_drop_manager::ctor