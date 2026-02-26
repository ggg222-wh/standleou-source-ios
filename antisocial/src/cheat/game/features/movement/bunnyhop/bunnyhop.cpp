#include "bunnyhop.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"

#include "game/sdk/structures/obscured.hpp"

#include "globals.hpp"

void c_bunnyhop::execute( c_player_controller* entity ) const
{
    if ( !entity || !entity->alive( ) )
        return;

    uintptr_t const movement_controller = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( entity ) + 0xA8 );
    if ( !movement_controller )
        return;

    uintptr_t const player_translation_parameters = *reinterpret_cast< uintptr_t* >( movement_controller + 0x70 );
    if ( !player_translation_parameters )
        return;

    uintptr_t const player_translation_data = *reinterpret_cast< uintptr_t* >( movement_controller + 0x78 );
    if ( !player_translation_data )
        return;

    uintptr_t const jump_param = *reinterpret_cast< uintptr_t* >( player_translation_parameters + 0x50 );

    static float original_speed { 0 };
    static bool update { false };

    float& jump_speed = *reinterpret_cast< float* >( jump_param + 0x58 );

    if ( original_speed <= 0 )
        original_speed = jump_speed;

    if ( c::get< bool >( g_ctx->cfg.movement_bunnyhop ) )
    {
        *reinterpret_cast< vec3_t* >( player_translation_data + 0x68 ) = vec3_t { };
        jump_speed = 6.f;

        *reinterpret_cast< float* >( player_translation_parameters + 0x28 ) = 999.f;
        auto const jump_ptr = reinterpret_cast< uintptr_t >( jump_param );
        obscured_float_t::set( jump_param + 0xA8, 7.9f );

        update = true;
    } else
    {
        if ( update )
        {
            jump_speed = original_speed;
            update = false;
        }
    }
}
