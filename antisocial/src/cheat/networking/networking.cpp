#include "networking.hpp"

#include <curl/curl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <fstream>

#include "globals.hpp"

size_t write_callback( void* contents, size_t size, size_t nmemb, void* userp )
{
    ( ( std::string* ) userp )->append( ( char* ) contents, size * nmemb );
    return size * nmemb;
}

std::unique_ptr< network::c_requests > network::g_requests = std::make_unique< network::c_requests >( );

network::c_requests* network::requests( )
{
    return g_requests.get( );
}

std::string base64_urlsafe( const std::string& input )
{
    static const char* b64_chars = ay_obfuscate( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_" );
    std::string encoded;
    int val = 0, valb = -6;

    for ( unsigned char c : input )
    {
        val = ( val << 8 ) + c;
        valb += 8;
        while ( valb >= 0 )
        {
            encoded.push_back( b64_chars[ ( val >> valb ) & 0x3F ] );
            valb -= 6;
        }
    }

    if ( valb > -6 )
        encoded.push_back( b64_chars[ ( ( val << 8 ) >> ( valb + 8 ) ) & 0x3F ] );

    return encoded;
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) )
std::string
network::c_requests::auth( const std::string& key, const std::string& hwid ) const
{
    std::string url = crypt_string( "http://62.60.249.46:80/api/check_key?key=" );
    url += key;
    url += crypt_string( "&hwid=" );
    url += hwid;

    std::string buffer;
    CURL* curl = curl_easy_init( );
    if ( curl )
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
        struct curl_slist* headers = NULL;
        headers = curl_slist_append( headers, ay_obfuscate( "Content-Type: application/x-www-form-urlencoded" ) );
        curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
        curl_easy_setopt( curl, CURLOPT_VERBOSE, 0L );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );

        CURLcode res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
        curl_slist_free_all( headers );
    }

    return buffer;
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) )
std::string
network::c_requests::end_point( const std::string& loc ) const
{
    std::string url = crypt_string( "http://62.60.249.46:80/" );
    url += loc;

    debug_log( url.c_str( ) );

    std::string buffer;
    CURL* curl = curl_easy_init( );
    if ( curl )
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
        struct curl_slist* headers = NULL;
        headers = curl_slist_append( headers, ay_obfuscate( "Content-Type: application/x-www-form-urlencoded" ) );
        curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
        curl_easy_setopt( curl, CURLOPT_VERBOSE, 0L );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );

        CURLcode res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
        curl_slist_free_all( headers );
    }

    return buffer;
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) )
std::string
network::c_requests::post_base64(
    const std::string& path,
    const std::string& base64_body,
    const std::map< std::string, std::string >& query ) const
{
    std::string url = crypt_string( "http://62.60.249.46:80/" ) + path;
    if ( !query.empty( ) )
    {
        url += "?";
        bool first = true;
        for ( auto const& [ k, v ] : query )
        {
            if ( !first )
                url += "&";
            url += k + "=" + v;
            first = false;
        }
    }

    std::string buffer;
    CURL* curl = curl_easy_init( );
    if ( curl )
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_POST, 1L );
        curl_easy_setopt( curl, CURLOPT_POSTFIELDS, base64_body.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, base64_body.size( ) );

        struct curl_slist* headers = NULL;
        headers = curl_slist_append( headers, "Content-Type: text/plain" );
        curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );

        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );

        CURLcode res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
        curl_slist_free_all( headers );
    }

    return buffer;
}

__attribute__( ( annotate( "strenc" ) ) ) bool
network::c_requests::check_network( const std::string& port ) const
{
    std::string url = crypt_string( "http://62.60.249.46:" );
    url += port;
    url += crypt_string( "/" );

    std::string buffer;
    CURL* curl = curl_easy_init( );
    if ( curl )
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
        struct curl_slist* headers = NULL;
        headers = curl_slist_append( headers, ay_obfuscate( "Content-Type: application/x-www-form-urlencoded" ) );
        curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
        curl_easy_setopt( curl, CURLOPT_VERBOSE, 0L );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );

        CURLcode res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
        curl_slist_free_all( headers );
    }

    bool match = buffer.find( crypt_string( "Not Found" ) ) != std::string::npos;

    return match;
}

__attribute__( ( annotate( "strenc" ) ) ) void
network::c_requests::download_file( const std::string& url, const std::string& output )
{
    std::string buffer;
    CURL* curl;

    curl = curl_easy_init( );
    if ( curl )
    {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
        struct curl_slist* headers = NULL;
        headers = curl_slist_append( headers, ay_obfuscate( "Content-Type: application/x-www-form-urlencoded" ) );
        curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
        curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
        curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );

        CURLcode res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
        curl_slist_free_all( headers );
    }

    if ( !buffer.empty( ) )
    {
        std::ofstream file( output, std::ios::binary );
        if ( file )
        {
            file.write( buffer.data( ), buffer.size( ) );
            file.close( );
        }
    }
}
