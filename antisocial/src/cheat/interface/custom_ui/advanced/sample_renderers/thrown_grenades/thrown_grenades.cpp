#include "thrown_grenades.hpp"

#include <string>

#include "game/features/visual/esp/esp.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"

#include "game/sdk/misc/math/math.hpp"

#include "globals.hpp"

texture_data_t c_sample_thrown_grenades::m_texture { };

void c_sample_thrown_grenades::render( ImDrawList* draw_list, const ImVec2& center )
{
    if ( !c::get< bool >( g_ctx->cfg.visual_grenades_esp ) )
        return;

    const float radius = 8 + 14;

    float const alpha = ImGui::GetStyle( ).Alpha;

    draw_list->AddShadowCircle( center, radius, ImColor( 8, 8, 8, static_cast< int >( 120 * alpha ) ), static_cast< int >( radius * 3 ), { 0, 0 }, 0, 400 );

    draw_list->AddCircleFilled( center, radius, ImColor( 8, 8, 8, static_cast< int >( 80 * alpha ) ), 200 );

    draw_list->AddShadowCircle( center, 1, ImColor( 255, 25, 25, static_cast< int >( 255 * alpha ) ), static_cast< int >( radius * 3.2f ), { 0, 0 }, 0, 400 );

    math::circle_progress( draw_list, 0.3f, 1.0f, radius - 4, center, c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.visual_grenades_esp_color ), alpha ), 4.f );

    const ImVec2& text_sz = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, 0, ay_obfuscate( "j" ) );
    draw_list->AddText( c_esp::m_fonts.weapons, 14, { center.x - text_sz.x / 2 + 1, center.y - text_sz.y / 2 + 1 }, ImColor( 0, 0, 0, static_cast< int >( 255 * alpha ) ), ay_obfuscate( "j" ) );
    draw_list->AddText( c_esp::m_fonts.weapons, 14, { center.x - text_sz.x / 2, center.y - text_sz.y / 2 }, ImColor( 255, 255, 255, static_cast< int >( 255 * alpha ) ), ay_obfuscate( "j" ) );
}
