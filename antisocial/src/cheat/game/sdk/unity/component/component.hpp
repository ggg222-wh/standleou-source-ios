#pragma once

#include <cstdint>

namespace unity
{
    struct string;
};

class c_transform;

class c_component
{
public:
    ~c_component( ) = default;
    c_component( ) = default;

    c_transform* get_transform( );
    unity::string* get_tag( );
    uintptr_t get_game_object( );
};