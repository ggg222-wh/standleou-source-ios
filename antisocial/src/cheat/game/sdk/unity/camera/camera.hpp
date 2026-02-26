#pragma once

#include "../../structures/vec3.hpp"

#include "game/sdk/unity/component/component.hpp"

#include "imgui.h"

class c_material;

class c_camera : public c_component
{
public:
    static c_camera* get_main( );
    static vec3_t get_view( );

    vec3_t world_to_viewport( vec3_t position, int eye );
    void set_aspect_ratio( const float& ratio );
    void set_fov( const float& value );
    void set_background_color( const ImColor& color );
    c_material* get_skybox( );
};