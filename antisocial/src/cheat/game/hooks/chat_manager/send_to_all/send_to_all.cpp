#include "send_to_all.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

#include "game/sdk/axlebolt/chat_manager/chat_manager.hpp"

#include "chat/chat.hpp"

void hk::chat_manager::send_to_all::hook( )
{
    memory_manager::hook( ( 0x19732EC ), reinterpret_cast< void* >( hk_send ), reinterpret_cast< void** >( &og_send ) );
}

void hk::chat_manager::send_to_all::hk_send( uintptr_t instance, unity::string* msg )
{
    auto const str = msg->to_cpp_string( );
    if ( !str.empty( ) && str[ 0 ] == '@' && c::get< bool >( g_ctx->cfg.misc_irc_chat ) )
    {
        g_ctx->chat->send_message( "[" + g_ctx->user_data.username + "] " + str.substr( 1 ) );
        return;
    }

    og_send( instance, msg );
}