#pragma once

#include "singleton.hpp"

#include <any>
#include <string>

class c_player_controller;

class c_prop_setter : public singleton< c_prop_setter >
{
public:
    bool m_pending = false;
    std::string m_key;
    std::any m_value;

public:
    ~c_prop_setter( ) = default;
    c_prop_setter( ) = default;

    template < typename T >
    void push_prop( const std::string& key, T&& value )
    {
        m_key = key;
        m_value = std::any( std::forward< T >( value ) );
        m_pending = true;
    }

    void execute( c_player_controller* local );
};
