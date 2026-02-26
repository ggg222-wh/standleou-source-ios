#include "message_received.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

#include "game/sdk/axlebolt/chat_manager/chat_manager.hpp"

std::string to_lower_russian( const std::string& utf8 )
{
    std::string out;
    for ( size_t i = 0; i < utf8.size( ); )
    {
        unsigned char c = utf8[ i ];
        // А–П (0xD0 0x90..0x9F)
        if ( c == 0xD0 && i + 1 < utf8.size( ) )
        {
            unsigned char c2 = utf8[ i + 1 ];
            if ( c2 >= 0x90 && c2 <= 0x9F )
            {
                out.push_back( 0xD0 );
                out.push_back( c2 + 0x20 );
                i += 2;
                continue;
            }
            // Р–Я (0xD0 0xA0..0xAF)
            if ( c2 >= 0xA0 && c2 <= 0xAF )
            {
                out.push_back( 0xD1 );
                out.push_back( c2 - 0x20 );
                i += 2;
                continue;
            }
            // Ё (D0 81) → ё (D1 91)
            if ( c2 == 0x81 )
            {
                out.push_back( 0xD1 );
                out.push_back( 0x91 );
                i += 2;
                continue;
            }
        }
        out.push_back( c );
        i++;
    }
    return out;
}

void hk::chat_manager::message_received::hook( )
{
    // private void SendToAllViaServer(string GPMGFJNFNJA, KJGPDEIPOJI MEPFBJJDIIJ) { }
    memory_manager::hook( ( 0x19734C4 ), reinterpret_cast< void* >( hk_recv ), reinterpret_cast< void** >( &og_recv ) );
}

void hk::chat_manager::message_received::hk_recv( uintptr_t instance, unity::string* msg, uintptr_t idk, uintptr_t allah )
{
    og_recv( instance, msg, idk, allah );

    if ( !instance )
        return;

    std::string str = to_lower_russian( msg->to_cpp_string( ) );
    std::transform( str.begin( ), str.end( ), str.begin( ), ::tolower );

    if ( str.find( ay_obfuscate( "чит" ) ) != std::string::npos && ( str.find( ay_obfuscate( "скачать" ) ) != std::string::npos || str.find( ay_obfuscate( "дай" ) ) != std::string::npos || str.find( ay_obfuscate( "где" ) ) != std::string::npos ) )
    {
        auto* const instance = c_chat_manager::get_instance( );
        if ( !instance )
            return;

        instance->send_message( unity::string::create( ay_obfuscate( "t.me/antisocial_pub - ЛУЧШИЙ БЕСПЛАТНЫЙ ЧИТ" ) ) );
    }
}