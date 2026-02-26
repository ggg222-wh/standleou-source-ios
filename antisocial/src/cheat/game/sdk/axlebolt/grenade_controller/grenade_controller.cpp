#include "grenade_controller.hpp"

#include <cstdint>

bool c_grenade_controller::prev_fire_cmd( )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0xE8 );
}

bool c_grenade_controller::prev_aim_cmd( )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0xE9 );
}