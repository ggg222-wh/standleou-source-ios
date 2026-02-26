#pragma once

#include <vector>

#include "singleton.hpp"

#include "game/sdk/structures/vec3.hpp"

struct hit_marker_t
{
    vec3_t position { 0, 0, 0 };
    int damage { 0 };
    int alpha_modulate { 360 };
};

class c_hit_markers : public singleton< c_hit_markers >
{
    std::vector< hit_marker_t > m_markers { };

public:
    ~c_hit_markers( ) = default;
    c_hit_markers( ) = default;

    void push_marker( const hit_marker_t& tracer );
    void render( );
    void clear( );
};