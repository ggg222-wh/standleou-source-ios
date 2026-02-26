#include "type.hpp"

#include "globals.hpp"

#include "../../il2cpp/il2cpp.hpp"

uintptr_t c_type::get_type( unity::string* str )
{
    static auto fn = reinterpret_cast< uintptr_t ( * )( unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "System" ), ay_obfuscate( "Type" ), ay_obfuscate( "GetType" ), 1 ) );
    return fn( str );
}
