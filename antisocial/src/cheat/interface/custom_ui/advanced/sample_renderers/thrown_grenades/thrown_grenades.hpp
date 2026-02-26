#pragma once

#include "imgui.h"

#include "texture_manager/texture_manager.hpp"

class c_sample_thrown_grenades
{
public:
    static texture_data_t m_texture;

    static void render( ImDrawList* draw_list, const ImVec2& center );
};