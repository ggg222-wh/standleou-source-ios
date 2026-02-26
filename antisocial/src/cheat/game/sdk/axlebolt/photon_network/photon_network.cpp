#include "photon_network.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

#include "game/sdk/axlebolt/photon_player/photon_player.hpp"

uintptr_t c_photon_network::get_room( )
{
    static auto get_room = reinterpret_cast< uintptr_t ( * )( ) >( memory_manager::get_absolute( ( 0x1478524 ) ) );
    return get_room( );
}

bool c_photon_network::is_master( c_photon_player* photon )
{
    static auto fn = reinterpret_cast< c_photon_player* ( * ) ( ) >( memory_manager::get_absolute( ( 0x1478658 ) ) );
    auto* const master = fn( );
    return master && photon == master;
}

bool c_photon_network::set_master( c_photon_player* photon )
{
    static auto fn = reinterpret_cast< bool ( * )( c_photon_player* ) >( memory_manager::get_absolute( ( 0x14800CC ) ) );
    return fn( photon );
}