#include "resources.hpp"

#include "../../il2cpp/il2cpp.hpp"

uintptr_t c_resources::find_shader( unity::string* name )
{
    static auto fn = reinterpret_cast< uintptr_t ( * )( unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "ResourcesAPIInternal" ), ay_obfuscate( "FindShaderByName" ), 1 ) );
    return fn( name );
}
