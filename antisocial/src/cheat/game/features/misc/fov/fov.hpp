#pragma once

#include "singleton.hpp"

class c_fov : public singleton< c_fov >
{
public:
    ~c_fov( ) = default;
    c_fov( ) = default;

    void execute( ) const;
};