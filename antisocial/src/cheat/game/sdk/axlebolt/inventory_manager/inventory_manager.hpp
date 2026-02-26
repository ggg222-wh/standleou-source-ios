#pragma once

#include <cstdint>

#include "../../unity/unity.hpp"

class c_inventory_manager
{
    static uintptr_t m_instance;

public:
    static c_inventory_manager* get_instance( );

    unity::array< uintptr_t >* get_definitions( );
    unity::array< uintptr_t >* get_skins( );
};