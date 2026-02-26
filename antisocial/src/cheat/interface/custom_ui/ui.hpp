//
// Created by Kai Tears on 22/07/2025.
//

#ifndef UI_HPP
#define UI_HPP

#include "advanced/utilities.hpp"

#include "texture_manager/texture_manager.hpp"

struct preview_model_t
{
    int w;
    int h;

    texture_data_t body;
    texture_data_t head;
    texture_data_t arms;
    texture_data_t legs;
};

struct preview_base_t
{
    texture_data_t background;
};

struct ui_tab_t
{
    std::string str_name;
    int default_subtab;

    c_animator c_standalone { float( 0.f ) };

    c_animator c_background { ImVec4( 0, 0, 0, 0 ) };
    c_animator c_outline { ImVec4( 0, 0, 0, 0 ) };
    c_animator c_text { ImVec4( 0, 0, 0, 0 ) };
};

struct ui_subtab_t
{
    std::string str_name;
    int parent_tab;

    c_animator c_standalone { float( 0.f ) };

    c_animator c_background { ImVec4( 0, 0, 0, 0 ) };
    c_animator c_outline { ImVec4( 0, 0, 0, 0 ) };
    c_animator c_text { ImVec4( 0, 0, 0, 0 ) };
};

class c_user_interface
{
    std::vector< ui_tab_t > m_tabs;
    std::vector< ui_subtab_t > m_subtabs;

public:
    ~c_user_interface( ) = default;
    c_user_interface( ) = default;

    static preview_model_t m_ayanami_rei;
    static preview_model_t m_stella;

    static preview_base_t m_preview_base;

    int m_current { 0 };
    int m_sub { 0 };

    void initialize( float scale );
    void render( );
};

extern c_user_interface g_user_interface;

#endif // UI_HPP
