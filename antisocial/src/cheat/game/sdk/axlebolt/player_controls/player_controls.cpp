#include "player_controls.hpp"

#include <cstdint>

c_delegate* c_player_controls::input_filter( )
{
    return *reinterpret_cast< c_delegate** >( reinterpret_cast< uintptr_t >( this ) + 0x88 );
}