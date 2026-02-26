#pragma once

#include "singleton.hpp"

#include "game/sdk/structures/vec3.hpp"

class c_removals : public singleton< c_removals >
{
public:
    ~c_removals( ) = default;
    c_removals( ) = default;

    void draw_sniper_scope( ) const;
};