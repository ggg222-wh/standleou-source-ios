#pragma once

#include <vector>

#include "singleton.hpp"

#include "game/sdk/structures/vec3.hpp"

struct hit_point_t
{
    vec3_t point { 0, 0, 0 };
    int alpha_modulate { 360 };
};

class c_hit_points : public singleton< c_hit_points >
{
    std::vector< hit_point_t > m_points { };

public:
    ~c_hit_points( ) = default;
    c_hit_points( ) = default;

    void push_point( const hit_point_t& tracer );
    void render( );
    void clear( );
};