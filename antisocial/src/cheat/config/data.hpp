#pragma once

#include "data_management.hpp"

#include "game/sdk/structures/vec3.hpp"

#include "imgui.h"

struct antiaim_config_t
{
    // jitter
    bool jitter = false;
    bool jitter_ranged = false;
    bool jitter_in_air = false;
    int jitter_frames = 5;
    int jitter_range = 20;   // ranged
    int jitter_first = -155; // yaw 1
    int jitter_second = 165; // yaw2

    // default
    int pitch_type = 0; // 0 - default, 1 - down, 2 - up
    int yaw_type = 0;   // 0 - default, 1 - backwards, 2 - forwards
    int yaw_select = 0; // 0 - at local, 2 - at enemy
    bool manuals = false;
};

struct spoof_config_t
{
    bool enabled = false;
    std::string spoof = "antisocial";
    bool button = false; // temp var
};

struct chams_config_t
{
    bool enabled = false;
    ImVec4 invisible_color = { 1, 0, 0, 1 };
    ImVec4 visible_color = { 0, 1, 0, 1 };
    ImVec4 outline_invisible_color = { 1, 0, 0, 1 }; // only pixel
    ImVec4 outline_visible_color = { 0, 1, 0, 1 };   // only pixel
    ImVec4 glow_invisible_color = { 1, 0, 0, 1 };    // only vischeck
    ImVec4 glow_visible_color = { 0, 1, 0, 1 };      // only vischeck
    float glow_width = 0.1f;                         // only vischeck
    int shader_type = 0;                             // 0 - glow, 1 - sky, 2 - visible check
    bool remove_texture = false;
    float outline_thickness = 0.02f; // only pixel
    float time_speed = 1.f;          // only pixel
    float pixel_density = 3.f;       // only pixel
    bool custom_texture = false;
};

struct cfg_t
{
    declare_variable( bool, esp_rect, false );
    declare_variable( ImVec4, esp_rect_start, ImVec4( 1, 1, 1, 1 ) );
    declare_variable( ImVec4, esp_rect_end, ImVec4( 1, 1, 1, 1 ) );
    declare_variable( bool, esp_health, false );
    declare_variable( bool, esp_health_gradient, false );
    declare_variable( ImVec4, esp_health_min, ImVec4( 1, 0, 0, 1 ) );
    declare_variable( ImVec4, esp_health_max, ImVec4( 0, 1, 0, 1 ) );
    declare_variable( bool, esp_ammo, false );
    declare_variable( bool, esp_ammo_gradient, false );
    declare_variable( ImVec4, esp_ammo_min, ImVec4( 1, 0, 0, 1 ) );
    declare_variable( ImVec4, esp_ammo_max, ImVec4( 0, 1, 0, 1 ) );
    declare_variable( bool, esp_weapon, false );
    declare_variable( bool, esp_weapon_icon, false );
    declare_variable( bool, esp_weapon_grenades, false );
    declare_variable( ImVec4, esp_weapon_clr, ImVec4( 1, 1, 1, 1 ) );
    declare_variable( bool, esp_nickname, false );
    declare_variable( ImVec4, esp_nickname_clr, ImVec4( 1, 1, 1, 1 ) );
    declare_variable( ImVec4, esp_nickname_clr_end, ImVec4( 1, 1, 1, 1 ) );
    declare_variable( bool, esp_distance, false );
    declare_variable( bool, esp_money, false );
    declare_variable( bool, esp_armor, false );
    declare_variable( bool, esp_scoped, false );
    declare_variable( bool, esp_open, false );
    declare_variable( bool, esp_profile_pictures, false );
    declare_variable( bool, esp_shadows, false );
    declare_variable( bool, esp_offscreen, false );
    declare_variable( bool, esp_target, false );
    declare_variable( ImVec4, esp_target_clr, ImVec4( 1, 0, 0, 1 ) );
    declare_variable( int, esp_nickname_font, 0 ); // 0 - verdana, 1 - sf pro bold

    declare_variable( bool, ragebot_enabled, false );
    declare_variable( bool, ragebot_silent, false );
    declare_variable( bool, ragebot_triggerbot, false );
    declare_variable( bool, ragebot_antiaim, false );
    declare_variable( antiaim_config_t, ragebot_antiaim_config, antiaim_config_t( ) );
    declare_variable( bool, ragebot_legit_selection, true );
    declare_variable( bool, ragebot_multipoints, false );
    declare_variable( float, ragebot_multipoints_scale_head, 0.1f );
    declare_variable( float, ragebot_multipoints_scale_body, 0.1f );
    declare_variable( float, ragebot_multipoints_scale_legs, 0.1f );
    declare_variable( float, ragebot_multipoints_scale_arms, 0.1f );
    declare_variable( float, ragebot_multipoints_tightness, 0.1f );
    declare_variable( bool, ragebot_head, true );
    declare_variable( bool, ragebot_body, false );
    declare_variable( bool, ragebot_arms, false );
    declare_variable( bool, ragebot_legs, false );
    declare_variable( bool, ragebot_autowall, false );
    declare_variable( bool, ragebot_doubletap, false );
    declare_variable( int, ragebot_minimal_damage, 0 );
    declare_variable( bool, ragebot_fakelags, false );
    declare_variable( int, ragebot_fakelags_rate, 3 );
    declare_variable( int, ragebot_fov, 0 );

    declare_variable( bool, visual_bullet_tracers, false );
    declare_variable( ImVec4, visual_bullet_tracers_color, ImVec4( 1, 1, 1, 1 ) );
    declare_variable( bool, visual_hit_markers, false );
    declare_variable( bool, visual_hit_points, false );

    declare_variable( bool, misc_thirdperson, false );
    declare_variable( bool, misc_peek_assist, false );

    declare_variable( bool, movement_bunnyhop, false );

    declare_variable( ImVec4, world_modulation, ImVec4( 0.1f, 0.1f, 0.1f, 1.0f ) );
    declare_variable( bool, world_modulation_auto, false );
    declare_variable( bool, world_modulate, false );

    declare_variable( bool, misc_aspect_ratio_enabled, false );
    declare_variable( float, misc_aspect_ratio, 3.13f );

    declare_variable( chams_config_t, chams_enemies, chams_config_t( ) );
    declare_variable( chams_config_t, chams_local, chams_config_t( ) );
    declare_variable( chams_config_t, chams_weapon, chams_config_t( ) );

    declare_variable( spoof_config_t, misc_spoof_uid, spoof_config_t( ) );
    declare_variable( spoof_config_t, misc_spoof_clan, spoof_config_t( ) );

    declare_variable( bool, misc_nuke, false );
    declare_variable( int, misc_nuke_type, 0 );

    declare_variable( bool, removals_scope_overlay, false );

    declare_variable( bool, misc_fov_enabled, false );
    declare_variable( int, misc_fov, 60 );

    declare_variable( bool, misc_hit_sound, false );

    declare_variable( bool, visual_fog_enabled, false );
    declare_variable( float, visual_fog_start, 4.f );
    declare_variable( float, visual_fog_density, 1 );
    declare_variable( float, visual_fog_end, 40.f );
    declare_variable( int, visual_fog_mode, 0 );
    declare_variable( ImVec4, visual_fog_color, ImVec4( 0.f, 0.f, 0.f, 1.f ) );

    declare_variable( bool, visual_grenade_prediction, false );
    declare_variable( ImVec4, visual_grenade_prediction_color, ImVec4( 1, 1, 1, 1 ) );

    declare_variable( bool, visual_grenades_esp, false );
    declare_variable( ImVec4, visual_grenades_esp_color, ImVec4( 1, 1, 1, 1 ) );

    declare_variable( bool, visual_dropped_weapons, false );
    declare_variable( bool, visual_dropped_weapons_ammo, false );
    declare_variable( ImVec4, visual_dropped_weapons_ammo_color, ImColor( 55, 155, 255 ).Value );
    declare_variable( ImVec4, visual_dropped_weapons_color, ImVec4( 1, 1, 1, 1 ) );

    declare_variable( bool, visual_planted_bomb, false );

    declare_variable( bool, widgets_active_modules, false );
    declare_variable( ImVec2, widgets_pos_modules, ImVec2( 40, 20 ) );

    declare_variable( bool, widgets_active_bomb, false );
    declare_variable( ImVec2, widgets_pos_bomb, ImVec2( 40, 60 ) );

    declare_variable( bool, widgets_active_players, false );
    declare_variable( ImVec2, widgets_pos_players, ImVec2( 40, 100 ) );

    declare_variable( bool, misc_chat_spam, true );
    declare_variable( bool, misc_irc_chat, true );

    declare_variable( bool, exploits_rapid_fire, false );
    declare_variable( bool, exploits_infinite_ammo, false );
    declare_variable( bool, exploits_no_spread, false );

    declare_variable( bool, visual_skybox_enabled, false );
    declare_variable( ImVec4, visual_skybox, ImVec4( 0, 0, 0, 1 ) );
    declare_variable( bool, visual_skybox_texture, false );

    declare_variable( bool, world_particles_enabled, false );
    declare_variable( bool, visual_hit_particles_enabled, false );
    declare_variable( ImVec4, world_particles_color, ImVec4( 1, 1, 1, 1 ) );

    declare_variable( bool, misc_trash_talk, false );

    declare_variable( int, previews_model_hitboxes, 0 );
    declare_variable( int, previews_model_esp, 0 );
};