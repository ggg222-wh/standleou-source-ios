#include "mesh.hpp"

#include "../../il2cpp/il2cpp.hpp"

#include "../../unity/renderer/renderer.hpp"

c_renderer* c_mesh::get_skinned( )
{
    static auto fn = reinterpret_cast< c_renderer* ( * ) ( c_mesh* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Common" ), ay_obfuscate( "SkinnedMeshLodGroup" ), ay_obfuscate( "get_SkinnedMeshRenderer" ), 0 ) );
    return fn( this );
}