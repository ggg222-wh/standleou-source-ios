#include "transform.hpp"

#include "../../il2cpp/il2cpp.hpp"

#include "globals.hpp"

vec3_t c_transform::get_position( )
{
    static auto func = reinterpret_cast< vec3_t ( * )( c_transform* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Transform" ), ay_obfuscate( "get_position" ), 0 ) );
    return func( this );
}

vec3_t c_transform::get_forward( )
{
    static auto func = reinterpret_cast< vec3_t ( * )( c_transform* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Transform" ), ay_obfuscate( "get_forward" ), 0 ) );
    return func( this );
}

vec3_t c_transform::get_up( )
{
    static auto func = reinterpret_cast< vec3_t ( * )( c_transform* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Transform" ), ay_obfuscate( "get_up" ), 0 ) );
    return func( this );
}

void c_transform::set_position( vec3_t position )
{
    static auto func = reinterpret_cast< void ( * )( c_transform*, vec3_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Transform" ), ay_obfuscate( "set_position" ), 1 ) );
    func( this, position );
}

void c_transform::set_local_euler( euler_angles_t rotation )
{
    static auto fn = reinterpret_cast< void ( * )( c_transform*, euler_angles_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Transform" ), ay_obfuscate( "set_localEulerAngles" ), 1 ) );
    fn( this, rotation );
}

void c_transform::set_euler( euler_angles_t rotation )
{
    static auto fn = reinterpret_cast< void ( * )( c_transform*, euler_angles_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Transform" ), ay_obfuscate( "set_eulerAngles" ), 1 ) );
    fn( this, rotation );
}
