#include "entities.hpp"

#include "game/sdk/axlebolt/player_manager/player_manager.hpp"
#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/biped_map/biped_map.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/misc/math/math.hpp"
#include "game/sdk/axlebolt/photon_network/photon_network.hpp"
#include "game/sdk/axlebolt/grenade_manager/grenade_manager.hpp"
#include "game/features/visual/grenade_prediction/grenade_prediction.hpp"
#include "game/features/visual/esp/players/players.hpp"

#include "alert/alert.hpp"
#include "networking/networking.hpp"

#include "game/hooks/player_inputs/create_move/create_move.hpp"

#include "game/features/features.hpp"

#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/visual/esp/esp.hpp"
#include "game/features/visual/chams/chams.hpp"
#include "game/features/misc/peek_assist/peek_assist.hpp"
#include "game/features/visual/bullet_tracers/bullet_tracers.hpp"
#include "game/features/visual/esp/grenades/grenades.hpp"
#include "game/features/visual/esp/planted_bomb/planted_bomb.hpp"
#include "game/features/visual/skybox/skybox.hpp"
#include "game/features/ragebot/ragebot.hpp"

#include "globals.hpp"

#include <utility>
#include <algorithm>

void c_dropped_items_database::clear( )
{
    m_weapons.clear( );
    m_grenades.clear( );
}

void c_dropped_items_database::update_weapons( )
{
    if ( !m_weapons_instance )
    {
        m_weapons.clear( );
        return;
    }

    auto const all_weapons = *reinterpret_cast< unity::dictionary< int, uintptr_t >** >( m_weapons_instance + 0x28 );
    if ( all_weapons && all_weapons->get_size( ) > 0 )
    {
        m_weapons = all_weapons->get_values( );
    } else
    {
        m_weapons.clear( );
    }
}

void c_dropped_items_database::update_grenades( )
{
    std::vector< uintptr_t > ret;

    auto* const instance = c_grenade_manager::get_instance( );
    if ( !instance )
    {
        m_grenades = { };
        return;
    }

    auto const list = *reinterpret_cast< unity::dictionary< int, uintptr_t >** >( instance + 0x20 );
    if ( !list || list->get_size( ) < 1 )
    {
        m_grenades = { };
        return;
    }

    for ( size_t idx = 0; idx < list->get_size( ); idx++ )
    {
        uintptr_t const grenade = list->get_values( )[ idx ];
        if ( !grenade )
            continue;

        auto const param = *reinterpret_cast< uintptr_t* >( grenade + 0x18 );
        if ( !param )
            continue;

        ret.push_back( grenade );

        if ( c_ent_grenade_esp::animation_data.find( grenade ) == c_ent_grenade_esp::animation_data.end( ) )
        {
            c_ent_grenade_esp::animation_data[ grenade ] = {
                .total_time = *reinterpret_cast< float* >( param + 0x8C ),
                .start_time = std::chrono::steady_clock::now( ),
                .alpha = 0.f };
        }
    }

    m_grenades = std::move( ret );
}

void c_players_database::update( )
{
    auto find_player = [ & ]( std::string const& uid ) {
        for ( auto const& player : m_players )
        {
            if ( !player )
                continue;

            if ( auto* photon = player->photon_player( ) )
                if ( photon->get_uuid( ) == uid )
                    return true;
        }
        return false;
    };

    auto reset_esp_animations = [ & ] {
        auto& params = c_ent_player_esp::m_parameters;
        for ( auto it = params.begin( ); it != params.end( ); )
        {
            if ( !find_player( it->first ) && it->first != crypt_string( "internal_preview_player_uid" ) )
            {
                it = params.erase( it );
            } else
            {
                ++it;
            }
        }
    };

    // auto reset_esp_animations = [ & ] {
    //     erase_if( c_esp::m_parameters, [ & ]( auto const& pair ) {
    //         return !find_player( pair.first );
    //     } );
    // };

    auto clear_data = [ & ] {
        m_players.clear( );
        m_local_player = nullptr;

        c_local_player::m_cmd = nullptr;

        g_ctx->features.bullet_tracers->clear( );
        g_ctx->features.grenade_prediction->clear( );
        g_ctx->features.ragebot->m_target = nullptr;
        g_ctx->features.ragebot->manual_antiaim( antiaim_t::none, false );

        g_ctx->features.peek_assist->m_peeking = false;
        g_ctx->features.peek_assist->m_peeked = false;

        c_ent_grenade_esp::animation_data.clear( );
        c_ent_planted_bomb_esp::animation_data.clear( );
        c_ent_planted_bomb_esp::planted = false;
        c_ent_planted_bomb_esp::time_left = 0.0;

        c_dropped_items_database::get( )->clear( );

        reset_esp_animations( );

        for ( auto it = c_ent_player_esp::m_avatars.begin( ); it != c_ent_player_esp::m_avatars.end( ); )
        {
            if ( it->second.loaded )
                g_ctx->texture_manager->unload( &it->second );
            it = c_ent_player_esp::m_avatars.erase( it );
        }

        for ( auto it = c_ent_player_esp::m_gray_avatars.begin( ); it != c_ent_player_esp::m_gray_avatars.end( ); )
        {
            if ( it->second.loaded )
                g_ctx->texture_manager->unload( &it->second );
            it = c_ent_player_esp::m_gray_avatars.erase( it );
        }
    };

    if ( !c_photon_network::get_room( ) )
    {
        g_ctx->features.chams->clear_bundles( );
        g_ctx->features.chams->clear_textures( );

        g_ctx->features.chams->enemies_frames = 0;
        g_ctx->features.chams->local_frames = 0;
        g_ctx->features.chams->weapon_frames = 0;

        g_ctx->features.skybox->clear_textures( );

        g_ctx->features.skybox->passed_frames = 0;

        c_dropped_items_database::get( )->m_weapons_instance = 0;

        if ( c::get< bool >( g_ctx->cfg.world_modulation_auto ) )
            c::get< bool >( g_ctx->cfg.world_modulate ) = true;

        clear_data( );
        return;
    }

    auto* const player_manager = c_player_manager::get_instance( );
    if ( !player_manager )
    {
        clear_data( );
        return;
    }

    m_local_player = player_manager->get_local_player( );
    if ( !m_local_player || !m_local_player->alive( ) )
    {
        clear_data( );
        return;
    }

    g_ctx->features.chams->load_bundles( );
    g_ctx->features.chams->init_textures( );
    g_ctx->features.skybox->init_textures( );

    hk::player_inputs::create_move::update( );

    m_local_player->fix_occlusion( );

    m_players = player_manager->get_players( );
    c_local_player::m_team = m_local_player->get_team( );

    g_ctx->features_mgr->process_pre_render( );

    for ( auto* entity : m_players )
    {
        if ( !entity )
            continue;

        entity->fix_occlusion( );
        g_ctx->features.esp->process_ent_player( entity );

        if ( entity->alive( ) )
            g_ctx->features_mgr->process_enemy( entity );
    }

    g_ctx->features_mgr->process_local_player( m_local_player );

    g_ctx->features_mgr->process_post_render( );

    reset_esp_animations( );
}

struct target_info_t
{
    c_player_controller* player;
    c_transform* bone;
    float distance;
    bool visible;
    bool wallbang;
};

c_player_controller* c_players_database::get_ragebot_entity( ) const
{
    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
        return nullptr;

    vec3_t const view_pos = g_ctx->features.thirdperson->get_unmodified_view( );
    std::vector< target_info_t > candidates;

    auto gather_bones = [ & ]( c_player_controller* entity ) {
        if ( !entity || !entity->alive( ) )
            return;
        auto* const photon = entity->photon_player( );
        if ( !photon || photon->get_property< bool >( crypt_string( "untouchable" ) ) )
            return;

        auto* const biped = entity->biped_map( );
        if ( !biped )
            return;

        std::vector< c_transform* > bones;
        biped->get_customized( bones, true, true, true, true );
        if ( bones.empty( ) )
            return;

        const int fov_setting = c::get< int >( g_ctx->cfg.ragebot_fov );
        const bool fov_enabled = fov_setting > 0;

        for ( auto* const bone : bones )
        {
            if ( !bone )
                continue;

            vec3_t const pos = bone->get_position( );
            float const dist = vec3_t::distance( pos, view_pos );
            bool const visible = !c_physics::linecast( view_pos, pos, 16384 );
            bool const wallbang = g_ctx->features.ragebot->autowall( view_pos, pos );

            auto const screen_opt = math::world_to_screen( pos );

            bool const in_fov = !fov_enabled || ( screen_opt.check && math::check_fov( screen_opt.position, fov_setting ) );

            if ( ( visible || wallbang ) && in_fov )
                candidates.push_back( { entity, bone, dist, visible, wallbang } );
        }
    };

    for ( auto* const entity : c_players_database::get( )->m_players )
        gather_bones( entity );

    if ( candidates.empty( ) )
        return nullptr;

    std::sort( candidates.begin( ), candidates.end( ),
               []( const target_info_t& a, const target_info_t& b ) {
                   if ( a.visible != b.visible )
                       return a.visible > b.visible;
                   if ( a.wallbang != b.wallbang )
                       return a.wallbang > b.wallbang;
                   return a.distance < b.distance;
               } );

    return candidates.front( ).player;
}
