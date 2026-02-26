#include "fog.hpp"

#include "game/sdk/unity/fog_manager/fog_manager.hpp"

#include "globals.hpp"

void c_fog::execute( ) const
{
    c_fog_manager* const fog = c_fog_manager::get( );
    if ( !fog )
        return;

    fog->set_enabled( c::get< bool >( g_ctx->cfg.visual_fog_enabled ) );

    if ( c::get< bool >( g_ctx->cfg.visual_fog_enabled ) )
    {
        fog->set_color( c::get< ImVec4 >( g_ctx->cfg.visual_fog_color ) );
        fog->set_density( c::get< float >( g_ctx->cfg.visual_fog_density ) );
        fog->set_start( c::get< float >( g_ctx->cfg.visual_fog_start ) );
        fog->set_end( c::get< float >( g_ctx->cfg.visual_fog_end ) );
        fog->set_mode( c::get< int >( g_ctx->cfg.visual_fog_mode ) );
    }

    fog->update( );
}