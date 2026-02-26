#include "peek_assist.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/misc/math/math.hpp"
#include "audio_player/audio_player.hpp"

#include "globals.hpp"

void c_peek_assist::execute( c_player_controller* local )
{
    if ( !local || !local->alive( ) || !c::get< bool >( g_ctx->cfg.misc_peek_assist ) || !g_ctx->user_data.premium )
        return;

    if ( !m_peeking )
    {
        if ( m_peeked )
        {
            if ( vec3_t::distance( m_last_pos, local->get_position( ) ) < 10.f )
            {
                while ( m_last_pos != local->get_position( ) )
                    local->component( )->get_transform( )->set_position( m_last_pos );
            }

            g_ctx->audio_player->play( static_cast< int >( sound_t::peekend ) );

            m_peeked = false;
        }

        return;
    }

    if ( !m_peeked )
        m_last_pos = local->get_position( );

    math::add_3d_radial_circle( ImGui::GetBackgroundDrawList( ), m_last_pos, 0.6f, 34, ImColor( 255, 255, 255, 175 ), ImColor( 255, 255, 255, 0 ) );
    // math::draw_3d_dotted_circle( m_last_pos, 0.6f, ImColor( 255, 255, 255 ), 14 );

    m_peeked = true;
}
