//
// Created by rei on 3/10/2025.
//

#include "grenades.hpp"

#include "globals.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "game/sdk/misc/math/math.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/unity/transform/transform.hpp"

#include "../esp.hpp"

using namespace ImGui;

void c_ent_grenade_esp::render( )
{
    if ( !m_param.screen.check )
        return;

    auto grenade = m_param.ptr;
    if ( !grenade )
        return;

    if ( animation_data.find( grenade ) == animation_data.end( ) )
        return;

    auto const current_time = std::chrono::steady_clock::now( );
    auto const elapsed_time = std::chrono::duration_cast< std::chrono::duration< float > >( current_time - animation_data[ grenade ].start_time );
    float const time_left = std::max( animation_data[ grenade ].total_time - elapsed_time.count( ), 0.0f );

    int const grenade_state = *reinterpret_cast< int* >( grenade + 0x68 );

    if ( grenade_state == 1 || grenade_state == 3 || time_left < 0.25f )
    {
        animation_data[ grenade ].alpha.update< float >( 0.f );

        if ( animation_data[ grenade ].alpha.get< float >( ) < 0.01f || grenade_state == 3 )
            return;
    } else
        animation_data[ grenade ].alpha.update< float >( 1.f );

    animation_data[ grenade ].trail.push_back( m_param.world );

    size_t const max_trail_length = 120;
    if ( animation_data[ grenade ].trail.size( ) > max_trail_length )
    {
        animation_data[ grenade ].trail.erase( animation_data[ grenade ].trail.begin( ) );
    }

    float const alpha = animation_data[ grenade ].alpha.get< float >( );

    auto get_grenade_color = [ & ]( int idx ) -> ImColor {
        switch ( idx )
        {
        case 91:
            return { 255, 25, 25, static_cast< int >( 255 * alpha ) };
        case 92:
            return { 0, 240, 20, static_cast< int >( alpha * 255 ) };
        case 93:
            return { 255, 255, 255, static_cast< int >( 255 * alpha ) };
        case 94:
            return { 255, 155, 0, static_cast< int >( 255 * alpha ) };
        case 95:
            return { 255, 155, 0, static_cast< int >( 255 * alpha ) };
        case 97:
            return { 255, 155, 0, static_cast< int >( 255 * alpha ) };
        default:
            return { 255, 255, 255, static_cast< int >( 255 * alpha ) };
        }
    };

    // trail
    if ( animation_data[ grenade ].trail.size( ) > 1 )
    {
        for ( size_t i = 1; i < animation_data[ grenade ].trail.size( ); ++i )
        {
            const vec3_t& prev_pos = animation_data[ grenade ].trail[ i - 1 ];
            const vec3_t& curr_pos = animation_data[ grenade ].trail[ i ];

            auto const prev_screen = math::world_to_screen( prev_pos );
            auto const curr_screen = math::world_to_screen( curr_pos );

            if ( prev_screen.check == 0 && curr_screen.check == 0 )
                GetBackgroundDrawList( )->AddLine( prev_screen.position, curr_screen.position, get_grenade_color( m_param.idx ), 2 );
        }
    }

    {
        // main rendering

        const float& radius = 8 + 14;

        GetBackgroundDrawList( )->AddShadowCircle( m_param.screen.position, radius, ImColor( 8, 8, 8, static_cast< int >( 120 * alpha ) ), static_cast< int >( radius * 3 ), { 0, 0 }, 0, 400 );

        GetBackgroundDrawList( )->AddCircleFilled( m_param.screen.position, radius, ImColor( 8, 8, 8, static_cast< int >( 80 * alpha ) ), 200 );

        GetBackgroundDrawList( )->AddShadowCircle( m_param.screen.position, 1, get_grenade_color( m_param.idx ), static_cast< int >( radius * 3.2f ), { 0, 0 }, 0, 400 );

        math::circle_progress( GetBackgroundDrawList( ), time_left, animation_data[ grenade ].total_time, radius - 4, m_param.screen.position, c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.visual_grenades_esp_color ), alpha ), 4.f );

        const ImVec2& text_sz = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, 0, c_weapon_controller::get_by_id[ m_param.idx ].icon.c_str( ) );
        GetBackgroundDrawList( )->AddText( c_esp::m_fonts.weapons, 14, { m_param.screen.position.x - text_sz.x / 2 + 1, m_param.screen.position.y - text_sz.y / 2 + 1 }, ImColor( 0, 0, 0, static_cast< int >( 255 * alpha ) ), c_weapon_controller::get_by_id[ m_param.idx ].icon.c_str( ) );
        GetBackgroundDrawList( )->AddText( c_esp::m_fonts.weapons, 14, { m_param.screen.position.x - text_sz.x / 2, m_param.screen.position.y - text_sz.y / 2 }, ImColor( 255, 255, 255, static_cast< int >( 255 * alpha ) ), c_weapon_controller::get_by_id[ m_param.idx ].icon.c_str( ) );
    }
}

void c_ent_grenade_esp::execute( uintptr_t grenade )
{
    if ( !grenade )
        return;

    c_transform* const transform = *reinterpret_cast< c_transform** >( grenade + 0x38 );
    if ( !transform )
        return;

    uintptr_t const weapon_material_manager = *reinterpret_cast< uintptr_t* >( grenade + 0x60 );
    if ( !weapon_material_manager )
        return;

    vec3_t const position = transform->get_position( );

    auto const screen = math::world_to_screen( position );

    c_ent_grenade_esp esp( { grenade, position, screen, *reinterpret_cast< int* >( weapon_material_manager + 0x18 ) } );
    esp.render( );
}

std::unordered_map< uintptr_t, grenade_animation_t > c_ent_grenade_esp::animation_data;