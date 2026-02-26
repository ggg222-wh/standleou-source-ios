#pragma once

#include <vector>

#include "singleton.hpp"

#include "game/sdk/structures/vec3.hpp"

struct bullet_tracer_t
{
    vec3_t start { 0, 0, 0 };
    vec3_t end { 0, 0, 0 };
    int alpha_modulate { 360 };
};

class c_bullet_tracers : public singleton< c_bullet_tracers >
{
    std::vector< bullet_tracer_t > m_tracers { };

public:
    ~c_bullet_tracers( ) = default;
    c_bullet_tracers( ) = default;

    void push_tracer( const bullet_tracer_t& tracer );
    void render( );
    void clear( );
};