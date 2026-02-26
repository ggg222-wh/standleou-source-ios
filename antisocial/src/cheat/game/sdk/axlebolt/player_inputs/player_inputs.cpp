#include "player_inputs.hpp"

#include "imgui.h"

#include <cstdint>

bool& c_player_inputs::fire( )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0x22 );
}

bool& c_player_inputs::drop( )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0x25 );
}

bool& c_player_inputs::pickup( )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0x28 );
}

ImVec2& c_player_inputs::delta_aim_angles( )
{
    return *reinterpret_cast< ImVec2* >( reinterpret_cast< uintptr_t >( this ) + 0x38 );
}

float& c_player_inputs::horizontal( )
{
    return *reinterpret_cast< float* >( reinterpret_cast< uintptr_t >( this ) + 0x10 );
}

float& c_player_inputs::vertical( )
{
    return *reinterpret_cast< float* >( reinterpret_cast< uintptr_t >( this ) + 0x14 );
}