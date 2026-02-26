//
// Created by rei on 3/13/2025.
//

#include "planted_bomb.hpp"

#include "globals.hpp"

#include "game/sdk/misc/math/math.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"

#include "game/features/visual/esp/esp.hpp"

using namespace ImGui;

std::unordered_map< uintptr_t, bomb_animation_t > c_ent_planted_bomb_esp::animation_data;
bool c_ent_planted_bomb_esp::planted;
double c_ent_planted_bomb_esp::time_left;

double c_ent_planted_bomb_esp::get_time_to_detonation( double plant_time ) const noexcept
{
    auto current_time = c_ent_planted_bomb_esp::get_current_time( );
    double explosion_time = plant_time + 40.0;
    double time_left = explosion_time - current_time;

    if ( time_left < 0 )
    {
        return 0;
    }

    return time_left;
}

void c_ent_planted_bomb_esp::render( ) const noexcept
{
    if ( !m_param.screen.check || !m_param.ptr )
        return;

    const float& time_left = static_cast< float >( get_time_to_detonation( animation_data[ m_param.ptr ].plant_time ) );

    if ( time_left < 1 )
        return;

    {
        const float& radius = 8 + 14;

        GetBackgroundDrawList( )->AddShadowCircle( m_param.screen.position, radius, ImColor( 8, 8, 8, 120 ), static_cast< int >( radius * 3 ), { 0, 0 }, 0, 400 );

        GetBackgroundDrawList( )->AddCircleFilled( m_param.screen.position, radius, ImColor( 8, 8, 8, 80 ), 400 );

        GetBackgroundDrawList( )->AddShadowCircle( m_param.screen.position, 1, ImColor( 255, 99, 99 ), static_cast< int >( radius * 3.2f ), { 0, 0 }, 0, 400 );

        math::circle_progress( GetBackgroundDrawList( ), time_left, 40.f, radius - 4, m_param.screen.position, ImColor( 255, 99, 99 ), 4.f );

        const ImVec2& text_sz = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, 0, c_weapon_controller::get_by_id[ 100 ].icon.c_str( ) );
        GetBackgroundDrawList( )->AddText( c_esp::m_fonts.weapons, 14, { m_param.screen.position.x - text_sz.x / 2 + 1, m_param.screen.position.y - text_sz.y / 2 + 1 }, ImColor( 0, 0, 0 ), c_weapon_controller::get_by_id[ 100 ].icon.c_str( ) );
        GetBackgroundDrawList( )->AddText( c_esp::m_fonts.weapons, 14, { m_param.screen.position.x - text_sz.x / 2, m_param.screen.position.y - text_sz.y / 2 }, ImColor( 255, 255, 255 ), c_weapon_controller::get_by_id[ 100 ].icon.c_str( ) );
    }
}

double c_ent_planted_bomb_esp::get_current_time( ) noexcept
{
    auto now = std::chrono::high_resolution_clock::now( );
    auto duration = std::chrono::duration< double >( now.time_since_epoch( ) );
    return duration.count( );
}
