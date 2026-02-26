#pragma once

#include "singleton.hpp"

#include <map>
#include <string>

class c_chat : public singleton< c_chat >
{
    bool m_received { false };
    std::string m_message { };

    void ws_send( const std::string& text ) const; // objc

public:
    ~c_chat( ) = default;
    c_chat( ) = default;

    void receive_message( const std::string& message );
    void send_message( const std::string& message );

    void execute( );
};