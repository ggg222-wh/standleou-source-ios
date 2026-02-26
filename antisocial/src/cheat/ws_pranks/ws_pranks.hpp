#pragma once

#include "singleton.hpp"

#include "interface/custom_ui/advanced/utilities.hpp"
#include "texture_manager/texture_manager.hpp"

enum class prank_type
{
    jigsaw = 0,
    kaban,
    troll,
    plankton
};

class c_ws_pranks : public singleton< c_ws_pranks >
{
    bool m_initialized { false };

    c_animator m_alpha { float( 0.f ) };
    prank_type m_type { };

public:
    ~c_ws_pranks( ) = default;
    c_ws_pranks( ) = default;

    static texture_data_t m_jigsaw_tex;
    static texture_data_t m_kaban_tex;
    static texture_data_t m_plankton_tex;
    static texture_data_t m_troll_tex;

    void init( );

    void prank( const prank_type& type );

    void render( );
};