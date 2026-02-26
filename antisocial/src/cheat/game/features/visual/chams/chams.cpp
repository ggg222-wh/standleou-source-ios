#include "chams.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/mesh/mesh.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/component/component.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/type/type.hpp"
#include "game/sdk/unity/renderer/renderer.hpp"
#include "game/sdk/unity/resources/resources.hpp"
#include "game/sdk/unity/material/material.hpp"
#include "game/sdk/unity/objects/objects.hpp"
#include "game/sdk/unity/shader/shader.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/unity/texture/texture.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

#include "memory_manager/memory_manager.hpp"

#include "game/entities/entities.hpp"

#include "globals.hpp"

void c_chams::clear_bundles( )
{
    // t.me/antisocial_pub
}

void c_chams::load_bundles( )
{
    // t.me/antisocial_pub
}

void c_chams::init_textures( )
{
    if ( c::get< chams_config_t >( g_ctx->cfg.chams_local ).custom_texture )
    {
        std::string const texture_path = g_ctx->documents_directory + crypt_string( "/local.png" );
        if ( std::filesystem::exists( texture_path ) && !m_local_texture )
            m_local_texture = c_texture::load_from_file( texture_path );
    }

    if ( c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).custom_texture )
    {
        std::string const texture_path = g_ctx->documents_directory + crypt_string( "/enemies.png" );
        if ( std::filesystem::exists( texture_path ) && !m_enemies_texture )
            m_enemies_texture = c_texture::load_from_file( texture_path );
    }

    if ( c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).custom_texture )
    {
        std::string const fallback_texture_path = g_ctx->documents_directory + crypt_string( "/weapon.png" );
        if ( std::filesystem::exists( fallback_texture_path ) && !m_weapon_fallback_texture )
            m_weapon_fallback_texture = c_texture::load_from_file( fallback_texture_path );

        std::string const base_path = g_ctx->documents_directory + crypt_string( "/weapons/" );
        if ( std::filesystem::exists( base_path ) )
        {
            for ( const auto& [ id, weapon ] : c_weapon_controller::get_by_id )
            {
                std::string const tex_path = base_path + std::to_string( id ) + crypt_string( ".png" );
                if ( std::filesystem::exists( tex_path ) && m_weapon_textures.find( id ) == m_weapon_textures.end( ) )
                {
                    m_weapon_textures[ id ] = c_texture::load_from_file( tex_path );
                }
            }
        }
    }
}

void c_chams::clear_textures( )
{
    m_local_texture = m_enemies_texture = m_weapon_fallback_texture = nullptr;

    m_weapon_textures.clear( );
}

void c_chams::process_enemy( c_player_controller* ent )
{
    if ( !ent || !ent->alive( ) || !c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).enabled )
        return;

    c_mesh* mesh = ent->mesh_lod_group( );
    if ( !mesh )
        return;

    c_renderer* renderer = mesh->get_skinned( );
    if ( !renderer )
        return;

    c_material* material = renderer->get_material( );

    if ( enemies_frames < 10 )
    {
        enemies_frames++;
        return;
    }

    auto const shader = [ & ]( ) -> uintptr_t {
        if ( c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).custom_texture )
        {
            return c_resources::find_shader( unity::string::create( ay_obfuscate( "Legacy Shaders/Diffuse" ) ) );
        }

        return c_resources::find_shader( unity::string::create( ay_obfuscate( "Hidden/Internal-Colored" ) ) );
    }( );

    if ( shader )
        material->set_shader( shader );

    bool const visible = ent->visible( );

    material->set_color( unity::string::create( ay_obfuscate( "_Color" ) ), visible ? c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).visible_color : c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).invisible_color );

    if ( !c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).custom_texture )
    {
        material->set_int( unity::string::create( ay_obfuscate( "_ZWrite" ) ), 0 );
        material->set_int( unity::string::create( ay_obfuscate( "_ZTest" ) ), 8 );
    } else
        material->set_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ), reinterpret_cast< uintptr_t >( m_enemies_texture ) );

    renderer->set_material( material );
}

void c_chams::process_local( c_player_controller* ent )
{
    if ( !ent || !ent->alive( ) || !c::get< chams_config_t >( g_ctx->cfg.chams_local ).enabled )
        return;

    c_mesh* mesh = ent->mesh_lod_group( );
    if ( !mesh )
        return;

    c_renderer* renderer = mesh->get_skinned( );
    if ( !renderer )
        return;

    c_material* material = renderer->get_material( );

    if ( local_frames < 10 )
    {
        local_frames++;
        return;
    }

    auto const shader = [ & ]( ) -> uintptr_t {
        if ( c::get< chams_config_t >( g_ctx->cfg.chams_local ).custom_texture )
        {
            return c_resources::find_shader( unity::string::create( ay_obfuscate( "Legacy Shaders/Diffuse" ) ) );
        }

        return c_resources::find_shader( unity::string::create( ay_obfuscate( "Hidden/Internal-Colored" ) ) );
    }( );

    if ( shader )
        material->set_shader( shader );

    bool const visible = ent->visible( );

    material->set_color( unity::string::create( ay_obfuscate( "_Color" ) ), visible ? c::get< chams_config_t >( g_ctx->cfg.chams_local ).visible_color : c::get< chams_config_t >( g_ctx->cfg.chams_local ).invisible_color );

    if ( !c::get< chams_config_t >( g_ctx->cfg.chams_local ).custom_texture )
    {
        material->set_int( unity::string::create( ay_obfuscate( "_ZWrite" ) ), 0 );
        material->set_int( unity::string::create( ay_obfuscate( "_ZTest" ) ), 8 );
    } else
        material->set_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ), reinterpret_cast< uintptr_t >( m_local_texture ) );

    renderer->set_material( material );
}

void c_chams::process_weapon( c_player_controller* ent )
{
    if ( !ent || !ent->alive( ) || !c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).enabled )
        return;

    c_weapon_controller* weapon = ent->get_weapon( );
    if ( !weapon )
        return;

    if ( weapon_frames < 10 )
    {
        weapon_frames++;
        return;
    }

    int const id = weapon->get_id( );

    auto const lod = weapon->lod_group( );
    if ( !lod )
        return;

    unity::array< c_renderer* >* renderers = *reinterpret_cast< unity::array< c_renderer* >** >( lod + 0x38 );
    if ( !renderers || renderers->empty( ) )
        return;

    for ( auto& renderer : renderers->to_cpp_array( ) )
    {
        if ( !renderer )
            continue;

        c_material* material = renderer->get_material( );

        auto const shader = [ & ]( ) -> uintptr_t {
            if ( c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).custom_texture )
            {
                return c_resources::find_shader( unity::string::create( ay_obfuscate( "Legacy Shaders/Diffuse" ) ) );
            }

            return c_resources::find_shader( unity::string::create( ay_obfuscate( "Hidden/Internal-Colored" ) ) );
        }( );

        if ( shader )
            material->set_shader( shader );

        bool const visible = ent->visible( );

        material->set_color( unity::string::create( ay_obfuscate( "_Color" ) ), visible ? c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).visible_color : c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).invisible_color );

        if ( !c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).custom_texture )
        {
            material->set_int( unity::string::create( ay_obfuscate( "_ZWrite" ) ), 0 );
            material->set_int( unity::string::create( ay_obfuscate( "_ZTest" ) ), 8 );
        } else
            material->set_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ), ( m_weapon_textures.find( id ) != m_weapon_textures.end( ) ) ? reinterpret_cast< uintptr_t >( m_weapon_textures[ id ] ) : reinterpret_cast< uintptr_t >( m_weapon_fallback_texture ) );

        renderer->set_material( material );
    }
}
