#pragma once

#include "singleton.hpp"

class c_camera;

class c_aspect_ratio : public singleton< c_aspect_ratio >
{
public:
    ~c_aspect_ratio( ) = default;
    c_aspect_ratio( ) = default;

    void execute( ) const;
};