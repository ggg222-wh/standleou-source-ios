#pragma once

#include "globals.hpp"

#include "../unity.hpp"

struct ImColor;

class c_material
{
public:
    static c_material* create( const char* shader );

    uintptr_t get_texture( unity::string* name );
    void set_texture( unity::string* name, uintptr_t texture );
    void set_shader( uintptr_t shader );
    bool has_property( unity::string* property );
    void set_color( unity::string* name, const ImColor& color );
    void set_int( unity::string* name, const int& value );
    void ctor( uintptr_t shader );
};