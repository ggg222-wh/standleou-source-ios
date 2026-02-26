//
// Created by Kai Tears on 16/08/2025.
//

#ifndef MODULES_HPP
#define MODULES_HPP

#include "../widgets.hpp"

class c_wg_modules : public c_base_widget
{
    std::vector< std::pair< bool*, std::pair< std::string, std::string > > > m_captured_modules;

public:
    ~c_wg_modules( ) override = default;
    explicit c_wg_modules( const ImVec2& pos, bool* active, const std::vector< std::pair< bool*, std::pair< std::string, std::string > > >& captured )
        : c_base_widget( pos )
        , m_active( active )
        , m_captured_modules( captured )
    {
    }

    bool* m_active;

    bool m_push_pos = false;

    void render( ) override;

    void render_module( const ImVec2& pos, const float& width, const std::pair< bool*, std::pair< std::string, std::string > >& module );

    void update_modules( );
};

#endif // MODULES_HPP
