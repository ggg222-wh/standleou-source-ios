#pragma once

#include "forms/main/main.hpp"

#include "singleton.hpp"

#include <vector>
#include <functional>

class c_interface : public singleton< c_interface >
{
public:
    std::function< void( ) > m_initializer;
    std::vector< std::function< void( ) > > m_queue;

    void render( );
    void init( float scale );
    void show_loading_screen( );

    ~c_interface( ) = default;
    c_interface( ) = default;
};