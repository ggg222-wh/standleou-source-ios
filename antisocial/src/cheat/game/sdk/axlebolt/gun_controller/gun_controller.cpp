#include "gun_controller.hpp"

#include <cstdint>

#include "globals.hpp"
#include "../../il2cpp/il2cpp.hpp"

#include "memory_manager/memory_manager.hpp"

float& c_gun_controller::time_fired( )
{
    return *reinterpret_cast< float* >( reinterpret_cast< uintptr_t >( this ) + 0x18C );
}

int c_gun_controller::get_damage( const gun_damage_t& type )
{
    if ( !reinterpret_cast< uintptr_t >( this ) )
        return 0;

    uintptr_t const param = parameters( );
    if ( !param )
        return 0;

    uintptr_t const damage = *reinterpret_cast< uintptr_t* >( param + 0x88 );
    if ( !damage )
        return 0;

    return [ damage, type ]( ) -> int {
        switch ( type )
        {
        case gun_damage_t::head:
            return *reinterpret_cast< int* >( damage + 0x10 );
        case gun_damage_t::body:
            return *reinterpret_cast< int* >( damage + 0x14 );
        case gun_damage_t::arms:
            return *reinterpret_cast< int* >( damage + 0x18 );
        case gun_damage_t::legs:
            return *reinterpret_cast< int* >( damage + 0x1C );
        default:
            return 0;
        };
    }( );
}

uintptr_t c_gun_controller::parameters( )
{
    return *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x1A0 );
}

short c_gun_controller::magazine_capacity( )
{
    uintptr_t const safe_capacity = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0xD8 );
    if ( !safe_capacity )
        return 0;

    return static_cast< short >( *reinterpret_cast< int* >( safe_capacity + 0x14 ) - *reinterpret_cast< int* >( safe_capacity + 0x10 ) );
}

void c_gun_controller::set_magazine_capacity( short clip )
{
    uintptr_t const safe_capacity = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0xD8 );
    if ( !safe_capacity )
        return;

    *reinterpret_cast< int* >( safe_capacity + 0x14 ) = static_cast< int >( clip ) + *reinterpret_cast< int* >( safe_capacity + 0x10 );
}