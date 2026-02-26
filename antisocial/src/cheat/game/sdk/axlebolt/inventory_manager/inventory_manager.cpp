#include "inventory_manager.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"
#include "game/sdk/unity/unity.hpp"

#include "globals.hpp"

uintptr_t c_inventory_manager::m_instance { 0 };

c_inventory_manager* c_inventory_manager::get_instance( )
{
    if ( !m_instance )
    {
        uintptr_t const singleton_class = reinterpret_cast< uintptr_t >( g_ctx->il2cpp->get_class( ay_obfuscate( "Axlebolt.Standoff.Main.Inventory" ), ay_obfuscate( "InventoryManager" ) ) );

        if ( !singleton_class )
            return 0;

        m_instance = reinterpret_cast< uintptr_t ( * )( uintptr_t, const char* ) >( g_ctx->il2cpp->functions.class_get_field_from_name )( singleton_class, ay_obfuscate( "_instance" ) );
    }

    c_inventory_manager* ret = nullptr;
    using field_static_get_value_fn = void ( * )( uintptr_t, void** );
    auto fn = reinterpret_cast< field_static_get_value_fn >( g_ctx->il2cpp->functions.field_static_get_value );
    fn( m_instance, reinterpret_cast< void** >( &ret ) );

    return ret;
}

unity::array< uintptr_t >* c_inventory_manager::get_definitions( )
{
    static auto fn = reinterpret_cast< unity::array< uintptr_t >* ( * ) ( c_inventory_manager* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Main.Inventory" ), ay_obfuscate( "InventoryManager" ), ay_obfuscate( "GetInventoryItemDefinitions" ), 0 ) );
    return fn( this );
}

unity::array< uintptr_t >* c_inventory_manager::get_skins( )
{
    static auto fn = reinterpret_cast< unity::array< uintptr_t >* ( * ) ( c_inventory_manager* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Main.Inventory" ), ay_obfuscate( "InventoryManager" ), ay_obfuscate( "GetInventoryItems" ), 0 ) );
    return fn( this );
}
