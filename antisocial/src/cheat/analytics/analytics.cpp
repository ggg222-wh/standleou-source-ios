#include "analytics.hpp"

#include "networking/networking.hpp"

#include "globals.hpp"

#include <thread>

void c_analytics::i_am_cheater( const std::string& uid )
{
    std::thread( [ uid ] -> void {
        network::requests( )->end_point(
            crypt_string( "api/im_cheater?key=" ) + network::requests( )->m_user_key +
            crypt_string( "&game_id+" ) + g_ctx->user_data.telegram_id );
    } );
}

void c_analytics::who_is_cheater( )
{
    m_lock.store( true );

    std::thread( [ this ] -> void {
        auto const cheaters = network::requests( )->end_point(
            crypt_string( "api/im_cheater?key=" ) + network::requests( )->m_user_key +
            crypt_string( "&game_id+" ) + g_ctx->user_data.telegram_id );

        m_lock.store( false );
    } );
}

bool c_analytics::is_he_cheater( const std::string& uid )
{
    if ( !m_lock.load( ) )
        return;
}