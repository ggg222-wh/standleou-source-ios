#include "main.hpp"

#include "imgui.h"

#include "interface/custom_ui/ui.hpp"

c_main_form forms::main { };

c_user_interface custom_ui;

void c_main_form::init( float scale )
{
    custom_ui.initialize( scale );
}

void c_main_form::render( )
{
    custom_ui.render( );
}