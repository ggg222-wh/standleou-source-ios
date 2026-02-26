#include "zoom.hpp"

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"

#include "../../il2cpp_hook.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

void hk::camera_scope_zoomer::zoom::hook( )
{
    memory_manager::hook( ( 0x17E6410 ), reinterpret_cast< void* >( hk_zoom ), reinterpret_cast< void** >( &og_zoom ) );
}

void hk::camera_scope_zoomer::zoom::hk_zoom( uintptr_t i, float a, float b )
{
    if ( !c::get< bool >( g_ctx->cfg.removals_scope_overlay ) )
        og_zoom( i, a, b );
}
