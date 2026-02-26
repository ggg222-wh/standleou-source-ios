//
// Created by rei on 3/13/2025.
//

#ifndef ONOKURO_PLANTED_BOMB_HPP
#define ONOKURO_PLANTED_BOMB_HPP

#include <unordered_map>
#include <chrono>

#include "imgui.h"
#include "imgui_internal.h"

#include "game/sdk/structures/vec3.hpp"
#include "game/sdk/misc/math/math.hpp"

#include "interface/custom_ui/advanced/utilities.hpp"

struct bomb_param_t
{
    vec3_t world;
    w2s_t screen;
    uintptr_t ptr;
};

struct bomb_animation_t
{
    double plant_time;

    c_animator alpha { float( 0.f ) };
};

class c_ent_planted_bomb_esp
{
    bomb_param_t m_param;

public:
    static std::unordered_map< uintptr_t, bomb_animation_t > animation_data;
    static bool planted;
    static double time_left;

    static double get_current_time( ) noexcept;

    ~c_ent_planted_bomb_esp( ) = default;
    explicit c_ent_planted_bomb_esp( bomb_param_t param )
        : m_param( param ) {};

    double get_time_to_detonation( double plant_time ) const noexcept;

    void render( ) const noexcept;
};

#endif // ONOKURO_PLANTED_BOMB_HPP
