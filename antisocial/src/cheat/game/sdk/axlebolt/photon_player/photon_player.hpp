#pragma once

#include <string>

class c_photon_player
{
public:
    std::string get_nickname( );
    void set_clan_tag( const std::string& tag );
    void set_user_id( const std::string& uid );
    std::string get_uuid( );
    void add_score( int score );
    std::string get_id( );

    template < typename T >
    T get_property( const std::string& key );
    template < typename T >
    T get_property_boxed( const std::string& key );
};
