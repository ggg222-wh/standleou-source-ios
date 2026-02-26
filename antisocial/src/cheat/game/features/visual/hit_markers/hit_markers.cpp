#include "hit_markers.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/misc/math/math.hpp"

#include "game/features/visual/esp/esp.hpp"

#include "globals.hpp"

void c_hit_markers::clear( )
{
    m_markers.clear( );
}

void c_hit_markers::push_marker( const hit_marker_t& tracer )
{
    m_markers.push_back( tracer );
}

void c_hit_markers::render( )
{
    if ( m_markers.size( ) > 100 )
        m_markers.erase( m_markers.begin( ) );

    for ( int it { 0 }; it < m_markers.size( ); it++ )
    {
        hit_marker_t& marker = m_markers[ it ];

        marker.alpha_modulate--;

        if ( marker.alpha_modulate < 1 )
        {
            m_markers.erase( m_markers.begin( ) + it );
            continue;
        }

        ImColor const interpolated = ImColor( 1.f, 1.f, 1.f, marker.alpha_modulate / 255.f );

        w2s_t const screen = math::world_to_screen( math::calculate_world_position( marker.position ) );
        int const damage = marker.damage;

        if ( !screen.check )
            return;

        auto* const damage_str = std::to_string( damage ).c_str( );

        auto const text_size = c_esp::m_fonts.verdana->CalcTextSizeA( 10, FLT_MAX, -1, damage_str );

        ImGui::GetBackgroundDrawList( )->AddText( c_esp::m_fonts.verdana, 10, screen.position - text_size / 2 - ImVec2( 0, 16 ) + ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, 0.9f * interpolated.Value.w ), damage_str );
        ImGui::GetBackgroundDrawList( )->AddText( c_esp::m_fonts.verdana, 10, screen.position - text_size / 2 - ImVec2( 0, 16 ), interpolated, damage_str );
    }
}