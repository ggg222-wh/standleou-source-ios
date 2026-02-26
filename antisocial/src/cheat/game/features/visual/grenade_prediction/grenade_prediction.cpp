#include "grenade_prediction.hpp"

#include "globals.hpp"

#include "game/sdk/misc/math/math.hpp"
#include "game/entities/entities.hpp"
#include "game/sdk/axlebolt/weapon_controller/weapon_controller.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/component/component.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/sdk/unity/physics/physics.hpp"

void c_grenade_prediction::render( )
{
    auto* const local = c_players_database::get( )->m_local_player;
    if ( !local || !local->alive( ) )
        return;

    auto* const weapon = local->get_weapon( );
    if ( !weapon )
        return;

    if ( !c::get< bool >( g_ctx->cfg.visual_grenade_prediction ) )
        return;

    // Обработка целевой гранаты
    if ( m_target_grenade )
    {
        int state = *reinterpret_cast< int* >( m_target_grenade );
        if ( state == 3 )
        {
            clear( );
            return;
        }
    }

    auto color = c::get< ImVec4 >( g_ctx->cfg.visual_grenade_prediction_color );
    auto const convert_color = []( float a ) -> int { return static_cast< int >( a * 255 ); };

    // Отрисовка траектории целевой гранаты
    if ( m_predict_data_s.size( ) >= 2 && m_target_grenade )
    {
        for ( auto i = 1; i < m_predict_data_s.size( ); i++ )
        {
            if ( i < m_predict_stage )
                continue;

            auto const& current = m_predict_data_s[ i ];
            auto const& previous = m_predict_data_s[ i - 1 ];

            auto const screen_pos = math::world_to_screen( current.pos );
            auto const screen_prev_pos = math::world_to_screen( previous.pos );

            if ( screen_pos.check && screen_prev_pos.check )
                ImGui::GetBackgroundDrawList( )->AddLine( screen_pos.position, screen_prev_pos.position, ImColor( color ), 2 );

            if ( screen_pos.check && current.is_final_hit )
            {
                math::draw_3d_circle( current.pos, 2, 2, color, 22, false, 0.3f );
                break;
            }
        }
    }

    // Отрисовка предсказания для гранаты в руках
    if ( weapon->is_grenade( weapon->get_id( ) ) && m_predict_data.size( ) >= 2 )
    {
        for ( auto i = 1; i < m_predict_data.size( ); i++ )
        {
            auto const& current = m_predict_data[ i ];
            auto const& previous = m_predict_data[ i - 1 ];

            auto current_screen_pos = math::world_to_screen( current.pos );
            auto prev_screen_pos = math::world_to_screen( previous.pos );

            if ( current_screen_pos.check && prev_screen_pos.check )
            {
                auto line_color = current.is_visible
                                      ? ImColor( convert_color( color.x ), convert_color( color.y ), convert_color( color.z ), current.alpha / 2 )
                                      : ImColor( 255, 255, 255, current.alpha / 2 );

                ImGui::GetBackgroundDrawList( )->AddLine( prev_screen_pos.position, current_screen_pos.position, line_color, 2 );
            }

            if ( current_screen_pos.check && current.is_hit_pos )
            {
                auto radius = current.is_final_hit ? 6 : 4;
                auto circle_color = current.is_final_hit
                                        ? ImColor( 255, 255, 255 )
                                        : ImColor( 255, 255, 255, current.alpha / 2 );

                ImGui::GetBackgroundDrawList( )->AddCircleFilled( current_screen_pos.position, radius, circle_color );
            }
        }
    }
}

void c_grenade_prediction::execute( c_player_controller* local )
{
    if ( !local || !local->alive( ) || !c::get< bool >( g_ctx->cfg.visual_grenade_prediction ) )
        return;

    auto* const weapon = local->get_weapon( );
    if ( !weapon || !weapon->is_grenade( weapon->get_id( ) ) || !c_camera::get_main( ) )
        return;

    m_predict_data.clear( );

    constexpr int max_hits = 6;
    float time_alive = 3;
    int hits = 0;
    float bounce_scale = 0.5f;

    auto const camera_transform = reinterpret_cast< c_component* >( c_camera::get_main( ) )->get_transform( );
    vec3_t throw_vec = vec3_t::normalize( camera_transform->get_forward( ) + camera_transform->get_up( ) * 0.1f ) * 18.5f;
    vec3_t start_pos = g_ctx->features.thirdperson->get_unmodified_view( ) + vec3_t( 0, 0.05f, 0 );
    vec3_t position = start_pos;

    for ( float time_live = 0.0f; time_live < time_alive; time_live += 0.02f )
    {
        bool is_hit_pos = false;
        if ( time_live >= time_alive - 0.02f )
        {
            if ( hits < max_hits )
                is_hit_pos = true;
            hits = max_hits;
        }

        vec3_t old_position = position;
        vec3_t velocity = throw_vec + c_physics::get_gravity( ) * time_live * time_live / 2;
        position = start_pos + throw_vec * time_live;
        position += c_physics::get_gravity( ) * time_live * time_live / 2;

        raycast_hit_t hit;
        if ( hits < max_hits && c_physics::sphere_cast( old_position, 0.10f,
                                                        vec3_t::normalize( position - old_position ), &hit,
                                                        vec3_t::distance( old_position, position ), 16384, 0 ) )
        {
            throw_vec = -2 * vec3_t::dot( hit.normal, vec3_t::normalize( position - old_position ) * 20 ) * hit.normal +
                        vec3_t::normalize( position - old_position ) * 19;
            throw_vec *= bounce_scale;
            bounce_scale /= 3;
            start_pos = old_position;
            position = start_pos;
            is_hit_pos = true;
            hits++;
            time_live = 0;

            if ( hits >= max_hits )
                time_alive = 0.5f;
        }

        m_predict_data.push_back( { position,
                                    is_hit_pos,
                                    hits >= max_hits,
                                    hits >= max_hits ? static_cast< int >( ( 0.3f - time_live ) * ( 255.0f / 0.3f ) ) : 255,
                                    !c_physics::linecast( g_ctx->features.thirdperson->get_unmodified_view( ), position, 16384 ) } );
    }
}

void c_grenade_prediction::clear( )
{
    m_target_grenade = 0;
    m_predict_stage = 0;
    m_time_to_explode = false;
    m_grenadeid = 0;
    m_predict_data.clear( );
    m_predict_data_s.clear( );
    m_timetothrow = false;
}