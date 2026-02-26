#include "renderer.hpp"

#include "../../il2cpp/il2cpp.hpp"

c_material* c_renderer::get_material( )
{
    static auto fn = reinterpret_cast< c_material* ( * ) ( c_renderer* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Renderer" ), ay_obfuscate( "get_material" ), 0 ) );
    return fn( this );
}

unity::array< c_material* >* c_renderer::get_materials( )
{
    static auto fn = reinterpret_cast< unity::array< c_material* >* ( * ) ( c_renderer* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Renderer" ), ay_obfuscate( "get_materials" ), 0 ) );
    return fn( this );
}

void c_renderer::set_materials( unity::array< c_material* >* materials )
{
    static auto fn = reinterpret_cast< void ( * )( c_renderer*, unity::array< c_material* >* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Renderer" ), ay_obfuscate( "set_materials" ), 1 ) );
    return fn( this, materials );
}

void c_renderer::set_material( c_material* material )
{
    static auto fn = reinterpret_cast< void ( * )( c_renderer*, uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Renderer" ), ay_obfuscate( "set_material" ), 1 ) );
    return fn( this, reinterpret_cast< uintptr_t >( material ) );
}
