#include "removals.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"

#include "interface/custom_ui/advanced/utilities.hpp"

#include "game/entities/entities.hpp"

#include "globals.hpp"

void c_removals::draw_sniper_scope( ) const
{
    if ( !c::get< bool >( g_ctx->cfg.removals_scope_overlay ) )
        return;

    auto* const local = c_players_database::get_instance( )->m_local_player;
    if ( !local || !local->alive( ) )
        return;

    auto* const weapon = local->get_weapon( );
    if ( !weapon )
        return;

    bool const is_scoped = weapon->scoped( );

    // if ( !is_scoped )
    //     return;

    auto draw = ImGui::GetBackgroundDrawList( );
    // draw->AddLine( { ImGui::GetIO( ).DisplaySize.x / 2, 0 }, { ImGui::GetIO( ).DisplaySize.x / 2, ImGui::GetIO( ).DisplaySize.y }, ImColor( 0, 0, 0 ) );
    // draw->AddLine( { 0, ImGui::GetIO( ).DisplaySize.y / 2 }, { ImGui::GetIO( ).DisplaySize.x, ImGui::GetIO( ).DisplaySize.y / 2 }, ImColor( 0, 0, 0 ) );\

    ImVec2 const center = ImGui::GetIO( ).DisplaySize / 2;

    static c_animator state { float( 0.f ) };

    state.update< float >( is_scoped ? 1.0f : 0.f );

    if ( state.get< float >( ) < 0.01f )
        return;

    float const offset = 35.0f;
    float const length = 80.0f * state.get< float >( );

    ImColor const col_start = ImColor( 255, 255, 255, static_cast< int >( 255 * state.get< float >( ) ) );
    ImColor const col_end = ImColor( 255, 255, 255, 0 );

    draw->AddRectFilledMultiColor(
        ImVec2( center.x - 1, center.y - length - offset ),
        ImVec2( center.x + 1, center.y - offset ),
        col_start, col_start, col_end, col_end );

    draw->AddRectFilledMultiColor(
        ImVec2( center.x - 1, center.y + offset ),
        ImVec2( center.x + 1, center.y + length + offset ),
        col_end, col_end, col_start, col_start );

    draw->AddRectFilledMultiColor(
        ImVec2( center.x - length - offset, center.y - 1 ),
        ImVec2( center.x - offset, center.y + 1 ),
        col_start, col_end, col_end, col_start );

    draw->AddRectFilledMultiColor(
        ImVec2( center.x + offset, center.y - 1 ),
        ImVec2( center.x + length + offset, center.y + 1 ),
        col_end, col_start, col_start, col_end );
}