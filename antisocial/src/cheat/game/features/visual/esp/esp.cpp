#include "esp.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/axlebolt/gun_controller/gun_controller.hpp"
#include "game/sdk/axlebolt/biped_map/biped_map.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/misc/math/math.hpp"
#include "game/sdk/axlebolt/bomb_manager/bomb_manager.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/ragebot/ragebot.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/component/component.hpp"

#include "game/entities/entities.hpp"

#include "dropped_weapons/dropped_weapons.hpp"
#include "grenades/grenades.hpp"
#include "planted_bomb/planted_bomb.hpp"
#include "players/players.hpp"

#include "globals.hpp"

#include <sstream>
#include <iomanip>
#include <array>

esp_fonts_t c_esp::m_fonts;

std::vector< std::pair< ImFont*, float > > c_esp::m_nickname_fonts;

void c_esp::process_dropped_weapons( ) const
{
    if ( !c_dropped_items_database::get( )->m_weapons.empty( ) && c::get< bool >( g_ctx->cfg.visual_dropped_weapons ) )
    {
        for ( auto const ent : c_dropped_items_database::get( )->m_weapons )
        {
            if ( ent )
                c_dropped_weapons_esp::execute( ent );
        }
    }
}

void c_esp::process_thrown_grenades( ) const
{
    if ( !c_dropped_items_database::get( )->m_grenades.empty( ) && c::get< bool >( g_ctx->cfg.visual_grenades_esp ) )
    {
        for ( auto const ent : c_dropped_items_database::get( )->m_grenades )
        {
            if ( ent )
                c_ent_grenade_esp::execute( ent );
        }
    }
}

void c_esp::process_ent_player( c_player_controller* entity ) const
{
    if ( !entity )
        return;

    auto const screen = math::world_to_screen( entity->get_position( ) );
    bool const offscreen = !screen.check;

    auto* const photon = entity->photon_player( );
    if ( !photon )
        return;

    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
        return;

    auto const uuid = photon->get_uuid( );
    auto& param = c_ent_player_esp::m_parameters[ uuid ];

    param.alpha.update< float >( entity->alive( ) ? ( photon->get_property< bool >( crypt_string( "untouchable" ) ) ? 0.5f : 1.f ) : 0.f );

    if ( c::get< bool >( g_ctx->cfg.esp_profile_pictures ) && entity->alive( ) )
    {
        if ( !offscreen )
        {
            auto const avatar_array = photon->get_property_boxed< unity::array< unsigned char >* >( crypt_string( "avatar" ) );
            if ( avatar_array && avatar_array->get_capacity( ) > 4 )
            {
                std::vector< unsigned char > avatar_data;
                avatar_data.reserve( avatar_array->get_capacity( ) - 4 );

                for ( int i = 4; i < avatar_array->get_capacity( ); i++ )
                    avatar_data.push_back( avatar_array->get_ptr( )[ i ] );

                if ( !c_ent_player_esp::m_avatars[ uuid ].texture_id || !c_ent_player_esp::m_avatars[ uuid ].loaded )
                    g_ctx->texture_manager->load( avatar_data.data( ), ( int ) avatar_data.size( ), &( c_ent_player_esp::m_avatars[ uuid ] ), uuid );

                if ( !c_ent_player_esp::m_gray_avatars[ uuid ].texture_id || !c_ent_player_esp::m_gray_avatars[ uuid ].loaded )
                    g_ctx->texture_manager->load_grayscale( avatar_data.data( ), ( int ) avatar_data.size( ), &( c_ent_player_esp::m_gray_avatars[ uuid ] ), uuid );
            }
        }
    } else
    {
        auto it = c_ent_player_esp::m_avatars.find( uuid );
        if ( it != c_ent_player_esp::m_avatars.end( ) )
        {
            if ( it->second.loaded )
                g_ctx->texture_manager->unload( &it->second );

            c_ent_player_esp::m_avatars.erase( it );
        }

        auto it_gray = c_ent_player_esp::m_gray_avatars.find( uuid );
        if ( it_gray != c_ent_player_esp::m_gray_avatars.end( ) )
        {
            if ( it_gray->second.loaded )
                g_ctx->texture_manager->unload( &it_gray->second );

            c_ent_player_esp::m_gray_avatars.erase( it_gray );
        }
    }

    auto const pfp_texture = ( c::get< bool >( g_ctx->cfg.esp_profile_pictures ) && c_ent_player_esp::m_avatars[ uuid ].loaded )
                                 ? reinterpret_cast< ImTextureID >( c_ent_player_esp::m_avatars[ uuid ].texture_id )
                                 : 0;

    auto const gray_pfp_texture = ( c::get< bool >( g_ctx->cfg.esp_profile_pictures ) && c_ent_player_esp::m_gray_avatars[ uuid ].loaded )
                                      ? reinterpret_cast< ImTextureID >( c_ent_player_esp::m_gray_avatars[ uuid ].texture_id )
                                      : 0;

    auto const bounds = math::calculate_player_ent_bbox( entity->get_position( ), 1.8f );

    auto const get_player_weapon_clip = []( c_player_controller* const ent ) -> std::pair< short, short > {
        if ( ent && ent->alive( ) )
        {
            auto* const weapon = ent->get_weapon( );
            if ( weapon && weapon->is_gun( weapon->get_id( ) ) )
            {
                auto* const gun = weapon->get_gun( );
                if ( gun )
                {
                    return { gun->magazine_capacity( ), weapon->get_data( ).max_clip };
                }
            }
        }
        return { 0, 0 };
    };

    auto const get_player_weapon_id = []( c_player_controller* const ent ) -> short {
        if ( ent && ent->alive( ) )
        {
            auto* const weapon = ent->get_weapon( );
            return weapon->get_id( );
        }
        return 0;
    };

    auto const is_ammo_available = []( c_player_controller* const ent ) -> short {
        if ( ent && ent->alive( ) )
        {
            auto* const weapon = ent->get_weapon( );
            if ( weapon && weapon->is_gun( weapon->get_id( ) ) )
            {
                auto* const gun = weapon->get_gun( );
                if ( gun )
                {
                    return true;
                }
            }
        }
        return false;
    };

    auto const clip_pair = get_player_weapon_clip( entity );

    ent_grenades_t grenades = {
        entity->has_weapon( 91 ),
        entity->has_weapon( 92 ),
        entity->has_weapon( 93 ) };

    esp_ent_t generated = {
        entity->get_position( ),
        bounds,
        uuid,
        photon->get_property< bool >( crypt_string( "untouchable" ) ),
        entity->get_health( ),
        clip_pair.first,
        clip_pair.second,
        get_player_weapon_id( entity ),
        photon->get_nickname( ),
        grenades };

    c_ent_player_esp esp( generated );

    if ( offscreen )
    {
        if ( c::get< bool >( g_ctx->cfg.esp_offscreen ) )
            esp.render_offscreen( );

        return;
    }

    [ & ]( ) -> void {
        if ( !c::get< bool >( g_ctx->cfg.esp_target ) || !c::get< bool >( g_ctx->cfg.ragebot_enabled ) )
            return;

        auto* const local = c_players_database::get( )->m_local_player;
        if ( !local )
            return;

        auto* const weapon = local->get_weapon( );
        if ( !weapon || !weapon->is_gun( weapon->get_id( ) ) )
            return;

        auto* const target = c_players_database::get( )->get_ragebot_entity( );
        if ( !target )
            return;

        auto* const biped = target->biped_map( );
        if ( !biped )
            return;

        c_transform* selected { };
        auto* const bone = g_ctx->features.ragebot->m_target;

        selected = ( !bone ) ? g_ctx->features.ragebot->select_bone( biped, g_ctx->features.thirdperson->get_unmodified_view( ), weapon->get_gun( ) ) : bone;

        if ( !selected )
            return;

        auto const position = selected->get_position( );
        auto const position_up = selected->get_position( ) + vec3_t( 0.f, 0.3f, 0.f );

        w2s_t const position_screen = math::world_to_screen( position );
        w2s_t const position_up_screen = math::world_to_screen( position_up );

        int const radius = static_cast< int >( ( ( ( position_screen.position.y - position_up_screen.position.y ) * 0.8 ) / 2 ) * 2.6f );
        esp.render_selected_target( position, ( radius > 1 ) ? radius : 2 );
    }( );

    if ( c::get< bool >( g_ctx->cfg.esp_rect ) )
        esp.render_rect( );

    // if ( c::get< bool >( g_ctx->cfg.esp_health ) )
    esp.render_healthbar( );

    // if ( c::get< bool >( g_ctx->cfg.esp_ammo ) )
    esp.render_ammobar( is_ammo_available( entity ) );

    // if ( c::get< bool >( g_ctx->cfg.esp_nickname ) )
    esp.render_nickname( pfp_texture, gray_pfp_texture );

    std::vector< esp_flag_t > temp_flags { };

    if ( c::get< bool >( g_ctx->cfg.esp_armor ) )
    {
        auto const armor = photon->get_property< int >( crypt_string( "armor" ) );
        auto const helmet = photon->get_property< bool >( crypt_string( "helmet" ) );

        if ( helmet || armor > 0 )
        {
            std::string armor_value { };

            if ( helmet )
                armor_value += crypt_string( "H" );

            if ( armor > 0 )
                armor_value += crypt_string( "K" );

            temp_flags.push_back( { armor_value, ImColor( 255, 255, 255 ) } );
        }
    }

    if ( c::get< bool >( g_ctx->cfg.esp_money ) )
    {
        auto const money = photon->get_property< int >( crypt_string( "money" ) );

        temp_flags.push_back( { std::to_string( money ) + "$", ImColor( 55, 200, 0 ) } );
    }

    if ( c::get< bool >( g_ctx->cfg.esp_scoped ) )
    {
        auto* const weapon = entity->get_weapon( );

        if ( weapon && weapon->is_gun( weapon->get_id( ) ) )
        {
            bool const scoped = weapon->scoped( );

            if ( scoped )
                temp_flags.push_back( { crypt_string( "ZOOM" ), ImColor( 25, 155, 255 ) } );
        }
    }

    auto const view = g_ctx->features.thirdperson->get_unmodified_view( );

    if ( c::get< bool >( g_ctx->cfg.esp_open ) && g_ctx->user_data.premium )
    {
        if ( c_camera::get_main( ) )
        {
            auto const target_position = entity->get_position( ) + vec3_t( 0, entity->crouching( ) ? 1.3f : 1.6f, 0 );

            auto const view_camera = c_camera::get_main( )->get_transform( )->get_position( );

            bool const visible_from_camera = !c_physics::linecast( view_camera, target_position, 16384 );
            bool visible_from_view = !c_physics::linecast( view, target_position, 16384 );

            for ( float offset = 0.5f; offset <= 3.0f; offset += 0.5f )
            {
                vec3_t const left_eye = view - vec3_t( offset, 0.f, 0.f );
                vec3_t const right_eye = view + vec3_t( offset, 0.f, 0.f );

                if ( !c_physics::linecast( left_eye, target_position, 16384 ) ||
                     !c_physics::linecast( right_eye, target_position, 16384 ) )
                {
                    visible_from_view = true;
                    break;
                }
            }

            if ( visible_from_camera || visible_from_view )
                temp_flags.push_back( { crypt_string( "HIT" ), ImColor( 255, 15, 15 ) } );
        }
    }

    esp.render_weapon( vec3_t::distance( view, entity->get_position( ) ) );
    esp.render_flags( temp_flags );
}

void c_esp::process_planted_bomb( ) const
{
    auto const instance = c_bomb_manager::get_instance( );
    if ( !instance )
    {
        c_ent_planted_bomb_esp::planted = false;
        c_ent_planted_bomb_esp::time_left = 0;
        return;
    }

    process_ent_planted_bomb( reinterpret_cast< uintptr_t >( instance ) );
}

void c_esp::process_ent_planted_bomb( uintptr_t const entity ) const
{
    auto const reset_bomb_state = []( ) -> void {
        c_ent_planted_bomb_esp::planted = false;
        c_ent_planted_bomb_esp::time_left = 0;
    };

    if ( !entity )
    {
        reset_bomb_state( );
        return;
    }

    uintptr_t const planted = *reinterpret_cast< uintptr_t* >( entity + 0x60 );
    if ( !planted )
    {
        reset_bomb_state( );
        return;
    }

    bool const is_defused = *reinterpret_cast< bool* >( entity + 0xB0 );
    c_transform* const transform = *reinterpret_cast< c_transform** >( planted + 0x98 );
    if ( !transform || is_defused )
    {
        reset_bomb_state( );
        return;
    }

    auto const position = transform->get_position( );

    auto const screen = math::world_to_screen( position );

    if ( c_ent_planted_bomb_esp::animation_data.find( entity ) == c_ent_planted_bomb_esp::animation_data.end( ) )
    {
        c_ent_planted_bomb_esp::animation_data[ entity ] = {
            .plant_time = c_ent_planted_bomb_esp::get_current_time( ) };
    }

    c_ent_planted_bomb_esp esp( { position, screen, entity } );

    const float time_left = static_cast< float >( esp.get_time_to_detonation( c_ent_planted_bomb_esp::animation_data[ entity ].plant_time ) );

    if ( time_left < 1 )
    {
        reset_bomb_state( );
        return;
    }

    c_ent_planted_bomb_esp::time_left = time_left;
    c_ent_planted_bomb_esp::planted = time_left > 0;

    if ( c::get< bool >( g_ctx->cfg.visual_planted_bomb ) )
        esp.render( );
}