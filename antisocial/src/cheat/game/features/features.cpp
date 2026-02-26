#include "features.hpp"

#include "game/features/ragebot/ragebot.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/visual/world_modulation/world_modulation.hpp"
#include "game/features/visual/chams/chams.hpp"
#include "game/features/visual/bullet_tracers/bullet_tracers.hpp"
#include "game/features/visual/hit_points/hit_points.hpp"
#include "game/features/visual/fog/fog.hpp"
#include "game/features/visual/grenade_prediction/grenade_prediction.hpp"
#include "game/features/visual/esp/esp.hpp"
#include "game/features/movement/bunnyhop/bunnyhop.hpp"
#include "game/features/misc/peek_assist/peek_assist.hpp"
#include "game/features/misc/aspect_ratio/aspect_ratio.hpp"
#include "game/features/misc/spoof/spoof.hpp"
#include "game/features/misc/removals/removals.hpp"
#include "game/features/misc/fov/fov.hpp"
#include "game/features/misc/prop_setter/prop_setter.hpp"
#include "game/features/misc/chat_spam/chat_spam.hpp"
#include "game/features/exploits/weapon/weapon.hpp"
#include "game/features/visual/skybox/skybox.hpp"
#include "game/features/visual/particles/particles.hpp"
#include "game/features/visual/hit_markers/hit_markers.hpp"

#include "chat/chat.hpp"

#include "globals.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/axlebolt/photon_network/photon_network.hpp"

void c_features_manager::process_local_player( c_player_controller* local ) const
{
    if ( !local || !local->alive( ) )
        return;

    c_photon_player* photon = local->photon_player( );
    if ( !photon )
        return;

    if ( !c_photon_network::is_master( photon ) )
        c_photon_network::set_master( photon );

    g_ctx->chat->execute( );

    g_ctx->features.ragebot->execute( local );
    g_ctx->features.bunnyhop->execute( local );
    g_ctx->features.peek_assist->execute( local );
    g_ctx->features.thirdperson->update( local );
    g_ctx->features.modulation->modulate( );
    g_ctx->features.aspect_ratio->execute( );
    g_ctx->features.chams->process_local( local );
    g_ctx->features.chams->process_weapon( local );
    g_ctx->features.spoof->uid( local );
    g_ctx->features.spoof->clan( local );
    g_ctx->features.fov->execute( );
    g_ctx->features.ragebot->fake_lags( );
    g_ctx->features.fog->execute( );
    g_ctx->features.grenade_prediction->execute( local );
    g_ctx->features.prop_setter->execute( local );
    g_ctx->features.chat_spam->execute( );
    g_ctx->features.weapon_exploits->execute( local );
    g_ctx->features.skybox->execute( );
}

void c_features_manager::process_enemy( c_player_controller* ent ) const
{
    if ( !ent || !ent->alive( ) )
        return;

    g_ctx->features.chams->process_enemy( ent );
}

void c_features_manager::process_post_render( ) const
{
    g_ctx->features.bullet_tracers->render( );
    g_ctx->features.hit_points->render( );
    g_ctx->features.hit_markers->render( );
    g_ctx->features.grenade_prediction->render( );
    g_ctx->features.esp->process_thrown_grenades( );
    g_ctx->features.esp->process_planted_bomb( );
    g_ctx->features.removals->draw_sniper_scope( );
    g_ctx->features.ragebot->render( );
}

void c_features_manager::process_pre_render( ) const
{
    g_ctx->features.particles->ambient( );
    g_ctx->features.esp->process_dropped_weapons( );
}