#pragma once

#include <string>
#include <functional>

namespace alert
{
    void create( std::string alertTitle, std::string okActionName, std::string placeholderName, std::string& ret );
    void create_with_task( std::string alertTitle, std::string okActionName, std::string placeholderName, std::function< void( const std::string& key ) > task );
    void show_error( std::string alertTitle, std::string description );
} // namespace alert