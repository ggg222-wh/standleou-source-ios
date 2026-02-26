#include "chat_manager.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"
#include "game/sdk/unity/unity.hpp"

#include "game/sdk/axlebolt/photon_player/photon_player.hpp"

#include "globals.hpp"

uintptr_t c_chat_manager::m_instance { 0 };

c_chat_manager* c_chat_manager::get_instance( )
{
    if ( !m_instance )
    {
        uintptr_t const scene_class = reinterpret_cast< uintptr_t >( g_ctx->il2cpp->get_class( ay_obfuscate( "Axlebolt.Standoff.Game" ), ay_obfuscate( "ChatManager" ) ) );

        if ( !scene_class )
            return 0;

        m_instance = reinterpret_cast< uintptr_t ( * )( uintptr_t, const char* ) >( g_ctx->il2cpp->functions.class_get_field_from_name )( scene_class, ay_obfuscate( "_instance" ) );
    }

    c_chat_manager* ret = nullptr;
    using field_static_get_value_fn = void ( * )( uintptr_t, void** );
    auto fn = reinterpret_cast< field_static_get_value_fn >( g_ctx->il2cpp->functions.field_static_get_value );
    fn( m_instance, reinterpret_cast< void** >( &ret ) );

    return ret;
}

void c_chat_manager::send_message( unity::string* msg )
{
    static auto fn = reinterpret_cast< void ( * )( c_chat_manager*, unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Game" ), ay_obfuscate( "ChatManager" ), ay_obfuscate( "SendToAll" ), 1 ) );
    fn( this, msg );
}

void c_chat_manager::send_team( unity::string* msg )
{
    static auto fn = reinterpret_cast< void ( * )( c_chat_manager*, unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Game" ), ay_obfuscate( "ChatManager" ), ay_obfuscate( "SendToTeam" ), 1 ) );
    fn( this, msg );
}
