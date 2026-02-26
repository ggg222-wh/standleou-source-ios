#include "component.hpp"
#include "../transform/transform.hpp"

#include "../../il2cpp/il2cpp.hpp"
#include "globals.hpp"

#include "../../unity/unity.hpp"

c_transform* c_component::get_transform( )
{
    static auto func = reinterpret_cast< c_transform* ( * ) ( c_component* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Component" ), ay_obfuscate( "get_transform" ), 0 ) );
    return func( this );
}

unity::string* c_component::get_tag( )
{
    static auto fn = reinterpret_cast< unity::string* ( * ) ( c_component* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Component" ), ay_obfuscate( "get_tag" ), 0 ) );
    return fn( this );
}

uintptr_t c_component::get_game_object( )
{
    static auto fn = reinterpret_cast< uintptr_t ( * )( c_component* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Component" ), ay_obfuscate( "get_gameObject" ), 0 ) );
    return fn( this );
}