#include "bomb.hpp"

#include <string>

#include "game/features/visual/esp/esp.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"

#include "game/sdk/misc/math/math.hpp"

#include "globals.hpp"

texture_data_t c_sample_bomb::m_texture { };

void c_sample_bomb::render( ImDrawList* draw_list, const ImVec2& center )
{
    if ( !c::get< bool >( g_ctx->cfg.visual_planted_bomb ) )
        return;

    const float& radius = 8 + 14;

    draw_list->AddShadowCircle( center, radius, c::alpha_interpolate( ImColor( 8, 8, 8, 120 ), ImGui::GetStyle( ).Alpha ), static_cast< int >( radius * 3 ), { 0, 0 }, 0, 400 );

    draw_list->AddCircleFilled( center, radius, c::alpha_interpolate( ImColor( 8, 8, 8, 80 ), ImGui::GetStyle( ).Alpha ), 400 );

    draw_list->AddShadowCircle( center, 1, c::alpha_interpolate( ImColor( 255, 99, 99 ), ImGui::GetStyle( ).Alpha ), static_cast< int >( radius * 3.2f ), { 0, 0 }, 0, 400 );

    math::circle_progress( draw_list, 25.f, 40.f, radius - 4, center, c::alpha_interpolate( ImColor( 255, 99, 99 ), ImGui::GetStyle( ).Alpha ), 4.f );

    const ImVec2& text_sz = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, 0, ay_obfuscate( "o" ) );
    draw_list->AddText( c_esp::m_fonts.weapons, 14, { center.x - text_sz.x / 2 + 1, center.y - text_sz.y / 2 + 1 }, c::alpha_interpolate( ImColor( 0, 0, 0 ), ImGui::GetStyle( ).Alpha ), ay_obfuscate( "o" ) );
    draw_list->AddText( c_esp::m_fonts.weapons, 14, { center.x - text_sz.x / 2, center.y - text_sz.y / 2 }, c::alpha_interpolate( ImColor( 255, 255, 255 ), ImGui::GetStyle( ).Alpha ), ay_obfuscate( "o" ) );
}
