#pragma once

#include <string>

class c_photon_player;

class c_photon_network
{
public:
    static uintptr_t get_room( );
    static bool is_master( c_photon_player* photon );
    static bool set_master( c_photon_player* photon );
};
