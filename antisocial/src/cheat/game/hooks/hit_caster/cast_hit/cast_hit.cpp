#include "cast_hit.hpp"

#include "globals.hpp"

#include "game/features/ragebot/ragebot.hpp"
#include "game/features/misc/peek_assist/peek_assist.hpp"
#include "game/features/visual/bullet_tracers/bullet_tracers.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/misc/nuke/nuke.hpp"

#include "memory_manager/memory_manager.hpp"

#include "../../il2cpp_hook.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

void hk::hit_caster::cast_hit::hook( )
{
    memory_manager::hook( ( 0x17FA2FC ), reinterpret_cast< void* >( hk_cast ), reinterpret_cast< void** >( &og_cast ) );
}

uintptr_t hk::hit_caster::cast_hit::hk_cast( vec3_t start, vec3_t target, uintptr_t param )
{
    g_ctx->features.ragebot->cast_hit( start, target, param );

    if ( g_ctx->features.peek_assist->m_peeking )
        g_ctx->features.peek_assist->m_peeking = false;

    uintptr_t const ret = og_cast( start, target, param );

    if ( c::get< bool >( g_ctx->cfg.visual_bullet_tracers ) && ret )
    {
        vec3_t const end_position = *reinterpret_cast< vec3_t* >( ret + 0x30 );

        g_ctx->features.bullet_tracers->push_tracer(
            { .start = g_ctx->features.thirdperson->get_unmodified_view( ),
              .end = end_position } );
    }

    auto const get_grenade = []( const int& type ) -> int {
        switch ( type )
        {
        case 0: // HE
            return 91;
        case 1: // Flash
            return 93;
        case 2: // Smoke
            return 92;
        }
    };

    g_ctx->features.nuke->fire( g_ctx->features.thirdperson->get_unmodified_view( ), target, get_grenade( c::get< int >( g_ctx->cfg.misc_nuke_type ) ) );

    return ret;
}