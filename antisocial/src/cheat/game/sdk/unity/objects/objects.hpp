#pragma once

#include "globals.hpp"

#include "../../il2cpp/il2cpp.hpp"
#include "../unity.hpp"

class c_object
{
public:
    static c_object* find_from_id( int id );
    void dont_destroy_on_load( );
};

class c_objects
{
public:
    template < typename T >
    static unity::array< T >* get_objects_of_type( uintptr_t type )
    {
        static auto fn = reinterpret_cast< unity::array< T >* ( * ) ( uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ( "UnityEngine" ), ( "Object" ), ( "FindObjectsOfType" ), 1 ) );
        return fn( type );
    }
};