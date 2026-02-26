#include "players.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/axlebolt/gun_controller/gun_controller.hpp"
#include "game/sdk/axlebolt/biped_map/biped_map.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/misc/math/math.hpp"
#include "game/sdk/axlebolt/bomb_manager/bomb_manager.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/ragebot/ragebot.hpp"

#include "game/entities/entities.hpp"

#include "../esp.hpp"

#include "globals.hpp"

std::unordered_map< std::string, esp_param_t > c_ent_player_esp::m_parameters;
std::unordered_map< std::string, texture_data_t > c_ent_player_esp::m_avatars;
std::unordered_map< std::string, texture_data_t > c_ent_player_esp::m_gray_avatars;

void c_ent_player_esp::render_rect( ) const
{
    m_parameters[ m_ent.uuid ].colors.rect_start.update< ImVec4 >( !m_ent.untouchable ? c::get< ImVec4 >( g_ctx->cfg.esp_rect_start ) : c::to_grayscale( c::get< ImVec4 >( g_ctx->cfg.esp_rect_start ) ).Value );
    m_parameters[ m_ent.uuid ].colors.rect_end.update< ImVec4 >( !m_ent.untouchable ? c::get< ImVec4 >( g_ctx->cfg.esp_rect_end ) : c::to_grayscale( c::get< ImVec4 >( g_ctx->cfg.esp_rect_end ) ).Value );

    float const alpha = m_parameters[ m_ent.uuid ].alpha.get< float >( );
    ImColor const col_start = m_parameters[ m_ent.uuid ].colors.rect_start.get< ImVec4 >( );
    ImColor const col_end = m_parameters[ m_ent.uuid ].colors.rect_end.get< ImVec4 >( );

    const float mult_alpha = std::min( col_start.Value.w, col_end.Value.w );
    m_surface->AddRect( m_ent.bounds.Min + ImVec2( 1, 1 ), m_ent.bounds.Max - ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, 200 / 255.f * alpha * mult_alpha ) );
    m_surface->AddRect( m_ent.bounds.Min - ImVec2( 1, 1 ), m_ent.bounds.Max + ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, 200 / 255.f * alpha * mult_alpha ) );

    int vert_start_idx = m_surface->VtxBuffer.Size;
    m_surface->AddRect( m_ent.bounds.Min, m_ent.bounds.Max, ImColor( 1.f, 1.f, 1.f, 0.9f * alpha * mult_alpha ) );
    int vert_end_idx = m_surface->VtxBuffer.Size;

    ImGui::ShadeVertsLinearColorGradientKeepAlpha( m_surface, vert_start_idx, vert_end_idx, m_ent.bounds.Min, m_ent.bounds.Max, col_start, col_end );
}

void c_ent_player_esp::render_offscreen( ) const
{
    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
        return;

    auto const display_size = ImGui::GetIO( ).DisplaySize;

    w2s_t const w2s_player = math::world_to_screen( m_ent.world + vec3_t( 0.f, 1.0f, 0.f ) );
    auto const w2s_center = w2s_player.position;

    if ( ( w2s_center.x < 0 || w2s_center.x > display_size.x ) || ( w2s_center.y < 0 || w2s_center.y > display_size.y ) || !w2s_player.check )
    {
        ImVec2 const center = ImGui::GetIO( ).DisplaySize / 2;

        vec3_t const local_pos = g_ctx->features.thirdperson->get_unmodified_view( );
        vec3_t const player_pos = m_ent.world;
        vec3_t const euler_angles = g_ctx->features.ragebot->get_original_view_angles( local );

        auto const vec2_length = [ & ]( float x, float y ) -> float {
            return std::sqrt(
                ( x * x ) +
                ( y * y ) );
        };

        auto const vec2_normalize = [ & ]( const ImVec2& input ) -> ImVec2 {
            return ImVec2( input.x / vec2_length( input.x, input.y ), input.y / vec2_length( input.x, input.y ) );
        };

        ImVec2 const position = vec2_normalize( { local_pos.x - player_pos.x, local_pos.z - player_pos.z } );

        float const angle = atan2( position.x, position.y ) * 57.29578f - 180.f - euler_angles.y;

        int const distance_divided = 0; // static_cast<int>(vec3_t::Distance(pos, viewpos) / 10.f);

        ImVec2 const pos_0 = math::cos_tan_horizontal( angle, 10.f, center.x, center.y, 60 + distance_divided );
        ImVec2 const pos_1 = math::cos_tan_horizontal( angle + 5.f - distance_divided / 2, 10.f, center.x, center.y, 50 + distance_divided );
        ImVec2 const pos_2 = math::cos_tan_horizontal( angle - 5.f - distance_divided / 2, 10.f, center.x, center.y, 50 + distance_divided );
        ImVec2 const pos_3 = math::cos_tan_horizontal( angle, 10.f, center.x, center.y, 52 + distance_divided );

        ImVec2 const pos_center = math::cos_tan_horizontal( angle, 10.f, center.x, center.y, 55 + distance_divided );

        ImGui::GetBackgroundDrawList( )->AddShadowCircle( pos_center, 1.f, ImColor( 255, 255, 255, static_cast< int >( 255 * m_parameters[ m_ent.uuid ].alpha.get< float >( ) ) ), 100, { 0, 0 }, 0, 999 );
        ImGui::GetBackgroundDrawList( )->AddTriangleCutout( ImVec2( pos_0.x, pos_0.y ), ImVec2( pos_1.x, pos_1.y ), ImVec2( pos_2.x, pos_2.y ), ImVec2( pos_3.x, pos_3.y ), ImColor( 255, 255, 255, static_cast< int >( 255 * m_parameters[ m_ent.uuid ].alpha.get< float >( ) ) ) );
    }
}

void c_ent_player_esp::render_healthbar( ) const
{
    int const health = m_ent.health;

    ImColor const health_min = c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_health_min ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) );
    ImColor const health_max = c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_health_max ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) );

    m_parameters[ m_ent.uuid ].health_alpha.update< float >( c::get< bool >( g_ctx->cfg.esp_health ) ? 1.f : 0.f );

    if ( m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) < 0.01f )
        return;

    m_parameters[ m_ent.uuid ].colors.health_min.update( !m_ent.untouchable ? health_min.Value : c::to_grayscale( health_min ).Value );
    m_parameters[ m_ent.uuid ].colors.health_max.update( !m_ent.untouchable ? health_max.Value : c::to_grayscale( health_max ).Value );

    char text[ 10 ];
    snprintf( text, 10, "%i", static_cast< int >( health ) );
    ImVec2 const text_size = c_esp::m_fonts.pixel->CalcTextSizeA( 10, FLT_MAX, -1, text );

    if ( m_parameters[ m_ent.uuid ].health.get< float >( ) > 100 )
        m_parameters[ m_ent.uuid ].health.set< float >( static_cast< float >( health ) );

    m_parameters[ m_ent.uuid ].health.update< float >( health );

    float const progress = std::clamp( m_parameters[ m_ent.uuid ].health.get< float >( ), 0.0f, 100.f ) / 100.f;

    ImColor const color = math::color_lerp( m_parameters[ m_ent.uuid ].colors.health_min.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].colors.health_max.get< ImVec4 >( ), progress );

    m_surface->AddRectFilled( { m_ent.bounds.Min.x - 5, m_ent.bounds.Min.y }, { m_ent.bounds.Min.x - 3, m_ent.bounds.Max.y }, ImColor( 0.f, 0.f, 0.f, 165 / 255.f * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) ) );

    int vert_start_idx = m_surface->VtxBuffer.Size;

    m_surface->AddRectFilled( { m_ent.bounds.Min.x - 5, m_ent.bounds.Max.y - m_ent.bounds.GetHeight( ) * progress }, { m_ent.bounds.Min.x - 3, m_ent.bounds.Max.y }, color );

    if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
        m_surface->AddShadowRect( { m_ent.bounds.Min.x - 5, m_ent.bounds.Max.y - m_ent.bounds.GetHeight( ) * progress }, { m_ent.bounds.Min.x - 3, m_ent.bounds.Max.y }, color, 30, { 0, 0 }, 0, 32 );

    int vert_end_idx = m_surface->VtxBuffer.Size;

    if ( c::get< bool >( g_ctx->cfg.esp_health_gradient ) )
        ImGui::ShadeVertsLinearColorGradientKeepAlpha( m_surface, vert_start_idx, vert_end_idx, { m_ent.bounds.Min.x - 5, m_ent.bounds.Max.y - m_ent.bounds.GetHeight( ) * progress }, { m_ent.bounds.Min.x - 3, m_ent.bounds.Max.y }, color, ImColor( m_parameters[ m_ent.uuid ].colors.health_min.get< ImVec4 >( ) ) );

    m_surface->AddRect( { m_ent.bounds.Min.x - 6, m_ent.bounds.Min.y - 1 }, { m_ent.bounds.Min.x - 2, m_ent.bounds.Max.y + 1 }, ImColor( 0.f, 0.f, 0.f, 200 / 255.f * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) ) );

    if ( health > 10 && health < 90 )
        m_parameters[ m_ent.uuid ].health_value.update< float >( 1.f );
    else
        m_parameters[ m_ent.uuid ].health_value.update< float >( 0.f );

    if ( m_parameters[ m_ent.uuid ].health_value.get< float >( ) > 0.01f )
    {
        math::add_outlined_text( m_surface, c_esp::m_fonts.pixel, 10, { m_ent.bounds.Min.x - 3 - text_size.x / 2, m_ent.bounds.Max.y - ( m_ent.bounds.GetHeight( ) * progress ) - text_size.y / 2 }, ImColor( 1.f, 1.f, 1.f, m_parameters[ m_ent.uuid ].health_value.get< float >( ) * m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) * m_parameters[ m_ent.uuid ].alpha.get< float >( ) ), ImColor( 0.f, 0.f, 0.f, 0.9f * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].health_value.get< float >( ) * m_parameters[ m_ent.uuid ].health_alpha.get< float >( ) ), text );
    }
}

void c_ent_player_esp::render_ammobar( bool available ) const
{
    ImColor const ammo_min = c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_ammo_min ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) );
    ImColor const ammo_max = c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_ammo_max ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) );

    m_parameters[ m_ent.uuid ].ammo_alpha.update< float >( ( available && c::get< bool >( g_ctx->cfg.esp_ammo ) ) ? 1.f : 0.f );

    if ( m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) < 0.01f )
        return;

    m_parameters[ m_ent.uuid ].colors.ammo_min.update( !m_ent.untouchable ? ammo_min.Value : c::to_grayscale( ammo_min ).Value );
    m_parameters[ m_ent.uuid ].colors.ammo_max.update( !m_ent.untouchable ? ammo_max.Value : c::to_grayscale( ammo_max ).Value );

    short const clip = m_ent.clip;
    int const max_clip = m_ent.max_clip;

    char text[ 10 ];
    snprintf( text, 10, "%i", static_cast< int >( clip ) );
    ImVec2 const text_size = c_esp::m_fonts.pixel->CalcTextSizeA( 10, FLT_MAX, -1, text );

    if ( m_parameters[ m_ent.uuid ].ammo.get< float >( ) >= static_cast< float >( max_clip ) )
        m_parameters[ m_ent.uuid ].ammo.set< float >( static_cast< float >( clip ) );

    m_parameters[ m_ent.uuid ].ammo.update< float >( clip );

    if ( m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) )
    {
        float const progress = std::clamp( m_parameters[ m_ent.uuid ].ammo.get< float >( ), 0.0f, static_cast< float >( max_clip ) ) / static_cast< float >( max_clip );

        ImColor const color = math::color_lerp( m_parameters[ m_ent.uuid ].colors.ammo_min.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].colors.ammo_max.get< ImVec4 >( ), progress );

        m_surface->AddRectFilled( { m_ent.bounds.Min.x, m_ent.bounds.Max.y + 3 }, { m_ent.bounds.Max.x, m_ent.bounds.Max.y + 3 + ( 2 * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) }, ImColor( 0.f, 0.f, 0.f, 165 / 255.f * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) );

        int vert_start_idx = m_surface->VtxBuffer.Size;

        m_surface->AddRectFilled( { m_ent.bounds.Min.x, m_ent.bounds.Max.y + 3 }, { m_ent.bounds.Min.x + ( m_ent.bounds.GetWidth( ) * progress ), m_ent.bounds.Max.y + 3 + ( 2 * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) }, color );
        if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
            m_surface->AddShadowRect( { m_ent.bounds.Min.x, m_ent.bounds.Max.y + 3 }, { m_ent.bounds.Min.x + ( m_ent.bounds.GetWidth( ) * progress ), m_ent.bounds.Max.y + 3 + ( 2 * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) }, color, 30, { 0, 0 }, 0, 32 );

        int vert_end_idx = m_surface->VtxBuffer.Size;

        if ( c::get< bool >( g_ctx->cfg.esp_ammo_gradient ) )
            ImGui::ShadeVertsLinearColorGradientKeepAlpha( m_surface, vert_start_idx, vert_end_idx, { m_ent.bounds.Min.x, m_ent.bounds.Max.y + 3 }, { m_ent.bounds.Min.x + ( m_ent.bounds.GetWidth( ) * progress ), m_ent.bounds.Max.y + 3 + ( 2 * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) }, ImColor( m_parameters[ m_ent.uuid ].colors.ammo_min.get< ImVec4 >( ) ), color );

        m_surface->AddRect( { m_ent.bounds.Min.x - 1, m_ent.bounds.Max.y + 2 }, { m_ent.bounds.Max.x + 1, m_ent.bounds.Max.y + 2 + ( 4 * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) }, ImColor( 0.f, 0.f, 0.f, 200 / 255.f * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ) );

        if ( clip > 0 && clip < ( max_clip - 2 ) )
            m_parameters[ m_ent.uuid ].ammo_value.update< float >( 1.f );
        else
            m_parameters[ m_ent.uuid ].ammo_value.update< float >( 0.f );

        if ( m_parameters[ m_ent.uuid ].ammo_value.get< float >( ) > 0.01f && m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) > 0.5f )
        {
            math::add_outlined_text( m_surface, c_esp::m_fonts.pixel, 10, { m_ent.bounds.Min.x + ( m_ent.bounds.GetWidth( ) * progress ) - text_size.x / 2, m_ent.bounds.Max.y + 3 - text_size.y / 2 }, ImColor( 1.f, 1.f, 1.f, m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_value.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ), ImColor( 0.f, 0.f, 0.f, 0.9f * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_value.get< float >( ) * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) ), text );
        }
    } else
        m_parameters[ m_ent.uuid ].ammo_alpha.update< float >( 0.f );
}

void c_ent_player_esp::render_weapon( const float& distance ) const
{
    bool const distance_enabled = c::get< bool >( g_ctx->cfg.esp_distance );
    bool const text_enabled = c::get< bool >( g_ctx->cfg.esp_weapon );
    bool const icon_enabled = c::get< bool >( g_ctx->cfg.esp_weapon_icon );
    bool const grenades_enabled = c::get< bool >( g_ctx->cfg.esp_weapon_grenades );

    if ( m_parameters[ m_ent.uuid ].prev_weapon_id != m_ent.weapon_id )
    {
        m_parameters[ m_ent.uuid ].weapon.set< float >( 0.f );
        m_parameters[ m_ent.uuid ].prev_weapon_id = m_ent.weapon_id;
    }

    m_parameters[ m_ent.uuid ].weapon.update< float >( ( c::get< bool >( g_ctx->cfg.esp_weapon ) || c::get< bool >( g_ctx->cfg.esp_weapon_icon ) ) ? 1.f : 0.f );

    if ( m_parameters[ m_ent.uuid ].weapon.get< float >( ) < 0.01f )
        return;

    float original_y = m_ent.bounds.Max.y + 1 + ( 5 * m_parameters[ m_ent.uuid ].ammo_alpha.get< float >( ) );

    std::string const text = c_weapon_controller::get_by_id[ m_ent.weapon_id ].name;
    std::string const icon = c_weapon_controller::get_by_id[ m_ent.weapon_id ].icon;

    std::string grenades { };
    if ( m_ent.grenades.he )
        grenades += c_weapon_controller::get_by_id[ 91 ].icon;
    if ( m_ent.grenades.smoke )
        grenades += c_weapon_controller::get_by_id[ 92 ].icon;
    if ( m_ent.grenades.flash )
        grenades += c_weapon_controller::get_by_id[ 93 ].icon;

    std::string const distance_text = std::to_string( static_cast< int >( distance ) ) + crypt_string( "FT" );
    ImVec2 const distance_size = c_esp::m_fonts.pixel->CalcTextSizeA( 10, FLT_MAX, -1, distance_text.c_str( ) );
    ImVec2 const text_size = c_esp::m_fonts.pixel->CalcTextSizeA( 10, FLT_MAX, -1, text.c_str( ) );
    ImVec2 const icon_size = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, -1, icon.c_str( ) );
    ImVec2 const grenades_size = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, -1, grenades.c_str( ) );

    ImColor const color = c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_weapon_clr ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].weapon.get< float >( ) );

    m_parameters[ m_ent.uuid ].colors.weapon.update( !m_ent.untouchable ? color.Value : c::to_grayscale( color ).Value );

    if ( distance_enabled )
    {
        ImVec2 const position = { ( float ) int( m_ent.bounds.GetCenter( ).x - ( distance_size.x - 1 ) / 2 ), original_y };

        if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
            math::add_shadow_text( m_surface, c_esp::m_fonts.pixel, 10, position, ImColor( 1.f, 1.f, 1.f, color.Value.w ), ImColor( 1.f, 1.f, 1.f, color.Value.w ), distance_text );
        math::add_outlined_text( m_surface, c_esp::m_fonts.pixel, 10, position, ImColor( 1.f, 1.f, 1.f, color.Value.w ), ImColor( 0.f, 0.f, 0.f, 0.9f * color.Value.w ), distance_text.c_str( ) );

        original_y += distance_size.y - 1;
    }

    if ( text_enabled )
    {
        ImVec2 const position = { ( float ) int( m_ent.bounds.GetCenter( ).x - ( text_size.x - 1 ) / 2 ), original_y };

        if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
            math::add_shadow_text( m_surface, c_esp::m_fonts.pixel, 10, position, ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), text );
        math::add_outlined_text( m_surface, c_esp::m_fonts.pixel, 10, position, ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), ImColor( 0.f, 0.f, 0.f, 0.9f * color.Value.w ), text.c_str( ) );

        original_y += text_size.y - 1;
    }

    if ( icon_enabled )
    {
        ImVec2 const position = { m_ent.bounds.GetCenter( ).x - icon_size.x / 2, original_y };

        if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
            math::add_shadow_text( m_surface, c_esp::m_fonts.weapons, 14, position, ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), icon );
        m_surface->AddText( c_esp::m_fonts.weapons, 14, position + ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, 200 / 255.f * color.Value.w ), icon.c_str( ) );
        m_surface->AddText( c_esp::m_fonts.weapons, 14, position, ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), icon.c_str( ) );

        original_y += icon_size.y - 1;
    }

    if ( grenades_enabled && strlen( grenades.c_str( ) ) > 0 )
    {
        ImVec2 const position = { m_ent.bounds.GetCenter( ).x - grenades_size.x / 2, original_y };

        if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
            math::add_shadow_text( m_surface, c_esp::m_fonts.weapons, 14, position, ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), grenades );
        m_surface->AddText( c_esp::m_fonts.weapons, 14, position + ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, 200 / 255.f * color.Value.w ), grenades.c_str( ) );
        m_surface->AddText( c_esp::m_fonts.weapons, 14, position, ImColor( m_parameters[ m_ent.uuid ].colors.weapon.get< ImVec4 >( ) ), grenades.c_str( ) );

        original_y += icon_size.y - 1;
    }
}

void c_ent_player_esp::render_flags( std::vector< esp_flag_t >& flags ) const
{
    std::sort( flags.begin( ), flags.end( ), []( const esp_flag_t& a, const esp_flag_t& b ) {
        const float width_a = c_esp::m_fonts.pixel->CalcTextSizeA( 10, FLT_MAX, -1, a.value.c_str( ) ).x;
        const float width_b = c_esp::m_fonts.pixel->CalcTextSizeA( 10, FLT_MAX, -1, b.value.c_str( ) ).x;

        return width_a > width_b;
    } );

    std::for_each( flags.begin( ), flags.end( ), [ & ]( const esp_flag_t& flag ) {
        const int it = &flag - &flags[ 0 ];

        ImColor const color = !m_ent.untouchable ? flag.color : c::to_grayscale( flag.color );

        if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
            math::add_shadow_text( m_surface, c_esp::m_fonts.pixel, 10, { m_ent.bounds.Max.x + 3, m_ent.bounds.Min.y - 2 + ( 8 * it ) }, c::alpha_interpolate( color, m_parameters[ m_ent.uuid ].alpha.get< float >( ) ), c::alpha_interpolate( color, m_parameters[ m_ent.uuid ].alpha.get< float >( ) ), flag.value );
        math::add_outlined_text( m_surface, c_esp::m_fonts.pixel, 10, { m_ent.bounds.Max.x + 3, m_ent.bounds.Min.y - 2 + ( 8 * it ) }, c::alpha_interpolate( color, m_parameters[ m_ent.uuid ].alpha.get< float >( ) ), ImColor( 0.f, 0.f, 0.f, 0.9f * color.Value.w * m_parameters[ m_ent.uuid ].alpha.get< float >( ) ), flag.value );
    } );
}

void c_ent_player_esp::render_nickname( ImTextureID pfp_texture, ImTextureID gray_pfp_texture ) const
{
    m_parameters[ m_ent.uuid ].nickname.update< float >( c::get< bool >( g_ctx->cfg.esp_nickname ) ? 1.f : 0.f );

    if ( m_parameters[ m_ent.uuid ].nickname.get< float >( ) < 0.01f )
        return;

    std::string const nickname = m_ent.nickname;

    auto const font_pair = c_esp::m_nickname_fonts[ c::get< int >( g_ctx->cfg.esp_nickname_font ) ];
    if ( !font_pair.first )
        return;

    ImVec2 const nickname_size = font_pair.first->CalcTextSizeA( font_pair.second, FLT_MAX, -1, nickname.c_str( ) );

    ImVec2 avatar_size = {
        font_pair.second,
        font_pair.second };

    int const spacing = 3;

    float const full_width = static_cast< int >( nickname_size.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ) );

    m_parameters[ m_ent.uuid ].colors.nickname.update( !m_ent.untouchable ? c::get< ImVec4 >( g_ctx->cfg.esp_nickname_clr ) : c::to_grayscale( c::get< ImVec4 >( g_ctx->cfg.esp_nickname_clr ) ).Value );
    m_parameters[ m_ent.uuid ].colors.nickname_end.update( !m_ent.untouchable ? c::get< ImVec4 >( g_ctx->cfg.esp_nickname_clr_end ) : c::to_grayscale( c::get< ImVec4 >( g_ctx->cfg.esp_nickname_clr_end ) ).Value );
    m_parameters[ m_ent.uuid ].pfp_grayscale_factor.update( m_ent.untouchable ? 1.0f : 0.f );

    ImVec2 const base = ImVec2( m_ent.bounds.GetCenter( ).x - full_width / 2, m_ent.bounds.Min.y - nickname_size.y - ( ( pfp_texture && gray_pfp_texture ) ? 3 : 2 ) );

    if ( c::get< bool >( g_ctx->cfg.esp_shadows ) )
    {
        [ & ]( ) -> void {
            int const vtx_start = m_surface->VtxBuffer.Size;
            math::add_shadow_text( m_surface, font_pair.first, font_pair.second, ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ), c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), nickname );
            int const vtx_end = m_surface->VtxBuffer.Size;

            ImGui::ShadeVertsLinearColorGradientKeepAlpha( m_surface, vtx_start, vtx_end, ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ), ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ) + nickname_size, c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname_end.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ) );
        }( );
    }

    m_surface->AddText( font_pair.first, font_pair.second, ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ) + ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, 200 / 255.f * m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ).w * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), nickname.c_str( ) );

    [ & ]( ) -> void {
        int const vtx_start = m_surface->VtxBuffer.Size;
        m_surface->AddText( font_pair.first, font_pair.second, ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ), c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), nickname.c_str( ) );
        int const vtx_end = m_surface->VtxBuffer.Size;

        ImGui::ShadeVertsLinearColorGradientKeepAlpha( m_surface, vtx_start, vtx_end, ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ), ImVec2( base.x + ( ( pfp_texture && gray_pfp_texture ) ? ( avatar_size.x + spacing ) : 0 ), base.y ) + nickname_size, c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname_end.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ) );
    }( );

    if ( !pfp_texture || !gray_pfp_texture )
        return;

    m_surface->AddImageRounded( pfp_texture, base + ImVec2( 1, 1 ), base + avatar_size + ImVec2( 1, 1 ), { 0, 0 }, { 1, 1 }, ImColor( 0, 0, 0, static_cast< int >( 100 * m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ).w * m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ) ), 1 );

    float const grayscale_t = m_parameters[ m_ent.uuid ].pfp_grayscale_factor.get< float >( );

    m_surface->AddImageRounded( gray_pfp_texture, base, base + avatar_size, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), grayscale_t ), 1 );
    m_surface->AddImageRounded( pfp_texture, base, base + avatar_size, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( c::alpha_interpolate( m_parameters[ m_ent.uuid ].colors.nickname.get< ImVec4 >( ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) * m_parameters[ m_ent.uuid ].nickname.get< float >( ) ), 1.0f - grayscale_t ), 1 );
}

void c_ent_player_esp::render_selected_target( const vec3_t& target, int radius ) const
{
    auto const screen = math::world_to_screen( target );
    if ( !screen.check )
        return;

    math::add_radial_gradient( m_surface, screen.position, radius, c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_target_clr ), m_parameters[ m_ent.uuid ].alpha.get< float >( ) ), c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.esp_target_clr ), 0.f ) );
}