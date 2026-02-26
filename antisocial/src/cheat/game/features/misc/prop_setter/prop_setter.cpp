#include "prop_setter.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/axlebolt/photon_player/photon_player.hpp"

#include "globals.hpp"

void c_prop_setter::execute( c_player_controller* local )
{
    if ( !local || !local->alive( ) )
        return;

    if ( !m_pending )
        return;

    auto* const photon = local->photon_player( );
    if ( !photon )
        return;

    // will fix later idc

    // if ( m_key == crypt_string( "score" ) )
    // {
    //     if ( auto val = std::any_cast< int >( &m_value ) )
    //     {
    //         photon->add_score( *val );
    //         // photon->set_property
    //         // TODO koro4e
    //     }
    // }

    if ( auto val = std::any_cast< int >( &m_value ) )
    {
        photon->add_score( *val );
        // photon->set_property
        // TODO koro4e
    }

    m_pending = false;
}
