#include "objects.hpp"

c_object* c_object::find_from_id( int id )
{
    static auto fn = reinterpret_cast< c_object* ( * ) ( int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Object" ), ay_obfuscate( "FindObjectFromInstanceID" ), 1 ) );
    return fn( id );
}

void c_object::dont_destroy_on_load( )
{
    static auto fn = reinterpret_cast< void ( * )( c_object* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Object" ), ay_obfuscate( "DontDestroyOnLoad" ), 1 ) );
    fn( this );
}