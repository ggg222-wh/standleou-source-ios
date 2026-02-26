#include "aspect_ratio.hpp"

#include "game/sdk/unity/camera/camera.hpp"

#include "globals.hpp"

void c_aspect_ratio::execute( ) const
{
    if ( !c::get< bool >( g_ctx->cfg.misc_aspect_ratio_enabled ) )
        return;

    c_camera* const main = c_camera::get_main( );
    if ( !main )
        return;

    main->set_aspect_ratio( c::get< float >( g_ctx->cfg.misc_aspect_ratio ) );

    if ( !c::get< bool >( g_ctx->cfg.removals_scope_overlay ) || c::get< bool >( g_ctx->cfg.misc_fov_enabled ) )
        return;

    main->set_fov( 60 - 0.01f );
}