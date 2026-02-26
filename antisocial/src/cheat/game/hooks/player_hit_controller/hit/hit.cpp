#include "hit.hpp"

#include <vector>
#include <string>
#include <random>
#include <iostream>

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"
#include "audio_player/audio_player.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/features/visual/hit_points/hit_points.hpp"
#include "game/features/visual/hit_markers/hit_markers.hpp"
#include "game/features/visual/particles/particles.hpp"
#include "game/sdk/axlebolt/chat_manager/chat_manager.hpp"
#include "game/sdk/unity/type/type.hpp"

#include "imgui.h"

#include "../../il2cpp_hook.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

std::vector< std::string > talks = {
    "ghfdbkf",
    "im win if im want. my spinbot activated no chance for enemy",
    "When I'm play MM I'm play for win, don't scare for spin, i inject rage",
    "The global elite Master Soon TM",
    "(I dont care for smoke, i am not main)",
    "All Family in novo",
    "(You awall first? Ok lets fun)",
    "I dont care for smoke, i am not main",
    "I am hvh winner steam level 11 and shadow daggers rust coat coming soon",
    "Link main before speaking",
    "The only crime in hvh is to lose",
    "When he yes cheat i using nix ware and own himself",
    "When not use nxi war i use epsdx",
    "Your mom is an object to speak about in group of 70 years old men",
    "While you trying to say something to me, i eat my 9.99$ taco and don't give a fuck",
    "Link main before speaking",
    "When i play against nigger i kill him every round",
    "Everyone is my evil when i do never lose",
    "While you try to kill me once i own you 10 maps in a row",
    "I shoot russian pigs with my nato bruddas and then own everyone in MM on red trust" };

void hk::player_hit_controller::hit::hook( )
{
    memory_manager::hook( ( 0x17A8B28 ), reinterpret_cast< void* >( hk_hit ), reinterpret_cast< void** >( &og_hit ) );
}

void hk::player_hit_controller::hit::hk_hit( uintptr_t instance, uintptr_t data, uintptr_t controller, int a, int b )
{
    og_hit( instance, data, controller, a, b );

    unity::array< uintptr_t >* const hits = *reinterpret_cast< unity::array< uintptr_t >** >( data + 0x38 );
    if ( hits )
    {
        uintptr_t const first_hit = hits->get_ptr( )[ 0 ];
        if ( first_hit )
        {
            c_player_controller* const player = *reinterpret_cast< c_player_controller** >( instance + 0x60 );

            if ( !player )
                return;

            vec3_t const hit_location = *reinterpret_cast< vec3_t* >( first_hit + 0x1C );
            int const damage = *reinterpret_cast< int* >( first_hit + 0x2C );

            if ( c::get< bool >( g_ctx->cfg.visual_hit_particles_enabled ) )
                g_ctx->features.particles->hit_effect( hit_location );

            if ( c::get< bool >( g_ctx->cfg.visual_hit_points ) )
                g_ctx->features.hit_points->push_point( hit_point_t { hit_location } );

            if ( c::get< bool >( g_ctx->cfg.visual_hit_markers ) )
                g_ctx->features.hit_markers->push_marker( hit_marker_t { hit_location, damage } );

            if ( c::get< bool >( g_ctx->cfg.misc_hit_sound ) )
                g_ctx->audio_player->play( static_cast< int >( sound_t::hitsound ) );

            if ( c::get< bool >( g_ctx->cfg.misc_trash_talk ) && ( damage > player->get_health( ) ) )
            {
                std::random_device rd;
                std::mt19937 gen( rd( ) );
                std::uniform_int_distribution< size_t > dist( 0, talks.size( ) - 1 );

                std::string random_talk = talks[ dist( gen ) ];

                auto* const instance = c_chat_manager::get_instance( );
                if ( !instance )
                    return;

                instance->send_message( unity::string::create( random_talk ) );
            }
        }
    }
}