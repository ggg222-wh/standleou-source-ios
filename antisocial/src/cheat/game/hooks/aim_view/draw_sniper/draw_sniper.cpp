#include "draw_sniper.hpp"

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"

#include "../../il2cpp_hook.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

void hk::aim_view::draw_sniper::hook( )
{
    memory_manager::hook( ( 0x1877E68 ), reinterpret_cast< void* >( hk_draw ), reinterpret_cast< void** >( &og_draw ) );
    memory_manager::hook( ( 0x187843C ), reinterpret_cast< void* >( hk_draw1 ), reinterpret_cast< void** >( &og_draw1 ) );
    memory_manager::hook( ( 0x18781EC ), reinterpret_cast< void* >( hk_draw2 ), reinterpret_cast< void** >( &og_draw2 ) );
}

void hk::aim_view::draw_sniper::hk_draw( uintptr_t instance, uintptr_t gun )
{
    if ( !c::get< bool >( g_ctx->cfg.removals_scope_overlay ) )
        og_draw( instance, gun );
}

void hk::aim_view::draw_sniper::hk_draw1( uintptr_t instance, uintptr_t gun )
{
    if ( !c::get< bool >( g_ctx->cfg.removals_scope_overlay ) )
        og_draw1( instance, gun );
}

void hk::aim_view::draw_sniper::hk_draw2( uintptr_t instance, uintptr_t gun )
{
    if ( !c::get< bool >( g_ctx->cfg.removals_scope_overlay ) )
        og_draw2( instance, gun );
}
