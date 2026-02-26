#pragma once

#include <vector>

#include "singleton.hpp"

#include "game/sdk/structures/vec3.hpp"

#include "imgui.h"

class c_particles : public singleton< c_particles >
{
    struct particle_t
    {
        vec3_t position;
        ImVec2 position_screen { 0, 0 };
        vec3_t velocity;
        float alpha { 1 };
        bool checker { };
        bool visible { false };
    };

    std::vector< particle_t > m_ambient_particles;

public:
    ~c_particles( ) = default;
    c_particles( ) = default;

    void ambient( );

    void hit_effect( const vec3_t& where );
};