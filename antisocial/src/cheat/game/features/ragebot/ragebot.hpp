#pragma once

#include "singleton.hpp"

#include "game/sdk/structures/vec3.hpp"
#include "game/sdk/structures/euler.hpp"

#include <chrono>

class c_gun_controller;
class c_player_controller;
class c_transform;
class c_biped_map;
class c_player_inputs;

enum class antiaim_t
{
    none,
    left,
    right
};

class c_ragebot : public singleton< c_ragebot >
{
public:
    euler_angles_t orig_camera_angles { 0, 0, 0 };

    c_transform* m_target { };
    vec3_t m_end { 0, 0, 0 };
    vec3_t m_start { 0, 0, 0 };
    vec3_t m_target_offset { 0, 0, 0 };
    bool m_fire { false };
    std::chrono::steady_clock::time_point m_doubletap_time_point { };

    antiaim_t m_manual_selected { antiaim_t::none };

public:
    c_ragebot( ) = default;
    ~c_ragebot( ) = default;

    void movement_fix( float orig_camera_yaw, float antiaim_yaw, c_player_inputs* inputs );

    void execute( c_player_controller* local );

    std::pair< bool, std::pair< vec3_t, std::pair< c_transform*, vec3_t > > > select_first_available_point( c_biped_map* biped, const vec3_t& view, c_gun_controller* gun ) const;
    c_transform* select_bone( c_biped_map* biped_map, const vec3_t& view, c_gun_controller* gun ) const;
    bool autowall( const vec3_t& start, const vec3_t& end ) const;

    void cast_hit( vec3_t& start_position, vec3_t& target_position, uintptr_t parameters );

    void doubletap( c_player_controller* local, c_gun_controller* gun, uintptr_t commands );

    void antiaim( c_player_controller* local, c_player_inputs* inputs );
    void manual_antiaim( antiaim_t which, bool sound = true );
    void render( ) const;

    void fake_lags( ) const;

    vec3_t get_original_view_angles( c_player_controller* local ) const;
    void set_angles( c_player_controller* local );
};