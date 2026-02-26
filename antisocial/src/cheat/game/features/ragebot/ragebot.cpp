#include "ragebot.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/biped_map/biped_map.hpp"
#include "game/sdk/axlebolt/aiming_data/aiming_data.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/axlebolt/player_inputs/player_inputs.hpp"
#include "game/sdk/axlebolt/gun_controller/gun_controller.hpp"
#include "game/sdk/axlebolt/grenade_controller/grenade_controller.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/game_object/game_object.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/structures/euler.hpp"
#include "game/sdk/misc/math/math.hpp"

#include "audio_player/audio_player.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/entities/entities.hpp"
#include "globals.hpp"

#include <random>
#include <chrono>

void c_ragebot::movement_fix( float orig_yaw, float antiaim_yaw, c_player_inputs* inputs )
{
    auto* const player = c_players_database::get( )->m_local_player;
    if ( !inputs || !player || !player->alive( ) )
        return;

    if ( inputs->vertical( ) == 0.f || inputs->horizontal( ) == 0.f )
        return;

    auto const deg2rad = []( float deg ) { return deg * M_PI / 180.f; };

    auto const normalize_yaw = []( float yaw ) {
        return yaw < 0.f ? 360.f + yaw : yaw;
    };

    float const f1 = normalize_yaw( orig_yaw );
    float const f2 = normalize_yaw( antiaim_yaw );

    float yaw_delta = ( f2 < f1 ) ? std::abs( f2 - f1 ) : 360.f - std::abs( f1 - f2 );
    yaw_delta = 360.f - yaw_delta;

    float const forward = inputs->vertical( );
    float const side = inputs->horizontal( );

    ImVec2 fixed_move {
        std::cos( deg2rad( yaw_delta ) ) * side + std::cos( deg2rad( yaw_delta + 90 ) ) * forward,
        std::sin( deg2rad( yaw_delta ) ) * side + std::sin( deg2rad( yaw_delta + 90 ) ) * forward };

    player->force_move( fixed_move );
    inputs->vertical( ) = fixed_move.y;
    inputs->horizontal( ) = fixed_move.x;
}

bool c_ragebot::autowall( vec3_t const& start, vec3_t const& end ) const
{
    if ( !c::get< bool >( g_ctx->cfg.ragebot_autowall ) )
        return false;

    auto pass1 = [ & ]( ) -> bool {
        raycast_hit_t hit { }, hit2 { };
        c_physics::linecast( start, end, &hit, 16384 );

        c_collider* const collider = hit.get_collider( );
        if ( !collider )
            return false;

        unity::string* const tag = collider->get_tag( );
        if ( !tag )
            return false;

        surface_type const surface = c_physics::surface_from_tag( tag );
        if ( !math::check_surface_by_type( surface ) )
            return false;

        return !c_physics::linecast( hit.point, end, &hit2, 16384 );
    };

    auto pass2 = [ & ]( ) -> bool {
        vec3_t const dir = ( end - start ).normalized( );
        float total_loss = 0.0f;

        vec3_t cur_point = start;

        for ( int i = 0; i < 4; ++i )
        {
            raycast_hit_t hit { }, hit_exit { };

            if ( !c_physics::linecast( cur_point, end, &hit, 16384 ) )
                return true;

            c_collider* col = hit.get_collider( );
            if ( !col )
                return false;

            unity::string* tag = col->get_tag( );
            if ( !tag )
                return false;

            surface_type surf = c_physics::surface_from_tag( tag );
            if ( !math::check_surface_by_type( surf ) || surf == surface_mark_trigger )
                return false;

            if ( !c_physics::linecast( hit.point + dir * 0.01f, end, &hit_exit, 16384 ) )
                return true;

            float thickness = vec3_t::distance( hit.point, hit_exit.point );
            if ( thickness < 0.01f )
                thickness = 0.01f;

            float loss = c_physics::get_penetration_loss( surf, thickness );
            total_loss += loss;

            if ( total_loss > 100.0f )
                return false;

            cur_point = hit_exit.point + dir * 0.01f;
        }

        return true;
    };

    return pass1( ) && pass2( );
}

std::pair< bool, std::pair< vec3_t, std::pair< c_transform*, vec3_t > > > c_ragebot::select_first_available_point( c_biped_map* biped, const vec3_t& view, c_gun_controller* gun ) const
{
    if ( !gun || !biped )
        return { false, {} };

    auto const dmg_ok = [ gun ]( gun_damage_t dmg ) {
        return gun->get_damage( dmg ) > c::get< int >( g_ctx->cfg.ragebot_minimal_damage );
    };

    std::vector< c_transform* > bones;
    biped->get_customized(
        bones,
        c::get< bool >( g_ctx->cfg.ragebot_head ) && dmg_ok( gun_damage_t::head ),
        c::get< bool >( g_ctx->cfg.ragebot_body ) && dmg_ok( gun_damage_t::body ),
        c::get< bool >( g_ctx->cfg.ragebot_arms ) && dmg_ok( gun_damage_t::arms ),
        c::get< bool >( g_ctx->cfg.ragebot_legs ) && dmg_ok( gun_damage_t::legs ) );

    if ( bones.empty( ) )
        return { false, {} };

    bool const fov_enabled = c::get< int >( g_ctx->cfg.ragebot_fov ) > 0;
    bool const legit_selection = c::get< bool >( g_ctx->cfg.ragebot_legit_selection );
    ImVec2 constscreen_center = ImGui::GetIO( ).DisplaySize / 2;

    struct candidate_t
    {
        vec3_t pos;
        c_transform* bone;
    };
    std::vector< candidate_t > candidates;

    for ( auto* bone : bones )
    {
        if ( !bone )
            continue;

        vec3_t const bone_pos = bone->get_position( );

        float scale = c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_head ) / 4;
        if ( bone == biped->head( ) || bone == biped->neck( ) )
            scale = c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_head ) / 4;
        else if ( bone == biped->spine1( ) || bone == biped->spine2( ) || bone == biped->hip( ) )
            scale = c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_body ) / 4;
        else if ( bone == biped->left_shoulder( ) || bone == biped->left_upperarm( ) || bone == biped->left_forearm( ) || bone == biped->left_hand( ) ||
                  bone == biped->right_shoulder( ) || bone == biped->right_upperarm( ) || bone == biped->right_forearm( ) || bone == biped->right_hand( ) )
            scale = c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_arms ) / 4;
        else if ( bone == biped->left_thigh( ) || bone == biped->left_calf( ) || bone == biped->left_foot( ) ||
                  bone == biped->right_thigh( ) || bone == biped->right_calf( ) || bone == biped->right_foot( ) )
            scale = c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_legs ) / 4;

        float const tightness = c::get< float >( g_ctx->cfg.ragebot_multipoints_tightness ) / 4;
        int const points_count = static_cast< int >( tightness * 40.0f );

        constexpr int seed = 1337;
        srand( seed );

        std::vector< vec3_t > points;
        for ( int i = 0; i < points_count; i++ )
        {
            float const theta = ( ( float ) rand( ) / RAND_MAX ) * 2.0f * 3.1415926f;
            float const phi = ( ( float ) rand( ) / RAND_MAX ) * 3.1415926f;
            float const r = ( ( float ) rand( ) / RAND_MAX ) * scale;

            float const x = r * sinf( phi ) * cosf( theta );
            float const y = r * sinf( phi ) * sinf( theta );
            float const z = r * cosf( phi );

            points.push_back( bone_pos + vec3_t { x, y, z } );
        }

        for ( auto& pt : points )
        {
            auto const screen = math::world_to_screen( pt );
            if ( screen.check )
                ImGui::GetBackgroundDrawList( )->AddCircleFilled( screen.position, 1, ImColor( 255, 255, 255 ), 10 );
        }

        for ( auto& pt : points )
        {
            bool const linecast = c_physics::linecast( view, pt, 16384 );
            bool const wallbang = autowall( view, pt );
            if ( !linecast || ( wallbang && c::get< bool >( g_ctx->cfg.ragebot_autowall ) ) )
                candidates.push_back( { pt, bone } );
        }
    }

    if ( fov_enabled && legit_selection && !candidates.empty( ) )
    {
        const ImVec2 screen_center = ImGui::GetIO( ).DisplaySize / 2;

        auto const best = std::min_element( candidates.begin( ), candidates.end( ),
                                            [ & ]( const candidate_t& a, const candidate_t& b ) {
                                                auto sa = math::world_to_screen( a.pos );
                                                auto sb = math::world_to_screen( b.pos );
                                                if ( !sa.check )
                                                    return false;
                                                if ( !sb.check )
                                                    return true;

                                                float da = std::hypot( sa.position.x - screen_center.x, sa.position.y - screen_center.y );
                                                float db = std::hypot( sb.position.x - screen_center.x, sb.position.y - screen_center.y );
                                                return da < db;
                                            } );
        if ( best != candidates.end( ) )
            return { true, { best->pos, { best->bone, best->pos - best->bone->get_position( ) } } };
    }

    if ( !candidates.empty( ) )
        return { true, { candidates.front( ).pos, { candidates.front( ).bone, candidates.front( ).pos - candidates.front( ).bone->get_position( ) } } };

    return bones.front( ) ? std::make_pair( false, std::make_pair( bones.front( )->get_position( ), std::make_pair( bones.front( ), vec3_t { } ) ) )
                          : std::make_pair( false, std::make_pair( vec3_t { }, std::make_pair( nullptr, vec3_t { } ) ) );
}

c_transform* c_ragebot::select_bone( c_biped_map* biped, const vec3_t& view, c_gun_controller* gun ) const
{
    if ( !gun )
        return nullptr;

    std::vector< c_transform* > bones;
    auto dmg_ok = [ gun ]( gun_damage_t dmg ) { return gun->get_damage( dmg ) > c::get< int >( g_ctx->cfg.ragebot_minimal_damage ); };

    biped->get_customized(
        bones,
        c::get< bool >( g_ctx->cfg.ragebot_head ) && dmg_ok( gun_damage_t::head ),
        c::get< bool >( g_ctx->cfg.ragebot_body ) && dmg_ok( gun_damage_t::body ),
        c::get< bool >( g_ctx->cfg.ragebot_arms ) && dmg_ok( gun_damage_t::arms ),
        c::get< bool >( g_ctx->cfg.ragebot_legs ) && dmg_ok( gun_damage_t::legs ) );

    if ( bones.empty( ) )
        return nullptr;

    const bool fov_enabled = c::get< int >( g_ctx->cfg.ragebot_fov ) > 0;
    const ImVec2 screen_center = ImGui::GetIO( ).DisplaySize / 2;

    std::vector< c_transform* > candidates;

    for ( auto* bone : bones )
        if ( bone )
        {
            const vec3_t pos = bone->get_position( );
            if ( !c_physics::linecast( view, pos, 16384 ) || autowall( view, pos ) )
                candidates.push_back( bone );
        }

    if ( !fov_enabled || !c::get< bool >( g_ctx->cfg.ragebot_legit_selection ) )
        return !candidates.empty( ) ? candidates.front( ) : bones.front( );

    if ( !candidates.empty( ) )
    {
        std::sort( candidates.begin( ), candidates.end( ), [ & ]( c_transform* a, c_transform* b ) {
            auto sa = math::world_to_screen( a->get_position( ) );
            auto sb = math::world_to_screen( b->get_position( ) );
            if ( !sa.check )
                return false;
            if ( !sb.check )
                return true;

            const float dx_a = sa.position.x - screen_center.x;
            const float dy_a = sa.position.y - screen_center.y;
            const float dx_b = sb.position.x - screen_center.x;
            const float dy_b = sb.position.y - screen_center.y;

            const float dist_a = std::hypot( dx_a, dy_a );
            const float dist_b = std::hypot( dx_b, dy_b );

            return dist_a < dist_b;
        } );
        return candidates.front( );
    }

    return bones.front( );
}

void c_ragebot::execute( c_player_controller* local )
{
    if ( !c::get< bool >( g_ctx->cfg.ragebot_enabled ) )
        return;

    if ( !local || !local->alive( ) )
        return;

    auto* const weapon = local->get_weapon( );
    if ( !weapon || !weapon->is_gun( weapon->get_id( ) ) )
        return;

    auto* const gun = weapon->get_gun( );
    if ( !gun )
        return;

    doubletap( local, gun, weapon->get_commands( ) );

    auto* const target = c_players_database::get( )->get_ragebot_entity( );
    if ( !target || !target->alive( ) )
        return;

    vec3_t const view_pos = g_ctx->features.thirdperson->get_unmodified_view( );
    c_transform* bone = nullptr;

    std::pair< bool, std::pair< vec3_t, std::pair< c_transform*, vec3_t > > > selected_point = { };
    if ( c::get< bool >( g_ctx->cfg.ragebot_multipoints ) )
    {
        selected_point = select_first_available_point( target->biped_map( ), view_pos, gun );
        bone = selected_point.second.second.first;
    } else
        bone = select_bone( target->biped_map( ), view_pos, gun );

    if ( !bone )
        return;

    bool default_passed = false;
    vec3_t pos = bone->get_position( );
    if ( c::get< bool >( g_ctx->cfg.ragebot_multipoints ) )
    {
        auto const bone_point = bone->get_position( );
        bool const linecast = !c_physics::linecast( view_pos, bone_point, 16384 );
        bool const wallbang = autowall( view_pos, bone_point );
        default_passed = wallbang || linecast;
        pos = default_passed ? bone_point : selected_point.second.first;
    }

    auto const try_aim = [ this, local ]( vec3_t const& from, vec3_t const& to, bool silent ) -> bool {
        bool const linecast = c_physics::linecast( from, to, 16384 );
        bool const wallbang = autowall( from, to );

        if ( linecast && ( !wallbang || !c::get< bool >( g_ctx->cfg.ragebot_autowall ) ) )
            return false;

        if ( !silent )
        {
            if ( auto* const data = local->aiming_data( ) )
            {
                auto rot = vec3_t::to_euler_rad( quater_t::look_rotation( to - from ) );

                if ( rot.x >= 275.f )
                    rot.x -= 360.f;
                if ( rot.x <= -275.f )
                    rot.x += 360.f;

                data->cur_aim_ang< vec3_t >( ) = rot;
                data->cur_euler_ang< vec3_t >( ) = rot;

                orig_camera_angles = { rot.x, rot.y, orig_camera_angles.roll };
            }
        }

        return true;
    };

    if ( ( !c::get< bool >( g_ctx->cfg.ragebot_multipoints ) || ( selected_point.first || default_passed ) ) && try_aim( view_pos, pos, c::get< bool >( g_ctx->cfg.ragebot_silent ) ) )
    {
        // if ( c::get< bool >( g_ctx->cfg.ragebot_silent ) )
        {
            if ( !c::get< bool >( g_ctx->cfg.ragebot_multipoints ) )
                m_target = bone;
            else
            {
                m_target = selected_point.second.second.first;
                m_target_offset = selected_point.second.second.second;
            }
            // m_end = pos;
            m_start = view_pos;
        }

        if ( c::get< bool >( g_ctx->cfg.ragebot_triggerbot ) )
            m_fire = true;
    }
}

void c_ragebot::cast_hit( vec3_t& start, vec3_t& target, uintptr_t )
{
    if ( !m_target ) // && !c::get< bool >( g_ctx->cfg.ragebot_multipoints )
        return;

    if ( c::get< bool >( g_ctx->cfg.ragebot_enabled ) ) //_silent
    {
        start = m_start;
        target = c::get< bool >( g_ctx->cfg.ragebot_multipoints ) ? ( ( m_target->get_position( ) + m_target_offset ) - m_start ).normalized( ) : ( m_target->get_position( ) - m_start ).normalized( );
    }

    m_target = nullptr;
}

void c_ragebot::antiaim( c_player_controller* local, c_player_inputs* inputs )
{
    std::random_device rd { };
    std::mt19937 mt { rd( ) };

    if ( !c::get< bool >( g_ctx->cfg.ragebot_antiaim ) )
        return;

    auto* const aim_data = local->aiming_data( );
    if ( !aim_data )
        return;

    if ( orig_camera_angles.is_empty( ) )
        orig_camera_angles = { aim_data->cur_aim_ang< vec3_t >( ).x, aim_data->cur_euler_ang< vec3_t >( ).y, 0.f };

    if ( orig_camera_angles.is_empty( ) )
        return;

    orig_camera_angles.normalize( 70.f );
    orig_camera_angles.pitch -= inputs->delta_aim_angles( ).x;
    orig_camera_angles.yaw += inputs->delta_aim_angles( ).y;

    euler_angles_t aa { orig_camera_angles.pitch, orig_camera_angles.yaw, 0.f };

    // pitch cfg
    switch ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).pitch_type )
    {
    case 1:
        aa.pitch = 90.f;
        break;
    case 2:
        aa.pitch = -90.f;
        break;
    default:
        break;
    }

    // yaw cfg
    if ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).manuals && m_manual_selected != antiaim_t::none )
    {
        aa.yaw = vec3_t::normalize_angle( orig_camera_angles.yaw + ( m_manual_selected == antiaim_t::left ? -90.f : 90.f ) );
    } else
    {
        float base_yaw = orig_camera_angles.yaw;
        switch ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).yaw_type )
        {
        case 1:
            base_yaw = 165.f;
            break;
        case 2:
            base_yaw = 0.f;
            break;
        default:
            break;
        }

        auto const get_target_direction = [ & ]( ) -> float {
            auto* const target = c_players_database::get( )->get_ragebot_entity( );
            if ( !target || !target->alive( ) )
                return orig_camera_angles.yaw;

            auto* const biped = target->biped_map( );
            if ( !biped )
                return orig_camera_angles.yaw;

            auto* const head = biped->head( );
            if ( !head )
                return orig_camera_angles.yaw;

            auto rot = vec3_t::to_euler_rad( quater_t::look_rotation( head->get_position( ) - g_ctx->features.thirdperson->get_unmodified_view( ) ) );

            if ( rot.x >= 275.f )
                rot.x -= 360.f;
            if ( rot.x <= -275.f )
                rot.x += 360.f;

            return rot.y;
        };

        float yaw_angle = base_yaw;
        if ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).yaw_type != 0 )
        {
            switch ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).yaw_select )
            {
            case 0: // at local
                yaw_angle = vec3_t::normalize_angle( orig_camera_angles.yaw + base_yaw );
                break;
            case 1: // at target
                yaw_angle = vec3_t::normalize_angle( get_target_direction( ) + base_yaw );
                break;
            default:
                break;
            }
        }

        // jitter
        if ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter )
        {
            static int frames { };
            static bool flip { };

            if ( frames >= c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_frames )
            {
                frames = 0;
                flip = !flip;
            }
            ++frames;

            if ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_ranged )
            {
                int const range = c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_range;
                yaw_angle = vec3_t::normalize_angle( yaw_angle + ( flip ? range : -range ) );
            } else
            {
                int const yaw = flip
                                    ? c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_first
                                    : c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_second;
                yaw_angle = vec3_t::normalize_angle( orig_camera_angles.yaw + yaw );
            }
        }

        aa.yaw = yaw_angle;

        if ( local->in_air( ) && c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_in_air )
        {
            constexpr float pitches[] { -70.f, 70.f };
            constexpr float yaws[] { 0.f, 45.f, 90.f, -45.f, -90.f };

            std::uniform_int_distribution<> ry( 0, 4 ), rp( 0, 1 );
            aa.pitch = pitches[ rp( mt ) ];
            aa.yaw = yaws[ ry( mt ) ];
        }
    }

    bool stop = false;

    if ( ( inputs->fire( ) && local->get_weapon( )->ready( ) ) || inputs->drop( ) || inputs->pickup( ) || local->firing_grenade( ) )
        stop = true;

    aa.normalize( 70.f );

    if ( !stop )
    {
        aim_data->cur_aim_ang< vec3_t >( ).x = aa.pitch;
        aim_data->cur_euler_ang< vec3_t >( ).y = aa.yaw;
    } else
    {
        aim_data->cur_aim_ang< vec3_t >( ).x = orig_camera_angles.pitch;
        aim_data->cur_euler_ang< vec3_t >( ).y = orig_camera_angles.yaw;
    }

    movement_fix( orig_camera_angles.yaw, aim_data->cur_euler_ang< vec3_t >( ).y, inputs );
}

void c_ragebot::doubletap( c_player_controller* local, c_gun_controller* gun, uintptr_t cmds )
{
    if ( !local || !local->alive( ) || !gun || !cmds || !c::get< bool >( g_ctx->cfg.ragebot_doubletap ) )
        return;

    auto const now = std::chrono::steady_clock::now( );
    float const elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( now - m_doubletap_time_point ).count( ) / 1000.f;

    if ( !( *reinterpret_cast< bool* >( cmds + 0x10 ) ) || elapsed < 1.f )
        return;

    float const tf = gun->time_fired( );
    gun->time_fired( ) = tf - ( tf * 2 );

    m_fire = true;
    m_doubletap_time_point = now;

    local->force_lag( false );
}

void c_ragebot::fake_lags( ) const
{
    if ( !g_ctx->user_data.premium )
        return;

    static uint8_t tick { };
    static bool update { };

    if ( !c::get< bool >( g_ctx->cfg.ragebot_fakelags ) && !update )
        return;

    if ( tick > 10 )
        tick = 0;

    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
    {
        tick = 0;
        return;
    }

    if ( !c::get< bool >( g_ctx->cfg.ragebot_fakelags ) )
    {
        local->force_lag( false );
        update = false;
        return;
    }

    update = true;
    local->force_lag( tick < c::get< int >( g_ctx->cfg.ragebot_fakelags_rate ) );
    ++tick;
}

void c_ragebot::manual_antiaim( antiaim_t which, bool sound )
{
    if ( !c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).manuals )
        return;

    m_manual_selected = ( which != m_manual_selected ) ? which : antiaim_t::none;

    if ( !sound )
        return;

    g_ctx->audio_player->play( static_cast< int >( sound_t::manualaachange ) );
}

void c_ragebot::render( ) const
{
    ImVec2 const center = ImGui::GetIO( ).DisplaySize / 2;
    auto* const draw = ImGui::GetBackgroundDrawList( );

    if ( c::get< int >( g_ctx->cfg.ragebot_fov ) > 0 && c::get< bool >( g_ctx->cfg.ragebot_enabled ) )
    {
        math::add_fov_circle( draw, c::get< int >( g_ctx->cfg.ragebot_fov ), ImColor( 190, 105, 255 ), ImColor( 105, 175, 255 ) );
    }

    static c_animator left( ImVec4( 0, 0, 0, 0 ) );
    static c_animator right( ImVec4( 0, 0, 0, 0 ) );

    auto const& cfg = c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config );

    left.update< ImVec4 >( cfg.manuals ? ( m_manual_selected == antiaim_t::left
                                               ? ImColor( 225, 155, 255, 255 ).Value
                                               : ImColor( 255, 255, 255, 200 ).Value )
                                       : ImVec4( 0, 0, 0, 0 ) );

    right.update< ImVec4 >( cfg.manuals ? ( m_manual_selected == antiaim_t::right
                                                ? ImColor( 225, 155, 255, 255 ).Value
                                                : ImColor( 255, 255, 255, 200 ).Value )
                                        : ImVec4( 0, 0, 0, 0 ) );

    draw->AddTriangleFilled( { center.x - 25, center.y - 5 }, { center.x - 25, center.y + 5 }, { center.x - 35, center.y }, ImColor( left.get< ImVec4 >( ) ) );
    draw->AddTriangleFilled( { center.x + 25, center.y - 5 }, { center.x + 25, center.y + 5 }, { center.x + 35, center.y }, ImColor( right.get< ImVec4 >( ) ) );
}

vec3_t c_ragebot::get_original_view_angles( c_player_controller* local ) const
{
    if ( !local || !local->alive( ) )
        return vec3_t { };

    auto* const aim_data = local->aiming_data( );
    if ( !aim_data )
        return vec3_t { };

    if ( c::get< bool >( g_ctx->cfg.ragebot_antiaim ) )
    {
        if ( !orig_camera_angles.is_empty( ) )
            return { orig_camera_angles.pitch, orig_camera_angles.yaw, 0.f };

        return vec3_t { aim_data->cur_aim_ang< vec3_t >( ).x, aim_data->cur_euler_ang< vec3_t >( ).y, 0.f };
    }

    return vec3_t { aim_data->cur_aim_ang< vec3_t >( ).x, aim_data->cur_euler_ang< vec3_t >( ).y, 0.f };
}

void c_ragebot::set_angles( c_player_controller* local )
{
    if ( c::get< bool >( g_ctx->cfg.ragebot_antiaim ) )
    {
        c_transform* camera_transform { };
        if ( local->view_mode( ) == view_mode_t::view_mode_fps )
        {
            camera_transform = local->fps_camera( );
        } else if ( local->view_mode( ) == view_mode_t::view_mode_tps )
        {
            camera_transform = local->main_camera( )->get_transform( );
        }

        if ( camera_transform )
        {
            camera_transform->set_euler( orig_camera_angles );
        }
    }
}
