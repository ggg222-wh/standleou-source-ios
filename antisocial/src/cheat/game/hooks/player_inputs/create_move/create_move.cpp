#include "create_move.hpp"

#include "game/features/ragebot/ragebot.hpp"

#include "game/sdk/axlebolt/player_inputs/player_inputs.hpp"

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"

#include "imgui.h"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/biped_map/biped_map.hpp"
#include "game/sdk/axlebolt/aiming_data/aiming_data.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/axlebolt/player_controls/player_controls.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/axlebolt/gun_controller/gun_controller.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/structures/euler.hpp"
#include "game/sdk/structures/delegate.hpp"
#include "game/sdk/misc/math/math.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"
#include "game/entities/entities.hpp"

#include <random>

void ( *og_filter )( c_delegate* thisptr, c_player_inputs* );
void hk_filter( c_delegate* thisptr, c_player_inputs* inputs )
{
    auto* const local = c_players_database::get( )->m_local_player;

    if ( inputs )
        c_local_player::m_old_cmd = *inputs;

    c_local_player::m_cmd = inputs;

    if ( !local || !local->alive( ) || !inputs )
    {
        g_ctx->features.ragebot->orig_camera_angles = { 0, 0, 0 };
        return og_filter( thisptr, inputs );
    }

    g_ctx->features.ragebot->antiaim( local, inputs );

    return og_filter( thisptr, inputs );
}

void hk::player_inputs::create_move::update( )
{
    uintptr_t const controls_class = reinterpret_cast< uintptr_t >( g_ctx->il2cpp->get_class( ay_obfuscate( "Axlebolt.Standoff.Controls" ), ay_obfuscate( "PlayerControls" ) ) );
    uintptr_t const controls_field = reinterpret_cast< uintptr_t ( * )( uintptr_t, const char* ) >( g_ctx->il2cpp->functions.class_get_field_from_name )( controls_class, ay_obfuscate( "<Instance>k__BackingField" ) );

    uintptr_t controls { 0 };
    reinterpret_cast< void ( * )( uintptr_t, void** ) >( g_ctx->il2cpp->functions.field_static_get_value )( controls_field, reinterpret_cast< void** >( &controls ) );
    if ( controls )
    {
        reinterpret_cast< c_player_controls* >( controls )->input_filter( )->hook( ( void* ) hk_filter, ( void** ) &og_filter );
    }
}