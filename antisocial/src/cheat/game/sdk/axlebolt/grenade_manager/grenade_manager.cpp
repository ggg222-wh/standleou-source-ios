#include "grenade_manager.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"
#include "game/sdk/unity/unity.hpp"

#include "globals.hpp"

uintptr_t c_grenade_manager::m_instance { 0 };

c_grenade_manager* c_grenade_manager::get_instance( )
{
    if ( !m_instance )
    {
        uintptr_t const scene_class = reinterpret_cast< uintptr_t >( g_ctx->il2cpp->get_class( ay_obfuscate( "Axlebolt.Standoff.Inventory.Grenade" ), ay_obfuscate( "GrenadeManager" ) ) );

        if ( !scene_class )
            return 0;

        m_instance = reinterpret_cast< uintptr_t ( * )( uintptr_t, const char* ) >( g_ctx->il2cpp->functions.class_get_field_from_name )( scene_class, ay_obfuscate( "_instance" ) );
    }

    c_grenade_manager* ret = nullptr;
    using field_static_get_value_fn = void ( * )( uintptr_t, void** );
    auto fn = reinterpret_cast< field_static_get_value_fn >( g_ctx->il2cpp->functions.field_static_get_value );
    fn( m_instance, reinterpret_cast< void** >( &ret ) );

    return ret;
}

void c_grenade_manager::throw_grenade( int id, vec3_t start, vec3_t direction, float g, int w )
{
    static auto fn = reinterpret_cast< void ( * )( c_grenade_manager*, int, vec3_t, vec3_t, float, int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Inventory.Grenade" ), ay_obfuscate( "GrenadeManager" ), ay_obfuscate( "ThrowGrenade" ), 5 ) );
    return fn( this, id, start, direction, g, w );
}