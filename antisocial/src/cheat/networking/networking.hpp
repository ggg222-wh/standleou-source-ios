#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace network
{
    class c_requests
    {
    public:
        std::string m_user_key { };

        std::string end_point( const std::string& loc ) const;
        std::string post_base64( const std::string& path, const std::string& base64_body, const std::map< std::string, std::string >& query = { } ) const;

        std::string auth( const std::string& key, const std::string& hwid ) const;

        void download_file( const std::string& loc, const std::string& output );

        bool check_network( const std::string& port ) const;

        std::string get_hardware( ) const; // defined in objc
    };

    extern std::unique_ptr< c_requests > g_requests;

    extern c_requests* requests( );
} // namespace network