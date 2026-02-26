#include "nuke.hpp"

#include "game/sdk/axlebolt/grenade_manager/grenade_manager.hpp"

#include "globals.hpp"

void c_nuke::fire( vec3_t start, vec3_t direction, int grenade ) const
{
    if ( !c::get< bool >( g_ctx->cfg.misc_nuke ) || !g_ctx->user_data.premium )
        return;

    auto* const instance = c_grenade_manager::get_instance( );
    if ( !instance )
        return;

    instance->throw_grenade( grenade, start, direction, 99.f, 1 );
}