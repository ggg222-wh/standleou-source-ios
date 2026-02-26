#include "chat.hpp"

#include "globals.hpp"

#include "game/sdk/axlebolt/chat_manager/chat_manager.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/axlebolt/player_controller/player_controller.hpp"

#include "game/sdk/unity/unity.hpp"

#include "memory_manager/memory_manager.hpp"

#include "game/entities/entities.hpp"

void c_chat::send_message( const std::string& message )
{
    ws_send( message );
}

void c_chat::receive_message( const std::string& message )
{
    m_message = message;
    m_received = true;
}

void c_chat::execute( )
{
    if ( !m_received )
        return;

    m_received = false;

    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
        return;

    auto* const instance = c_chat_manager::get_instance( );
    if ( !instance )
        return;

    auto* const sender = local->photon_player( );
    if ( !sender )
        return;

    instance->send_team( unity::string::create( crypt_string( "[irc] " ) + m_message ) );
}