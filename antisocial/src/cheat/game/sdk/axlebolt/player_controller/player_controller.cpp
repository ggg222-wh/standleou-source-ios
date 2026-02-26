#include "player_controller.hpp"

#include "../../unity/transform/transform.hpp"
#include "../../il2cpp/il2cpp.hpp"
#include "../../unity/unity.hpp"
#include "../../unity/physics/physics.hpp"

#include "../photon_player/photon_player.hpp"
#include "../biped_map/biped_map.hpp"
#include "../aiming_data/aiming_data.hpp"
#include "../occlusion_controller/occlusion_controller.hpp"
#include "../mesh/mesh.hpp"
#include "../weapon_controller/weapon_controller.hpp"
#include "../grenade_controller/grenade_controller.hpp"
#include "game/sdk/structures/state_simple.hpp"
#include "game/sdk/structures/move_state.hpp"
#include "game/sdk/structures/view_synchronization.hpp"

#include "game/features/visual/thirdperson/thirdperson.hpp"

#include "../../structures/blended_value.hpp"
#include "../../structures/view_mode.hpp"

#include "globals.hpp"
#include "imgui.h"

void c_player_controller::force_move( const ImVec2& direction )
{
    if ( !this )
        return;

    auto movement_control = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0xA8 );

    if ( movement_control )
    {
        auto params = *reinterpret_cast< uintptr_t* >( movement_control + 0x70 );
        auto data = *reinterpret_cast< uintptr_t* >( movement_control + 0x78 );

        if ( data && params )
        {
            auto walk_params = *reinterpret_cast< uintptr_t* >( params + 0x38 );
            auto dir = *reinterpret_cast< blended_value_t< ImVec2 >** >( data + 0xA8 );

            auto speed = *reinterpret_cast< float* >( walk_params + 0x10 );

            if ( dir )
                dir->m_actual = ( direction * speed );
        }
    }
}

c_photon_player* c_player_controller::photon_player( )
{
    return *reinterpret_cast< c_photon_player** >( reinterpret_cast< uintptr_t >( this ) + 0x108 );
}

uintptr_t c_player_controller::photon_view( )
{
    return *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0xF8 );
}

team_t c_player_controller::get_team( )
{
    return static_cast< team_t >( photon_player( )->get_property< int >( "team" ) );
}

int c_player_controller::get_health( )
{
    return photon_player( )->get_property< int >( "health" );
}

bool unity_is_alive( uintptr_t obj )
{
    if ( !obj )
        return false;

    uintptr_t cached_ptr = 0;
    if ( memcpy( &cached_ptr, reinterpret_cast< void* >( obj + 0x10 ), sizeof( cached_ptr ) ) == nullptr )
        return false;

    return cached_ptr != 0;
}

bool c_player_controller::alive( )
{
    try
    {

        if ( reinterpret_cast< uintptr_t >( this ) && reinterpret_cast< uintptr_t >( this ) > 0x1000 && unity_is_alive( reinterpret_cast< uintptr_t >( this ) ) )
        {
            if (
                reinterpret_cast< uintptr_t >( this ) == 0 ||
                reinterpret_cast< uintptr_t >( this ) == NULL ||
                !reinterpret_cast< uintptr_t >( this ) )
                return false;

            uintptr_t const cached_ptr = *reinterpret_cast< uintptr_t* >( ( uintptr_t ) this + 0x10 );
            if ( cached_ptr && cached_ptr != 0 )
            {
                return photon_player( ) && biped_map( ) && get_health( ) > 0;
            }
        }

        return false;
    } catch ( ... )
    {
        return false;
    }
}

vec3_t c_player_controller::get_position( )
{
    return component( )->get_transform( )->get_position( );
}

c_weapon_controller* c_player_controller::get_weapon( )
{
    uintptr_t const weaponry = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x98 );
    if ( !weaponry )
        return nullptr;

    return *reinterpret_cast< c_weapon_controller** >( weaponry + 0x90 );
}

c_biped_map* c_player_controller::biped_map( )
{
    return *reinterpret_cast< c_biped_map** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );
}

c_aiming_data* c_player_controller::aiming_data( )
{
    uintptr_t const aim_controller = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x90 );
    if ( !aim_controller )
        return nullptr;

    return *reinterpret_cast< c_aiming_data** >( aim_controller + 0x90 );
}

void c_player_controller::fix_occlusion( )
{
    if ( auto* const occlusion_controller = *reinterpret_cast< c_occlusion_controller** >( reinterpret_cast< uintptr_t >( this ) + 0xC0 ); occlusion_controller )
    {
        occlusion_controller->set_visible( true );
        occlusion_controller->set_enabled( false );
        occlusion_controller->set_in_area( false );
    }
}

void c_player_controller::set_tps_view( )
{
    static auto fn = reinterpret_cast< void ( * )( c_player_controller* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Player" ), ay_obfuscate( "PlayerController" ), ay_obfuscate( "SetTPSView" ), 0 ) );
    fn( this );
};

void c_player_controller::set_fps_view( )
{
    static auto fn = reinterpret_cast< void ( * )( c_player_controller* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Player" ), ay_obfuscate( "PlayerController" ), ay_obfuscate( "SetFPSView" ), 0 ) );
    fn( this );
};

c_transform* c_player_controller::main_camera_holder( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x20 );
}

c_component* c_player_controller::main_camera( )
{
    return *reinterpret_cast< c_component** >( reinterpret_cast< uintptr_t >( this ) + 0x50 );
}

c_transform* c_player_controller::fps_camera( )
{
    uintptr_t aim_controller = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x90 );
    if ( aim_controller )
    {
        static auto get_transform = reinterpret_cast< c_transform* ( * ) ( uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "GameObject" ), ay_obfuscate( "get_transform" ), 0 ) );
        return get_transform( *reinterpret_cast< uintptr_t* >( aim_controller + 0x60 ) );
    }
}

view_mode_t c_player_controller::view_mode( )
{
    static auto fn = reinterpret_cast< view_mode_t ( * )( c_player_controller* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Player" ), ay_obfuscate( "PlayerController" ), ay_obfuscate( "get_ViewMode" ), 0 ) );
    return fn( this );
}

bool c_player_controller::in_air( )
{
    uintptr_t const movement_controller = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0xA8 );
    if ( movement_controller )
    {
        uintptr_t const character_controller = *reinterpret_cast< uintptr_t* >( movement_controller + 0xC8 );
        if ( character_controller )
        {
            static auto fn = reinterpret_cast< bool ( * )( uintptr_t ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "CharacterController" ), ay_obfuscate( "get_isGrounded" ), 0 ) );
            return !fn( character_controller );
        }
    }
}

c_mesh* c_player_controller::mesh_lod_group( )
{
    return *reinterpret_cast< c_mesh** >( reinterpret_cast< uintptr_t >( this ) + 0xE0 );
}

bool c_player_controller::visible( )
{
    c_biped_map* const biped = biped_map( );
    if ( !biped )
        return false;

    auto const check_visibility = [ this ]( c_transform* transform ) -> bool {
        if ( !transform )
            return false;

        vec3_t const pos = transform->get_position( );

        return !c_physics::linecast( g_ctx->features.thirdperson->get_unmodified_view( ), pos, 16384 );
    };

    return check_visibility( biped->head( ) ) ||
           check_visibility( biped->hip( ) ) ||
           check_visibility( biped->left_hand( ) ) ||
           check_visibility( biped->right_hand( ) ) ||
           check_visibility( biped->left_hand( ) ) ||
           check_visibility( biped->right_hand( ) );
}

bool c_player_controller::crouching( )
{
    uintptr_t const aim_controller = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x90 );
    if ( aim_controller )
    {
        state_simple_t< move_state >* const state = *reinterpret_cast< state_simple_t< move_state >** >( aim_controller + 0x128 );
        if ( !state )
            return false;

        return state->m_current_state == move_state_crouch || state->m_current_state == move_state_finished_crouch;
    }

    return false;
}

void c_player_controller::force_lag( const bool& lag )
{
    static view_synchronization orig = view_synchronization_off;
    uintptr_t const view = photon_view( );

    if ( !view )
    {
        return;
    }

    if ( orig == view_synchronization_off )
    {
        orig = *reinterpret_cast< view_synchronization* >( view + 0x48 );
    }

    *reinterpret_cast< view_synchronization* >( view + 0x48 ) = ( lag ? view_synchronization_off : orig );
}

bool c_player_controller::has_weapon( int idx )
{
    if ( !this || !this->alive( ) )
        return false;

    uintptr_t const weaponry = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x98 );
    if ( !weaponry )
        return false;

    auto const weapon_by_slots = *reinterpret_cast< unity::dictionary< int, c_weapon_controller* >** >( weaponry + 0x68 );
    if ( !weapon_by_slots )
        return false;

    auto const values = weapon_by_slots->get_values( );
    for ( auto const& val : values )
    {
        if ( !val )
            continue;

        if ( val->get_id( ) == idx )
            return true;
    }

    return false;
}

bool c_player_controller::firing_grenade( )
{
    if ( auto* const weap = get_weapon( ) )
    {
        if ( weap->is_grenade( weap->get_id( ) ) )
        {
            auto* const grenade = reinterpret_cast< c_grenade_controller* >( weap );
            if ( grenade->prev_aim_cmd( ) || grenade->prev_fire_cmd( ) )
                return true;
        }
    }

    return false;
}

c_component* c_player_controller::component( )
{
    return reinterpret_cast< c_component* >( this );
}