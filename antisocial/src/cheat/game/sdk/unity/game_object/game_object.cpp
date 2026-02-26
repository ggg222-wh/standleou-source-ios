#include "game_object.hpp"

void c_game_object::create( const char* const& name )
{
    static auto fn = reinterpret_cast< void ( * )( c_game_object*, unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "GameObject" ), ay_obfuscate( "Internal_CreateGameObject" ), 2 ) );
    fn( this, unity::string::create( name ) );
}

c_component* c_game_object::add_component( uintptr_t type )
{
    static auto fn = reinterpret_cast< c_component* ( * ) ( c_game_object*, uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "GameObject" ), ay_obfuscate( "AddComponent" ), 1 ) );
    return fn( this, type );
}