#pragma once

#include "singleton.hpp"

class c_player_controller;

class c_spoof : public singleton< c_spoof >
{
public:
    ~c_spoof( ) = default;
    c_spoof( ) = default;

    void uid( c_player_controller* ent ) const;
    void clan( c_player_controller* ent ) const;
};