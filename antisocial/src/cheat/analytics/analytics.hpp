#pragma once

#include "singleton.hpp"

#include <map>
#include <atomic>

class c_analytics : public singleton< c_analytics >
{
    std::atomic< bool > m_lock;

public:
    ~c_analytics( ) = default;
    c_analytics( ) = default;

    void i_am_cheater( const std::string& uid );
    void who_is_cheater( );

    bool is_he_cheater( const std::string& uid );
};