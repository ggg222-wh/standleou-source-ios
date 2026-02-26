#include "dropped_weapons.hpp"

#include <string>

#include "game/features/visual/esp/esp.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"

#include "globals.hpp"

texture_data_t c_sample_dropped_weapons::m_texture { };

void c_sample_dropped_weapons::render( ImDrawList* draw_list, const ImVec2& center )
{
    if ( !c::get< bool >( g_ctx->cfg.visual_dropped_weapons ) )
        return;

    std::string const icon = crypt_string( "Z" );
    auto const size = c_esp::m_fonts.weapons->CalcTextSizeA( 14, FLT_MAX, -1, icon.c_str( ) );

    int const clip = 5;
    int const max_clip = 8;

    draw_list->AddText( c_esp::m_fonts.weapons, 14, center - size / 2 + ImVec2( 1, 1 ), c::alpha_interpolate( ImColor( 0.f, 0.f, 0.f, ( 200 / 255.f ) * c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ).w ), ImGui::GetStyle( ).Alpha ), icon.c_str( ) );
    draw_list->AddText( c_esp::m_fonts.weapons, 14, center - size / 2, c::alpha_interpolate( ImColor( c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ) ), ImGui::GetStyle( ).Alpha ), icon.c_str( ) );

    if ( !c::get< bool >( g_ctx->cfg.visual_dropped_weapons_ammo ) )
        return;

    int const width = size.x + 6;

    ImRect const rect = {
        { static_cast< float >( int( center.x - static_cast< float >( width / 2 ) ) ), center.y + size.y / 2 + 2 },
        { static_cast< float >( int( center.x + static_cast< float >( width / 2 ) ) ), center.y + size.y / 2 + 2 + 2 } };

    ImDrawListSplitter splitter;
    splitter.Split( draw_list, 2 );

    splitter.SetCurrentChannel( draw_list, 1 );
    auto old_flags = draw_list->Flags;
    draw_list->Flags &= ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;

    float const alpha = c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ).w;
    float const progress = static_cast< float >( std::min( clip, max_clip ) ) / static_cast< float >( max_clip );

    draw_list->AddRectFilled( rect.Min, rect.Max, ImColor( 0.f, 0.f, 0.f, 165 / 255.f * alpha ) );
    draw_list->AddRectFilled( rect.Min, { rect.Min.x + rect.GetWidth( ) * progress, rect.Max.y }, c::alpha_interpolate( c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_ammo_color ), alpha ) );

    draw_list->AddRect( rect.Min - ImVec2( 1, 1 ), { rect.Min.x + rect.GetWidth( ) + 1, rect.Max.y + 1 }, ImColor( 0.f, 0.f, 0.f, 200 / 255.f * alpha ) );

    draw_list->Flags = old_flags;
    splitter.Merge( draw_list );
}
