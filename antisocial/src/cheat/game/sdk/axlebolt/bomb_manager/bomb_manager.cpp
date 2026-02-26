#include "bomb_manager.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"
#include "game/sdk/unity/unity.hpp"

#include "globals.hpp"

uintptr_t c_bomb_manager::m_instance { 0 };

c_bomb_manager* c_bomb_manager::get_instance( )
{
    if ( !m_instance )
    {
        uintptr_t const scene_class = reinterpret_cast< uintptr_t >( g_ctx->il2cpp->get_class( ay_obfuscate( "Axlebolt.Standoff.Inventory.Bomb" ), ay_obfuscate( "BombManager" ) ) );

        if ( !scene_class )
            return 0;

        m_instance = reinterpret_cast< uintptr_t ( * )( uintptr_t, const char* ) >( g_ctx->il2cpp->functions.class_get_field_from_name )( scene_class, ay_obfuscate( "_instance" ) );
    }

    c_bomb_manager* ret = nullptr;
    using field_static_get_value_fn = void ( * )( uintptr_t, void** );
    auto fn = reinterpret_cast< field_static_get_value_fn >( g_ctx->il2cpp->functions.field_static_get_value );
    fn( m_instance, reinterpret_cast< void** >( &ret ) );

    return ret;
}
