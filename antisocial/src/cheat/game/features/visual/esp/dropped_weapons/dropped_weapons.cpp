#include "dropped_weapons.hpp"

#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/structures/vec3.hpp"
#include "game/sdk/misc/math/math.hpp"

#include "../esp.hpp"

void c_dropped_weapons_esp::execute( uintptr_t weapon )
{
    if ( !weapon )
        return;

    c_transform* const transform = *reinterpret_cast< c_transform** >( weapon + 0x78 );
    if ( !transform )
        return;

    uintptr_t const data = *reinterpret_cast< uintptr_t* >( weapon + 0x88 );
    if ( !data )
        return;

    int const weapon_idx = *reinterpret_cast< int* >( weapon + 0x68 );

    int const clip = static_cast< int >( *reinterpret_cast< short* >( data + 0x50 ) );
    int const max_clip = c_weapon_controller::get_by_id[ weapon_idx ].max_clip;

    vec3_t const position = transform->get_position( );

    auto const screen = math::world_to_screen( position );

    std::string const icon = c_weapon_controller::get_by_id[ weapon_idx ].icon;
    auto const size = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, -1, icon.c_str( ) );

    if ( !screen.check )
        return;

    ImGui::GetBackgroundDrawList( )->AddText( c_esp::m_fonts.weapons, 14, screen.position - size / 2 + ImVec2( 1, 1 ), ImColor( 0.f, 0.f, 0.f, ( 200 / 255.f ) * c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ).w ), icon.c_str( ) );
    ImGui::GetBackgroundDrawList( )->AddText( c_esp::m_fonts.weapons, 14, screen.position - size / 2, ImColor( c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ) ), icon.c_str( ) );

    if ( !c::get< bool >( g_ctx->cfg.visual_dropped_weapons_ammo ) )
        return;

    if ( !c_weapon_controller::is_gun( weapon_idx ) )
        return;

    int const width = size.x + 6;

    ImRect const rect = {
        { static_cast< float >( int( screen.position.x - static_cast< float >( width / 2 ) ) ), screen.position.y + size.y / 2 + 2 },
        { static_cast< float >( int( screen.position.x + static_cast< float >( width / 2 ) ) ), screen.position.y + size.y / 2 + 2 + 2 } };

    float const alpha = c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ).w;
    float const progress = static_cast< float >( std::min( clip, max_clip ) ) / static_cast< float >( max_clip );

    ImGui::GetBackgroundDrawList( )->AddRectFilled( rect.Min, rect.Max, ImColor( 0.f, 0.f, 0.f, 165 / 255.f * alpha ) );
    ImGui::GetBackgroundDrawList( )->AddRectFilled( rect.Min, { rect.Min.x + rect.GetWidth( ) * progress, rect.Max.y }, c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_ammo_color ), alpha ) );

    ImGui::GetBackgroundDrawList( )->AddRect( rect.Min - ImVec2( 1, 1 ), { rect.Min.x + rect.GetWidth( ) + 1, rect.Max.y + 1 }, ImColor( 0.f, 0.f, 0.f, 200 / 255.f * alpha ) );
}