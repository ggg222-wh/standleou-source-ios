#pragma once

#include <cstdint>
#include <string>
#include <map>

#include "globals.hpp"

struct ent_weapon_t
{
    std::string icon { };
    std::string name { };
    int max_clip { 0 };
};

class c_gun_controller;

class c_weapon_controller
{
public:
    static std::map< int, ent_weapon_t > get_by_id;
    static bool is_gun( int id );

    int get_id( );
    c_gun_controller* get_gun( );
    uintptr_t get_commands( );
    std::string get_name( );
    bool is_grenade( int id );
    uintptr_t lod_group( );
    bool ready( );
    bool scoped( );
    ent_weapon_t get_data( );
};