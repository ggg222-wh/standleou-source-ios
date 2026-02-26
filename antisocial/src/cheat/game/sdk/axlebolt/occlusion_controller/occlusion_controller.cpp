#include "occlusion_controller.hpp"

#include "../../il2cpp/il2cpp.hpp"

#include "globals.hpp"

bool c_occlusion_controller::set_in_area( bool v )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0x29 ) = v;
}

bool c_occlusion_controller::set_enabled( bool v )
{
    return *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 0x28 ) = v;
}

void c_occlusion_controller::set_visible( bool v )
{
    static auto fn = reinterpret_cast< void ( * )( c_occlusion_controller*, bool ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Occlusion.Dynamic" ), ay_obfuscate( "ObjectOccludee" ), ay_obfuscate( "SetVisible" ), 1 ) );
    fn( this, v );
};