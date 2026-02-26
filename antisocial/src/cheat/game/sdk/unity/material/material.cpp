#include "material.hpp"

#include "../resources/resources.hpp"

#include "../../il2cpp/il2cpp.hpp"

#include "imgui.h"

uintptr_t c_material::get_texture( unity::string* name )
{
    static auto fn = reinterpret_cast< uintptr_t ( * )( c_material*, unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( "GetTexture" ), 1 ) );
    return fn( this, name );
}

void c_material::set_texture( unity::string* name, uintptr_t texture )
{
    static auto fn = reinterpret_cast< void ( * )( c_material*, unity::string*, uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( "SetTexture" ), 2 ) );
    fn( this, name, texture );
}

void c_material::set_shader( uintptr_t shader )
{
    static auto fn = reinterpret_cast< void ( * )( c_material*, uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( "set_shader" ), 1 ) );
    fn( this, shader );
}

bool c_material::has_property( unity::string* property )
{
    static auto fn = reinterpret_cast< bool ( * )( c_material*, unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( "HasProperty" ), 1 ) );
    return fn( this, property );
}

void c_material::set_color( unity::string* name, const ImColor& color )
{
    static auto fn = reinterpret_cast< void ( * )( c_material*, unity::string*, ImColor ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( "SetColor" ), 2 ) );
    fn( this, name, color );
}

void c_material::set_int( unity::string* name, const int& value )
{
    static auto fn = reinterpret_cast< void ( * )( c_material*, unity::string*, int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( "SetInt" ), 2 ) );
    fn( this, name, value );
}

void c_material::ctor( uintptr_t shader )
{
    static auto fn = reinterpret_cast< void ( * )( c_material*, uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ), ay_obfuscate( ".ctor" ), 1 ) );
    return fn( this, shader );
}

c_material* c_material::create( const char* shader )
{
    uintptr_t shader_ptr = c_resources::find_shader( unity::string::create( shader ) );

    c_material* material = 0;

    static il2cpp_image* unity_asm;
    if ( !unity_asm )
        unity_asm = g_ctx->il2cpp->get_img_to_asm( ay_obfuscate( "UnityEngine.CoreModule" ) );

    static il2cpp_class* material_class;
    if ( !material_class )
        material_class = reinterpret_cast< il2cpp_class* ( * ) ( il2cpp_image*, const char*, const char* ) >( g_ctx->il2cpp->functions.class_from_name )( unity_asm, ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Material" ) );

    material = reinterpret_cast< c_material* >( reinterpret_cast< il2cpp_object* ( * ) ( il2cpp_class* ) >( g_ctx->il2cpp->functions.object_new )( material_class ) );

    material->ctor( shader_ptr );

    return material;
}