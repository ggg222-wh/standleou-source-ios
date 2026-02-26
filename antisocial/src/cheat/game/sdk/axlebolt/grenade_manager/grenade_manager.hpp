#pragma once

#include <vector>

struct vec3_t;

class c_grenade_manager
{
    static uintptr_t m_instance;

public:
    static c_grenade_manager* get_instance( );

    void throw_grenade( int id, vec3_t start, vec3_t direction, float g, int w );
};