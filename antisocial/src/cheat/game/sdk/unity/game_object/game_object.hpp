#pragma once

#include <cstdint>

#include "../objects/objects.hpp"

class c_component;

class c_game_object : public c_object
{
public:
    c_game_object( ) = default;
    ~c_game_object( ) = default;

    void create( const char* const& name );
    c_component* add_component( uintptr_t type );
};