//
// Created by Kai Tears on 16/08/2025.
//

#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include "imgui.h"
#include "imgui_internal.h"

#include <unordered_map>
#include <string>
#include <utility>
#include <vector>

#include "interface/custom_ui/advanced/utilities.hpp"

#include "singleton.hpp"

class c_widgets : public singleton< c_widgets >
{
public:
    ~c_widgets( ) = default;
    c_widgets( ) = default;

    static bool m_allowed_edit;

    void render( bool allow_edit ) const;

    void update( );
    void save( );
};

class c_base_widget
{
public:
    virtual ~c_base_widget( ) = default;
    explicit c_base_widget( const ImVec2& pos )
        : m_pos( pos )
    {
    }

    ImVec2 m_pos = { 20, 20 };

    virtual void render( ) = 0;

    void render_activator( const ImVec2& pos, const std::string& id, bool* active ) const;
    void render_title( const ImRect& rect, const std::string& title ) const;
    [[nodiscard]] ImRect generate_rect( const float& width, const float& height ) const;
};

#endif // WIDGETS_HPP
