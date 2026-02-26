#include "ctor.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

#include "game/entities/entities.hpp"

void hk::weapon_drop_manager::ctor::hook( )
{
    memory_manager::hook( ( 0x182C8CC ), reinterpret_cast< void* >( hk_ctor ), reinterpret_cast< void** >( &og_ctor ) );
}

void hk::weapon_drop_manager::ctor::hk_ctor( uintptr_t instance )
{
    og_ctor( instance );

    c_dropped_items_database::get( )->m_weapons_instance = instance;
}