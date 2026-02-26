#include "spoof.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"

#include "globals.hpp"

void c_spoof::uid( c_player_controller* ent ) const
{
    if ( !ent || !ent->alive( ) || !c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_uid ).enabled || !g_ctx->user_data.premium )
        return;

    c_photon_player* const photon = ent->photon_player( );
    if ( !photon )
        return;

    photon->set_user_id( c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_uid ).spoof );
}

void c_spoof::clan( c_player_controller* ent ) const
{
    if ( !ent || !ent->alive( ) || !c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_clan ).enabled || !g_ctx->user_data.premium )
        return;

    c_photon_player* const photon = ent->photon_player( );
    if ( !photon )
        return;

    photon->set_clan_tag( c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_clan ).spoof );
}
