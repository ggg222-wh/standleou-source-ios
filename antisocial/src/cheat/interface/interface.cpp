#include "interface.hpp"

#include "globals.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void c_interface::init( float scale )
{
    forms::main.init( scale );

    if ( m_initializer )
        m_initializer( );
}

void c_interface::render( )
{
    ImGui::GetBackgroundDrawList( )->Flags &= ~( ImDrawListFlags_AntiAliasedFill | ImDrawListFlags_AntiAliasedLines );

    if ( g_ctx->is_presented )
        forms::main.render( );

    for ( auto const& func : m_queue )
    {
        func( );
    }
}

void c_interface::show_loading_screen( )
{
    ImGui::GetForegroundDrawList( )->AddRectFilled( { 0, 0 }, ImGui::GetIO( ).DisplaySize, ImColor( 0, 0, 0, 100 ) );

    []( ) -> void {
        ImDrawList* dl = ImGui::GetForegroundDrawList( );
        ImVec2 const center = ImGui::GetIO( ).DisplaySize * 0.5f;

        constexpr float radius = 16.0f;
        constexpr float thickness = 3.0f;
        constexpr int segments = 64;
        constexpr float speed = 2.0f;

        float const time = static_cast< float >( ImGui::GetTime( ) );
        float const start = fmodf( time * speed * IM_PI * 2.0f, IM_PI * 2.0f );
        float const angle_len = IM_PI * 1.3f;

        auto const col_head = ImColor( 255, 255, 255, 255 );
        auto const col_tail = ImColor( 150, 150, 150, 180 );

        dl->PathClear( );
        dl->PathArcTo( center, radius, start, start + angle_len, segments );

        dl->PathStroke( col_tail, false, thickness );

        dl->PathClear( );
        dl->PathArcTo( center, radius, start + angle_len * 0.7f, start + angle_len, segments / 4 );
        dl->PathStroke( col_head, false, thickness );
    }( );
}