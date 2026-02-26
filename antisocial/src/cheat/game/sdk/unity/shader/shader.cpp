#include "shader.hpp"

#include "../../il2cpp/il2cpp.hpp"

#include "globals.hpp"

int c_shader::property_to_id( unity::string* property )
{
    static auto fn = reinterpret_cast< int ( * )( unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Shader" ), ay_obfuscate( "PropertyToID" ), 1 ) );
    return fn( property );
}