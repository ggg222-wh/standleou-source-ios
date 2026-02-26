#include "system.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"

unity::array< unsigned char >* c_system::file_to_bytes( unity::string* path )
{
    static auto fn = reinterpret_cast< unity::array< unsigned char >* ( * ) ( unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "System.IO" ), ay_obfuscate( "File" ), ay_obfuscate( "ReadAllBytes" ), 1 ) );
    return fn( path );
}