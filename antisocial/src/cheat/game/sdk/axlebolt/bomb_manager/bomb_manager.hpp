#pragma once

#include <cstdint>

class c_bomb_manager
{
    static uintptr_t m_instance;

public:
    static c_bomb_manager* get_instance( );
};