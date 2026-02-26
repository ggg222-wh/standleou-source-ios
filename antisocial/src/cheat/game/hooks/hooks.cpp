#include "hooks.hpp"

#include "player_controller/late_update/late_update.hpp"
#include "hit_caster/cast_hit/cast_hit.hpp"
#include "player_inputs/get_inputs/get_inputs.hpp"
#include "player_hit_controller/hit/hit.hpp"
#include "camera_scope_zoomer/zoom/zoom.hpp"
#include "aim_view/draw_sniper/draw_sniper.hpp"
#include "grenade_manager/grenade_hooks/grenade_hooks.hpp"
#include "weapon_drop_manager/ctor/ctor.hpp"
#include "chat_manager/message_received/message_received.hpp"
#include "chat_manager/send_to_all/send_to_all.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

void log_shit( const std::string& text )
{
    char* copy = strdup( text.c_str( ) );
    FILE* f = fopen( std::string( g_ctx->documents_directory + "/servers.txt" ).c_str( ), "a+" );
    if ( f )
    {
        fputs( copy, f );
        fputc( '\n', f );
        fclose( f );
    }
    free( copy );
}

unity::array< uintptr_t >* ( *og_servers )( );
unity::array< uintptr_t >* hk_servers( )
{
    auto* const ret = og_servers( );

    if ( ret )
    {
        auto const vec = ret->to_cpp_array( );
        for ( auto const& it : vec )
        {
            if ( !it )
                continue;

            auto* const dns = *reinterpret_cast< unity::string** >( it + 0x10 );
            auto* const location = *reinterpret_cast< unity::string** >( it + 0x18 );
            auto* const display_name = *reinterpret_cast< unity::string** >( it + 0x20 );
            auto* const ip = *reinterpret_cast< unity::string** >( it + 0x28 );

            log_shit( dns->to_cpp_string( ) );
            log_shit( location->to_cpp_string( ) );
            log_shit( display_name->to_cpp_string( ) );
            log_shit( ip->to_cpp_string( ) );

            log_shit( crypt_string( "-----------" ) );
        }
    }

    return ret;
}

unity::array< uintptr_t >* ( *og_servers2 )( );
unity::array< uintptr_t >* hk_servers2( )
{
    auto* const ret = og_servers2( );

    if ( ret )
    {
        auto const vec = ret->to_cpp_array( );
        for ( auto const& it : vec )
        {
            if ( !it )
                continue;

            auto* const dns = *reinterpret_cast< unity::string** >( it + 0x10 );
            auto* const location = *reinterpret_cast< unity::string** >( it + 0x18 );
            auto* const display_name = *reinterpret_cast< unity::string** >( it + 0x20 );
            auto* const ip = *reinterpret_cast< unity::string** >( it + 0x28 );

            log_shit( dns->to_cpp_string( ) );
            log_shit( location->to_cpp_string( ) );
            log_shit( display_name->to_cpp_string( ) );
            log_shit( ip->to_cpp_string( ) );

            log_shit( crypt_string( "-----------" ) );
        }
    }

    return ret;
}

void c_hooks::hook( ) const
{
    hk::player_controller::late_update::hook( );
    hk::hit_caster::cast_hit::hook( );
    hk::player_inputs::get_inputs::hook( );
    hk::camera_scope_zoomer::zoom::hook( );
    hk::aim_view::draw_sniper::hook( );
    hk::player_hit_controller::hit::hook( );
    hk::grenade_manager::grenade_hooks::hook( );
    hk::weapon_drop_manager::ctor::hook( );
    hk::chat_manager::message_received::hook( );
    hk::chat_manager::send_to_all::hook( );

    memory_manager::hook( 0x17CB168, reinterpret_cast< void* >( hk_servers ), reinterpret_cast< void** >( &og_servers ) );
    memory_manager::hook( 0x17CB9EC, reinterpret_cast< void* >( hk_servers2 ), reinterpret_cast< void** >( &og_servers2 ) );
}

void c_hooks::unhook( ) const
{
    // TODO, or does someone give a fuck actually?
}