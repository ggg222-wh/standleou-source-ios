#pragma once

#include "singleton.hpp"

#include <map>

class c_config : public singleton< c_config >
{
    std::map< int, std::string > m_configs { };
    bool m_pending_refresh { true };

public:
    std::string to_string( ) const;
    void load_from_string( const std::string& data ) const;

    void create( const std::string& name );
    void remove( const int& id );
    void load( const int& id );
    void update( const int& id );

    void on_gui( int child );
};