#pragma once

#include "singleton.hpp"

struct vec3_t;

class c_nuke : public singleton< c_nuke >
{
public:
    ~c_nuke( ) = default;
    c_nuke( ) = default;

    void fire( vec3_t start, vec3_t direction, int grenade ) const;
};