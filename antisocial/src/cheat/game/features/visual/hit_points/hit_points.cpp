#include "hit_points.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/misc/math/math.hpp"

void c_hit_points::clear( )
{
    m_points.clear( );
}

void c_hit_points::push_point( const hit_point_t& tracer )
{
    m_points.push_back( tracer );
}

void c_hit_points::render( )
{
    if ( m_points.size( ) > 100 )
        m_points.erase( m_points.begin( ) );

    for ( int it { 0 }; it < m_points.size( ); it++ )
    {
        hit_point_t& point = m_points[ it ];

        point.alpha_modulate--;

        if ( point.alpha_modulate < 1 )
        {
            m_points.erase( m_points.begin( ) + it );
            continue;
        }

        ImColor const interpolated = ImColor( 80 / 255.f, 155 / 255.f, 1.f, point.alpha_modulate / 255.f );

        w2s_t const point_screen = math::world_to_screen( math::calculate_world_position( point.point ) );

        if ( !point_screen.check )
            return;

        ImGui::GetBackgroundDrawList( )->AddRectFilled( point_screen.position - ImVec2( 1, 1 ), point_screen.position + ImVec2( 1, 1 ), interpolated );
    }
}