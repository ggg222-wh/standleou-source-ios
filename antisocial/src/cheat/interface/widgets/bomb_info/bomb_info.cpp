//
// Created by Kai Tears on 16/08/2025.
//

#include "bomb_info.hpp"

#include "game/features/visual/esp/esp.hpp"
#include "game/features/visual/esp/planted_bomb/planted_bomb.hpp"

using namespace ImGui;

void circle_progress( ImDrawList* draw_list, float progress, float max, float radius, const ImVec2& pos, const ImColor& col, const float& thickness )
{
    const float& position = progress / max * 6.28f;

    const auto& center = pos;

    draw_list->PathClear( );
    draw_list->PathArcTo( center, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 120.f );
    draw_list->PathStroke( col, 0, thickness );

    ImVec2 start_point = ImVec2(
        center.x + radius * cosf( IM_PI * 1.5f ),
        center.y + radius * sinf( IM_PI * 1.5f ) );

    ImVec2 end_point = ImVec2(
        center.x + radius * cosf( IM_PI * 1.5f + position ),
        center.y + radius * sinf( IM_PI * 1.5f + position ) );

    float cap_radius = thickness / 2.0f;
    draw_list->AddCircleFilled( start_point, cap_radius, col );
    draw_list->AddCircleFilled( end_point, cap_radius, col );
}

void c_wg_bomb_info::render( )
{
    struct _state
    {
        c_animator alpha { float( 0.f ) };

        c_animator window_width { float( 0.f ) };
    };

    // TODO: move this globally
    static std::unordered_map< ImGuiID, _state > __state;
    _state& state = __state[ GetID( "Modules" ) ];

    float target_alpha = 0.0f;

    if ( *m_active )
    {
        if ( c_ent_planted_bomb_esp::planted || c_widgets::m_allowed_edit )
            target_alpha = 1.0f;
        else
            target_alpha = 0.0f;
    } else
    {
        target_alpha = c_widgets::m_allowed_edit ? 0.5f : 0.0f;
    }

    state.alpha.update( target_alpha );

    // state.alpha.update( ( *m_active ) ? 1 : ( ( c_widgets::m_allowed_edit ) ? 0.5f : 0.0f ) );

    if ( state.alpha.get< float >( ) > 0.01f )
    {
        SetNextWindowPos( m_pos, ImGuiCond_FirstUseEver );

        PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
        PushStyleVar( ImGuiStyleVar_Alpha, state.alpha.get< float >( ) );

        auto const wnd_flags = ( c_widgets::m_allowed_edit ) ? ( ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground ) : ( ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs );
        Begin( "Bomb", 0, wnd_flags );
        {
            // if ( m_push_pos )
            // {
            //     SetWindowPos( m_pos );
            //     m_push_pos = false;
            // }

            if ( c_widgets::m_allowed_edit )
                m_pos = GetWindowPos( );

            auto const generated_rect = generate_rect( GetWindowWidth( ), 32 );

            render_title( generated_rect, "Bomb" );
            render_activator( generated_rect.Min + ImVec2( 6, 6 ), "Bomb", m_active );

            PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );

            // shit ass code
            // SetCursorPos( { 0, 0 } );
            // ItemSize( { 0, generated_rect.GetSize( ).y - 2 } );

            SetCursorPos( { 0, generated_rect.GetSize( ).y + 3 } ); // 5 - 2

            auto const pos = GetCursorScreenPos( );
            float const width = GetWindowWidth( );

            ImRect const rect = {
                pos,
                pos + ImVec2( width, 60 ) };

            auto const time_left = c_ent_planted_bomb_esp::time_left;

            GetWindowDrawList( )->AddRectFilled( rect.Min + ImVec2( 5, 0 ), rect.Max - ImVec2( 5, 0 ), c::alpha_interpolate( ImColor( 10, 10, 10, 125 ), GetStyle( ).Alpha ), GetStyle( ).WindowRounding );
            GetWindowDrawList( )->AddRect( rect.Min + ImVec2( 5, 0 ), rect.Max - ImVec2( 5, 0 ), c::alpha_interpolate( ImColor( 255, 255, 255, 10 ), GetStyle( ).Alpha ), GetStyle( ).WindowRounding );

            circle_progress( GetWindowDrawList( ), static_cast< float >( time_left ), 40.0f, 15, { rect.Min.x + 35, rect.GetCenter( ).y }, c::alpha_interpolate( ImColor( 234, 193, 251 ), GetStyle( ).Alpha ), 4.f );

            auto const size = c_esp::m_fonts.weapons->CalcTextSizeA( 14.f, FLT_MAX, -1, "o" );
            GetWindowDrawList( )->AddText( c_esp::m_fonts.weapons, 14, ImVec2( rect.Min.x + 35, rect.GetCenter( ).y ) - size / 2, c::alpha_interpolate( ImColor( 255, 255, 255 ), GetStyle( ).Alpha ), "o" );

            std::string const seconds = "Seconds left: " + std::to_string( static_cast< int >( time_left ) );
            auto const size_sec = CalcTextSize( seconds.c_str( ) );
            GetWindowDrawList( )->AddText( ImVec2( rect.Min.x + 15 + 35 + 10, rect.GetCenter( ).y - size_sec.y / 2 ), c::alpha_interpolate( ImColor( 255, 255, 255 ), GetStyle( ).Alpha ), seconds.c_str( ) );

            ItemSize( { 0, rect.GetHeight( ) } );

            Dummy( { 180, 5 } );

            PopStyleVar( );

            End( );
        }

        PopStyleVar( 2 );
    }
}
