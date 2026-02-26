#include "texture_manager.hpp"

#include "globals.hpp"

#include "ws_pranks/ws_pranks.hpp"

#include "interface/custom_ui/ui.hpp"

#include "interface/custom_ui/advanced/sample_renderers/dropped_weapons/dropped_weapons.hpp"
#include "interface/custom_ui/advanced/sample_renderers/thrown_grenades/thrown_grenades.hpp"
#include "interface/custom_ui/advanced/sample_renderers/bomb/bomb.hpp"
#include "interface/custom_ui/advanced/sample_renderers/enemies/enemies.hpp"

#include <fstream>

void c_texture_manager::init_internal( )
{
    if ( m_initialized )
        return;

    m_initialized = true;

    auto const load_texture_from_file = [ this ]( const std::string& path, texture_data_t* out_texture, const std::string& uuid ) -> bool {
        std::ifstream file( path, std::ios::binary | std::ios::ate );
        if ( !file.is_open( ) )
            return false;

        std::streamsize size = file.tellg( );
        file.seekg( 0, std::ios::beg );

        std::vector< unsigned char > buffer( size );
        if ( !file.read( reinterpret_cast< char* >( buffer.data( ) ), size ) )
            return false;

        return load( buffer.data( ), static_cast< int >( size ), out_texture, uuid );
    };

    load( g_ctx->user_data.profile_picture_raw.data( ), g_ctx->user_data.profile_picture_raw.size( ), &g_ctx->user_data.profile_picture, crypt_string( "internal_profile_picture" ) );

    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/background.png" ), &c_user_interface::m_preview_base.background, crypt_string( "internal_preview_bg" ) );

    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/ayanami_rei/body.png" ), &c_user_interface::m_ayanami_rei.body, crypt_string( "internal_preview_rei_body" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/ayanami_rei/arms.png" ), &c_user_interface::m_ayanami_rei.arms, crypt_string( "internal_preview_rei_arms" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/ayanami_rei/legs.png" ), &c_user_interface::m_ayanami_rei.legs, crypt_string( "internal_preview_rei_legs" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/ayanami_rei/head.png" ), &c_user_interface::m_ayanami_rei.head, crypt_string( "internal_preview_rei_head" ) );

    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/stella/body.png" ), &c_user_interface::m_stella.body, crypt_string( "internal_preview_stella_body" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/stella/arms.png" ), &c_user_interface::m_stella.arms, crypt_string( "internal_preview_stella_arms" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/stella/legs.png" ), &c_user_interface::m_stella.legs, crypt_string( "internal_preview_stella_legs" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/girls/stella/head.png" ), &c_user_interface::m_stella.head, crypt_string( "internal_preview_stella_head" ) );

    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/dropped_weapons.png" ), &c_sample_dropped_weapons::m_texture, crypt_string( "internal_preview_dropped_weapons" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/thrown_grenades.png" ), &c_sample_thrown_grenades::m_texture, crypt_string( "internal_preview_thrown_grenades" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/previews/bomb.png" ), &c_sample_bomb::m_texture, crypt_string( "internal_preview_bomb" ) );

    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/jigsaw.jpeg" ), &c_ws_pranks::m_jigsaw_tex, crypt_string( "internal_ws_jigsaw" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/kaban.jpg" ), &c_ws_pranks::m_kaban_tex, crypt_string( "internal_ws_kaban" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/troll.jpg" ), &c_ws_pranks::m_troll_tex, crypt_string( "internal_ws_troll" ) );
    load_texture_from_file( g_ctx->documents_directory + crypt_string( "/plankton.png" ), &c_ws_pranks::m_plankton_tex, crypt_string( "internal_ws_plankton" ) );
}
