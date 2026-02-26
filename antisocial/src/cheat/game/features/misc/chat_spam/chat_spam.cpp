#include "chat_spam.hpp"

#include "game/sdk/axlebolt/chat_manager/chat_manager.hpp"

#include "globals.hpp"

void c_chat_spam::execute( ) const
{
    if ( !c::get< bool >( g_ctx->cfg.misc_chat_spam ) && g_ctx->user_data.premium )
        return;

    auto* const instance = c_chat_manager::get_instance( );
    if ( !instance )
        return;

    static int passed_frames { 0 };
    passed_frames++;
    if ( passed_frames < 5 )
        return;

    instance->send_message( unity::string::create( ay_obfuscate( "t.me/antisocial_pub - ЛУЧШИЙ БЕСПЛАТНЫЙ ЧИТ" ) ) );
    passed_frames = 0;
}