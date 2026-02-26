#include "ws_pranks.hpp"

#include <filesystem>
#include <fstream>

#include "globals.hpp"

texture_data_t c_ws_pranks::m_jigsaw_tex { };
texture_data_t c_ws_pranks::m_kaban_tex { };
texture_data_t c_ws_pranks::m_plankton_tex { };
texture_data_t c_ws_pranks::m_troll_tex { };

void c_ws_pranks::init( )
{
    if ( m_initialized )
        return;

    m_initialized = true;
}

void c_ws_pranks::prank( const prank_type& type )
{
    m_type = type;
    m_alpha.set< float >( 5.f );
}

void c_ws_pranks::render( )
{
    if ( m_alpha.get< float >( ) < 0.01f )
        return;

    m_alpha.set< float >( m_alpha.get< float >( ) - 0.1f );

    switch ( m_type )
    {
    case prank_type::jigsaw:
    {
        if ( !m_jigsaw_tex.loaded )
            return;

        ImGui::GetForegroundDrawList( )->AddImage( ( ImTextureID ) m_jigsaw_tex.texture_id, { 0, 0 }, ImGui::GetIO( ).DisplaySize, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, m_alpha.get< float >( ) ) );
    }
    break;
    case prank_type::kaban:
    {
        if ( !m_kaban_tex.loaded )
            return;

        ImGui::GetForegroundDrawList( )->AddImage( ( ImTextureID ) m_kaban_tex.texture_id, { 0, 0 }, ImGui::GetIO( ).DisplaySize, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, m_alpha.get< float >( ) ) );
    }
    break;
    case prank_type::troll:
    {
        if ( !m_troll_tex.loaded )
            return;

        ImGui::GetForegroundDrawList( )->AddImage( ( ImTextureID ) m_troll_tex.texture_id, { 0, 0 }, ImGui::GetIO( ).DisplaySize, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, m_alpha.get< float >( ) ) );
    }
    break;
    case prank_type::plankton:
    {
        if ( !m_plankton_tex.loaded )
            return;

        ImGui::GetForegroundDrawList( )->AddImage( ( ImTextureID ) m_plankton_tex.texture_id, { 0, 0 }, ImGui::GetIO( ).DisplaySize, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, m_alpha.get< float >( ) ) );
    }
    break;
    default:
        break;
    };
}