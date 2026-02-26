//
// Created by Kai Tears on 16/08/2025.
//

#include "modules.hpp"

#include "game/features/ragebot/ragebot.hpp"
#include "game/features/misc/peek_assist/peek_assist.hpp"

#include "globals.hpp"

using namespace ImGui;

void c_wg_modules::update_modules( )
{
    m_captured_modules.clear( );

    auto const get_manuals_direction = [ & ]( ) -> std::string {
        switch ( g_ctx->features.ragebot->m_manual_selected )
        {
        case antiaim_t::left:
            return "Manual AA: Left";
        case antiaim_t::right:
            return "Manual AA: Right";
        case antiaim_t::none:
            return "Manual AA: None";
        default:
            return "Manual AA: -";
        }
    };

    m_captured_modules = {
        { &c::get< bool >( g_ctx->cfg.misc_thirdperson ), { "Third person", "third_person" } },
        { reinterpret_cast< bool* >( &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).manuals ), { get_manuals_direction( ), "manuals" } },
        { &g_ctx->features.peek_assist->m_peeking, { "Peeking", "peeker" } } };
}

void c_wg_modules::render( )
{
    struct _state
    {
        c_animator alpha { float( 0.f ) };

        c_animator window_width { float( 0.f ) };
    };

    // TODO: move this globally
    static std::unordered_map< ImGuiID, _state > __state;
    _state& state = __state[ GetID( "Modules" ) ];

    state.alpha.update( ( *m_active ) ? 1 : ( ( c_widgets::m_allowed_edit ) ? 0.5f : 0.0f ) );

    if ( state.alpha.get< float >( ) > 0.01f )
    {
        SetNextWindowPos( m_pos, ImGuiCond_FirstUseEver );

        auto const get_longest_module = [ this ]( ) -> float {
            float longest = 0.0f;

            for ( const auto& mod : m_captured_modules )
            {
                float w = CalcTextSize( mod.second.first.c_str( ) ).x;
                if ( w > longest && ( *mod.first ) )
                    longest = w;
            }

            float result = longest + 30.0f;

            return ( result < 100.0f ) ? 100.0f : result;
        };

        state.window_width.update< float >( get_longest_module( ) );

        SetNextWindowSize( { state.window_width.get< float >( ), 0 } );

        PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
        PushStyleVar( ImGuiStyleVar_Alpha, state.alpha.get< float >( ) );

        auto const get_manuals_direction = [ & ]( ) -> std::string {
            switch ( g_ctx->features.ragebot->m_manual_selected )
            {
            case antiaim_t::left:
                return "Manual AA: Left";
            case antiaim_t::right:
                return "Manual AA: Right";
            case antiaim_t::none:
                return "Manual AA: None";
            default:
                return "Manual AA: -";
            }
        };

        m_captured_modules[ 1 ].second.first = get_manuals_direction( );

        auto const wnd_flags = ( c_widgets::m_allowed_edit ) ? ( ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground ) : ( ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs );
        Begin( "Modules", 0, wnd_flags );
        {
            // if ( m_push_pos )
            // {
            //     SetWindowPos( m_pos );
            //     m_push_pos = false;
            // }

            if ( c_widgets::m_allowed_edit )
                m_pos = GetWindowPos( );

            auto const generated_rect = generate_rect( GetWindowWidth( ), 32 );

            render_title( generated_rect, "Modules" );
            render_activator( generated_rect.Min + ImVec2( 6, 6 ), "Modules", m_active );

            PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );

            // shit ass code
            // SetCursorPos( { 0, 0 } );
            // ItemSize( { 0, generated_rect.GetSize( ).y - 2 } );

            SetCursorPos( { 0, generated_rect.GetSize( ).y + 3 } ); // 5 - 2

            for ( auto& mod : m_captured_modules )
            {
                render_module( GetCursorScreenPos( ), GetWindowWidth( ), mod );
            }

            Dummy( { 0, 5 } );

            PopStyleVar( );

            End( );
        }

        PopStyleVar( 2 );
    }
}

void c_wg_modules::render_module( const ImVec2& pos, const float& width, const std::pair< bool*, std::pair< std::string, std::string > >& module )
{
    // yep, now this should be done locally
    struct _state
    {
        c_animator primary { float( 0.f ) };
    };

    static std::unordered_map< ImGuiID, _state > __state;
    _state& state = __state[ GetID( module.second.second.c_str( ) ) ];

    state.primary.update< float >( ( *module.first ) ? 1 : 0 );

    if ( state.primary.get< float >( ) < 0.01f )
        return;

    ImRect const rect = {
        pos,
        pos + ImVec2( width, 25 * state.primary.get< float >( ) ) };

    GetWindowDrawList( )->AddRectFilled( rect.Min + ImVec2( 5 * state.primary.get< float >( ), 0 ), rect.Max - ImVec2( 5, 0 ), c::alpha_interpolate( ImColor( 10, 10, 10, 125 ), GetStyle( ).Alpha * state.primary.get< float >( ) ), GetStyle( ).WindowRounding );
    GetWindowDrawList( )->AddRect( rect.Min + ImVec2( 5 * state.primary.get< float >( ), 0 ), rect.Max - ImVec2( 5, 0 ), c::alpha_interpolate( ImColor( 255, 255, 255, 10 ), GetStyle( ).Alpha * state.primary.get< float >( ) ), GetStyle( ).WindowRounding );

    auto const size = CalcTextSize( module.second.first.c_str( ) );
    GetWindowDrawList( )->AddText( rect.GetCenter( ) - size / 2, c::alpha_interpolate( ImColor( 255, 255, 255 ), GetStyle( ).Alpha * state.primary.get< float >( ) ), module.second.first.c_str( ) );

    ItemSize( { 0, 25 * state.primary.get< float >( ) + 5 } );
}