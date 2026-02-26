#include "skybox.hpp"

#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/texture/texture.hpp"
#include "game/sdk/unity/type/type.hpp"
#include "game/sdk/unity/renderer/renderer.hpp"
#include "game/sdk/unity/resources/resources.hpp"
#include "game/sdk/unity/material/material.hpp"
#include "game/sdk/unity/objects/objects.hpp"
#include "game/sdk/unity/shader/shader.hpp"
#include "game/sdk/unity/unity.hpp"

#include "globals.hpp"

#include <filesystem>

void c_skybox::execute( )
{
    if ( !c::get< bool >( g_ctx->cfg.visual_skybox_enabled ) )
        return;

    c_camera* const main = c_camera::get_main( );
    if ( !main )
        return;

    // main->set_background_color( c::get< ImVec4 >( g_ctx->cfg.visual_skybox ) );

    auto* const material = main->get_skybox( );
    if ( !material )
        return;

    if ( passed_frames < 10 )
    {
        passed_frames++;
        return;
    }

    auto const shader = c_resources::find_shader( unity::string::create( ay_obfuscate( "Legacy Shaders/Diffuse" ) ) );

    if ( shader )
        material->set_shader( shader );

    material->set_color( unity::string::create( ay_obfuscate( "_Color" ) ), c::get< ImVec4 >( g_ctx->cfg.visual_skybox ) );

    if ( c::get< bool >( g_ctx->cfg.visual_skybox_texture ) && m_texture )
        material->set_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ), reinterpret_cast< uintptr_t >( m_texture ) );
}

void c_skybox::init_textures( )
{
    if ( !c::get< bool >( g_ctx->cfg.visual_skybox_texture ) )
        return;

    std::string const texture_path = g_ctx->documents_directory + crypt_string( "/skybox.png" );
    if ( std::filesystem::exists( texture_path ) && !m_texture )
        m_texture = c_texture::load_from_file( texture_path );
}

void c_skybox::clear_textures( )
{
    m_texture = nullptr;
}