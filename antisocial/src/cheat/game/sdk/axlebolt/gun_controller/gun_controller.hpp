#pragma once

#include <cstdint>

enum class gun_damage_t
{
    head,
    body,
    arms,
    legs
};

class c_gun_controller
{
public:
    float& time_fired( );
    int get_damage( const gun_damage_t& type );
    uintptr_t parameters( );
    short magazine_capacity( );
    void set_magazine_capacity( short clip );
};