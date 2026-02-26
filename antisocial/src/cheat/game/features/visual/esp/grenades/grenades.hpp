//
// Created by rei on 3/10/2025.
//

#pragma once

#include <chrono>
#include <vector>
#include <unordered_map>

#include "game/sdk/structures/vec3.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"
#include "game/sdk/misc/math/math.hpp"

struct esp_grenade_param_t
{
    uintptr_t ptr;
    vec3_t world;
    w2s_t screen;
    int idx;
};

struct grenade_animation_t
{
    float total_time { 1.7f };
    std::chrono::steady_clock::time_point start_time;

    std::vector< vec3_t > trail;
    c_animator alpha { float( 0.f ) };
};

class c_ent_grenade_esp
{
    esp_grenade_param_t m_param;

public:
    static std::unordered_map< uintptr_t, grenade_animation_t > animation_data;

    ~c_ent_grenade_esp( ) = default;
    explicit c_ent_grenade_esp( esp_grenade_param_t param )
        : m_param( param ) {};

    void render( );

    static void execute( uintptr_t grenade );
};