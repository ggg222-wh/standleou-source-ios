#include "camera.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"

#include "globals.hpp"

#include "game/sdk/unity/component/component.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/material/material.hpp"

c_camera* c_camera::get_main( )
{
    static auto func = reinterpret_cast< c_camera* ( * ) ( ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "get_main" ), 0 ) );
    return func( );
}

vec3_t c_camera::get_view( )
{
    auto* const main = get_main( );
    if ( !main )
        return { 0, 0, 0 };

    auto* const transform = reinterpret_cast< c_component* >( main )->get_transform( );
    if ( !transform )
        return { 0, 0, 0 };

    return transform->get_position( );
}

vec3_t c_camera::world_to_viewport( vec3_t position, int eye )
{
    static auto func = reinterpret_cast< vec3_t ( * )( c_camera*, vec3_t, int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "WorldToViewportPoint" ), 2 ) );
    return func( this, position, eye );
}

void c_camera::set_aspect_ratio( const float& ratio )
{
    static auto fn = reinterpret_cast< void ( * )( c_camera*, float ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "set_aspect" ), 1 ) );
    fn( this, ratio );
}

void c_camera::set_fov( const float& value )
{
    static auto fn = reinterpret_cast< void ( * )( c_camera*, float ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "set_fieldOfView" ), 1 ) );
    fn( this, value );
}

void c_camera::set_background_color( const ImColor& color )
{
    static auto set_color = reinterpret_cast< void ( * )( c_camera*, ImColor ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "set_backgroundColor" ), 1 ) );
    static auto set_flags = reinterpret_cast< void ( * )( c_camera*, int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "set_clearFlags" ), 1 ) );

    set_color( this, color );
    set_flags( this, 2 );
}

c_material* c_camera::get_skybox( )
{
    static auto fn = reinterpret_cast< c_material* ( * ) ( c_camera* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Camera" ), ay_obfuscate( "get_skyboxMaterial" ), 0 ) );
    return fn( this );
}
