#pragma once

#include "../../structures/vec3.hpp"
#include "../../structures/euler.hpp"

class c_transform
{
public:
    vec3_t get_position( );
    vec3_t get_forward( );
    vec3_t get_up( );
    void set_position( vec3_t position );
    void set_local_euler( euler_angles_t rotation );
    void set_euler( euler_angles_t rotation );
};