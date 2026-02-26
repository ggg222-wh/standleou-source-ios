#pragma once

#include "globals.hpp"

#include "../unity.hpp"

class c_material;

class c_renderer
{
public:
    c_material* get_material( );
    unity::array< c_material* >* get_materials( );
    void set_materials( unity::array< c_material* >* materials );
    void set_material( c_material* material );
};