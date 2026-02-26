//
// Created by Kai Tears on 16/08/2025.
//

#include "widgets.hpp"

#include "bomb_info/bomb_info.hpp"
#include "modules/modules.hpp"
#include "alive_players/alive_players.hpp"

#include "globals.hpp"

using namespace ImGui;

bool c_widgets::m_allowed_edit = false;

c_wg_modules* wg_modules;
c_wg_bomb_info* wg_bomb_info;

void c_widgets::render( bool allow_edit ) const
{
    m_allowed_edit = allow_edit;

    if ( !wg_modules )
    {
        wg_modules = new c_wg_modules( c::get< ImVec2 >( g_ctx->cfg.widgets_pos_modules ), &c::get< bool >( g_ctx->cfg.widgets_active_modules ), { } );
        wg_modules->update_modules( );
    }
    wg_modules->render( );

    if ( !wg_bomb_info )
        wg_bomb_info = new c_wg_bomb_info( c::get< ImVec2 >( g_ctx->cfg.widgets_pos_bomb ), &c::get< bool >( g_ctx->cfg.widgets_active_bomb ) );
    wg_bomb_info->render( );

    // static c_wg_alive_players wg_alive_players( { 40, 240 }, &c::get<bool>(g_ctx->cfg.widgets_active_players), { { "Player 1", "1" }, { "Player 2", "2" } } );
    // wg_alive_players.render( );
}

void c_widgets::update( )
{
    // I forgot about this shit while I was initially coding widget base
    // so for now we'll stick with this shit ass code
    if ( wg_bomb_info )
    {
        *wg_bomb_info->m_active = c::get< bool >( g_ctx->cfg.widgets_active_bomb );
        wg_bomb_info->m_pos = c::get< ImVec2 >( g_ctx->cfg.widgets_pos_bomb );
        // wg_bomb_info->m_push_pos = true;
        ImGuiWindow* window = ImGui::FindWindowByName( "Bomb" );
        if ( window )
        {
            window->Pos = c::get< ImVec2 >( g_ctx->cfg.widgets_pos_bomb );
            window->DC.CursorPos = window->Pos;
        }
    }

    if ( wg_modules )
    {
        wg_modules->update_modules( );
        *wg_modules->m_active = c::get< bool >( g_ctx->cfg.widgets_active_modules );
        wg_modules->m_pos = c::get< ImVec2 >( g_ctx->cfg.widgets_pos_modules );
        // wg_modules->m_push_pos = true;
        ImGuiWindow* window = ImGui::FindWindowByName( "Modules" );
        if ( window )
        {
            window->Pos = c::get< ImVec2 >( g_ctx->cfg.widgets_pos_modules );
            window->DC.CursorPos = window->Pos;
        }
    }
}

void c_widgets::save( )
{
    if ( wg_bomb_info )
        c::get< ImVec2 >( g_ctx->cfg.widgets_pos_bomb ) = wg_bomb_info->m_pos;

    if ( wg_modules )
        c::get< ImVec2 >( g_ctx->cfg.widgets_pos_modules ) = wg_modules->m_pos;
}

void c_base_widget::render_title( const ImRect& rect, const std::string& title ) const
{
    ItemSize( rect.GetSize( ) );

    auto* const draw_list = GetWindowDrawList( );

    int const vert_start_idx = draw_list->VtxBuffer.Size;
    draw_list->AddRectFilled( rect.Min + ImVec2( 2, 2 ), rect.Max - ImVec2( 2, 2 ), c::alpha_interpolate( ImColor( 27, 27, 47 ), GetStyle( ).Alpha ), GetStyle( ).WindowRounding );
    int const vert_end_idx = draw_list->VtxBuffer.Size;

    ShadeVertsLinearColorGradientKeepAlpha( draw_list, vert_start_idx, vert_end_idx, rect.Min, rect.Max, ImColor( 16, 16, 28 ), ImColor( 27, 15, 61 ) );

    draw_list->AddRect( rect.Min + ImVec2( 2, 2 ), rect.Max - ImVec2( 2, 2 ), c::alpha_interpolate( GetStyleColorVec4( ImGuiCol_Border ), GetStyle( ).Alpha ), GetStyle( ).WindowRounding );

    auto const title_size = CalcTextSize( title.c_str( ) );
    draw_list->AddText( rect.GetCenter( ) - title_size / 2, c::alpha_interpolate( ImColor( 255, 255, 255 ), GetStyle( ).Alpha ), title.c_str( ) );
}

void c_base_widget::render_activator( const ImVec2& pos, const std::string& id, bool* active ) const
{
    struct _state
    {
        c_animator text { ImVec4( 0, 0, 0, 0 ) };
        c_animator alpha { float( 0.f ) };
    };

    static std::unordered_map< ImGuiID, _state > __state;
    _state& state = __state[ GetID( id.c_str( ) ) ];

    state.text.update< ImVec4 >( ( *active ) ? ImColor( 255, 255, 255 ).Value : ImColor( 155, 155, 155 ).Value );

    state.alpha.update< float >( c_widgets::m_allowed_edit ? 1 : 0 );

    const ImRect rect = {
        pos - ImVec2( 12, 12 ),
        pos + ImVec2( 12, 12 ) };

    GetForegroundDrawList( )->AddCircleFilled( pos, 12, c::alpha_interpolate( ImColor( 29, 29, 49 ), GetStyle( ).Alpha * state.alpha.get< float >( ) ), 20 );
    GetForegroundDrawList( )->AddCircle( pos, 12, c::alpha_interpolate( GetStyleColorVec4( ImGuiCol_Border ), GetStyle( ).Alpha * state.alpha.get< float >( ) ), 20 );

    const char* eye = "\xef\x81\xae";
    auto const eye_size = CalcTextSize( eye );
    GetForegroundDrawList( )->AddText( rect.GetCenter( ) - eye_size / 2 + ImVec2( 1, -1 ), c::alpha_interpolate( state.text.get< ImVec4 >( ), GetStyle( ).Alpha * state.alpha.get< float >( ) ), eye );

    SetCursorPos( { 0, 0 } );
    if ( InvisibleButton( id.c_str( ), { 20, 20 } ) )
        *active = !( *active );
}

ImRect c_base_widget::generate_rect( const float& width, const float& height ) const
{
    auto* const current_window = GetCurrentWindow( );

    return { ImVec2( current_window->Pos.x, current_window->Pos.y ), ImVec2( current_window->Pos.x + width, current_window->Pos.y + height ) };
}