#include "late_update.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"

#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/ragebot/ragebot.hpp"

#include "game/sdk/axlebolt/aiming_data/aiming_data.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/objects/objects.hpp"
#include "game/sdk/unity/game_object/game_object.hpp"
#include "game/sdk/unity/type/type.hpp"

#include "game/sdk/structures/view_mode.hpp"

#include "globals.hpp"

#include "game/entities/entities.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"

#include "memory_manager/memory_manager.hpp"

#include "../../il2cpp_hook.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

void hk::player_controller::late_update::hook( )
{
    memory_manager::hook( 0x195918C, reinterpret_cast< void* >( hk_update ), reinterpret_cast< void** >( &og_update ) );

    il2cpp_class* const klass = g_ctx->il2cpp->get_class( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "GameObject" ) );

    auto* object = reinterpret_cast< c_game_object* >( reinterpret_cast< il2cpp_object* ( * ) ( il2cpp_class* ) >( g_ctx->il2cpp->functions.object_new )( klass ) );

    object->create( "_" );
    auto type = c_type::get_type( unity::string::create( ay_obfuscate( "Axlebolt.Standoff.Main.Inventory.Charms.JointCorrector, Assembly-CSharp.dll" ) ) );
    object->add_component( type );
    object->dont_destroy_on_load( );
}

void hk::player_controller::late_update::hk_update( c_player_controller* entity )
{
    auto* const local = c_players_database::get( )->m_local_player;

    if ( local && local->alive( ) )
    {
        g_ctx->features.ragebot->set_angles( local );
        g_ctx->features.thirdperson->late( local );
    }

    // og_update( entity );
}