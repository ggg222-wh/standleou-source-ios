#pragma once

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class base64
{
private:
    static const char base64_encoding_table[ 64 ];
    static const int base64_decoding_table[ 131 ];

    static int binary_string_to_decimal( std::string binary_string );
    static bool is_string_ascii( std::string message );

public:
    static bool is_number( const std::string input );
    static std::string line_splitting( std::string base64_message, int length );

    static std::string base64_encode( std::string message );
    static std::string base64_decode( std::string message );
};
