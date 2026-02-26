#pragma once

#include <cstdint>
#include <string>

#include "game/sdk/unity/unity.hpp"

class c_texture
{
public:
    static c_texture* load_from_file( const std::string& path );

    bool image_from_bytes( unity::array< unsigned char >* data );
    void ctor( int width, int height, int a, bool b );
};