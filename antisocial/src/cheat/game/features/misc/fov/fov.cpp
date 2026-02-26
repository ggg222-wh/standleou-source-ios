#include "fov.hpp"

#include "globals.hpp"

#include "game/sdk/unity/camera/camera.hpp"
#include "game/entities/entities.hpp"

void c_fov::execute( ) const
{
    if ( !c::get< bool >( g_ctx->cfg.misc_fov_enabled ) )
        return;

    c_camera* const main = c_camera::get_main( );
    if ( !main )
        return;

    main->set_fov( c::get< int >( g_ctx->cfg.misc_fov ) );

    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
        return;

    uintptr_t const main_camera = reinterpret_cast< uintptr_t >( local->main_camera( ) );
    if ( !main_camera )
        return;

    uintptr_t const zoomer = *reinterpret_cast< uintptr_t* >( main_camera + 0x20 );
    if ( !zoomer )
        return;

    *reinterpret_cast< float* >( zoomer + 0x30 ) = c::get< int >( g_ctx->cfg.misc_fov );
}