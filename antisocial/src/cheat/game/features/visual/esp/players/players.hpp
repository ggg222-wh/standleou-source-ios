#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "game/sdk/structures/vec3.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"
#include "texture_manager/texture_manager.hpp"

struct esp_colors_t
{
    c_animator health_min { ImVec4( 0, 0, 0, 0 ) };
    c_animator health_max { ImVec4( 0, 0, 0, 0 ) };
    c_animator ammo_min { ImVec4( 0, 0, 0, 0 ) };
    c_animator ammo_max { ImVec4( 0, 0, 0, 0 ) };
    c_animator rect_start { ImVec4( 0, 0, 0, 0 ) };
    c_animator rect_end { ImVec4( 0, 0, 0, 0 ) };
    c_animator nickname { ImVec4( 0, 0, 0, 0 ) };
    c_animator nickname_end { ImVec4( 0, 0, 0, 0 ) };
    c_animator weapon { ImVec4( 0, 0, 0, 0 ) };
};

struct esp_param_t
{
    c_animator health { float( 0.f ) };
    c_animator health_alpha { float( 0.f ) };
    c_animator health_value { float( 0.f ) };
    c_animator ammo { float( 0.f ) };
    c_animator ammo_alpha { float( 0.f ) };
    c_animator ammo_value { float( 0.f ) };

    c_animator alpha { float( 0.f ) };

    c_animator weapon { float( 0.f ) };
    c_animator nickname { float( 0.f ) };
    c_animator pfp_grayscale_factor { float( 0.f ) };

    int prev_weapon_id;

    esp_colors_t colors;
};

struct ent_grenades_t
{
    bool he;
    bool smoke;
    bool flash;
};

struct esp_ent_t
{
    vec3_t world;
    ImRect bounds;
    std::string uuid;
    bool untouchable;
    int health;
    short clip;
    short max_clip;
    int weapon_id;
    std::string nickname;
    ent_grenades_t grenades;
};

struct esp_flag_t
{
    std::string value;
    ImColor color;
};

class c_ent_player_esp
{
    esp_ent_t m_ent { };
    ImDrawList* m_surface { };

public:
    ~c_ent_player_esp( ) = default;
    explicit c_ent_player_esp( const esp_ent_t& ent, ImDrawList* surface = ImGui::GetBackgroundDrawList( ) )
        : m_ent( ent )
        , m_surface( surface ) {};

    static std::unordered_map< std::string, esp_param_t > m_parameters;
    static std::unordered_map< std::string, texture_data_t > m_avatars;
    static std::unordered_map< std::string, texture_data_t > m_gray_avatars;

    void render_rect( ) const;
    void render_offscreen( ) const;
    void render_healthbar( ) const;
    void render_ammobar( bool available ) const;
    void render_weapon( const float& distance = 0 ) const;
    void render_nickname( ImTextureID pfp_texture, ImTextureID gray_pfp_texture ) const;
    void render_flags( std::vector< esp_flag_t >& flags ) const;
    void render_selected_target( const vec3_t& target, int radius ) const;
};