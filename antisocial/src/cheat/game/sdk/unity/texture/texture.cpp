#include "texture.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"

#include "../system/system.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

bool c_texture::image_from_bytes( unity::array< unsigned char >* data )
{
    static auto fn = reinterpret_cast< bool ( * )( c_texture*, unity::array< unsigned char >* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "ImageConversion" ), ay_obfuscate( "LoadImage" ), 2 ) );
    return fn( this, data );
}

void c_texture::ctor( int width, int height, int a, bool b )
{
    static auto fn = reinterpret_cast< void ( * )( c_texture*, int, int, int, bool ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Texture2D" ), ay_obfuscate( ".ctor" ), 4 ) );
    fn( this, width, height, a, b );
}

c_texture* c_texture::load_from_file( const std::string& path )
{
    il2cpp_class* const texture2d_class = g_ctx->il2cpp->get_class( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Texture2D" ) );

    c_texture* const texture = reinterpret_cast< c_texture* >( reinterpret_cast< il2cpp_object* ( * ) ( il2cpp_class* ) >( g_ctx->il2cpp->functions.object_new )( texture2d_class ) );

    texture->ctor( 1000, 1000, 5, false );

    auto* const bytes = c_system::file_to_bytes( unity::string::create( path.c_str( ) ) );

    texture->image_from_bytes( bytes );

    return texture;
}