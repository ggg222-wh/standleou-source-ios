#pragma once

#include "game/sdk/unity/unity.hpp"

class c_system
{
public:
    static unity::array< unsigned char >* file_to_bytes( unity::string* path );
};