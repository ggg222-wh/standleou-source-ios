#pragma once

#include "../../structures/team.hpp"

#include "../../unity/component/component.hpp"
#include "../../structures/vec3.hpp"
#include "../../structures/view_mode.hpp"

class c_photon_player;
class c_biped_map;
class c_aiming_data;
class c_transform;
class c_weapon_controller;
class c_mesh;

struct ImVec2;

class c_player_controller
{
public:
    ~c_player_controller( ) = default;
    c_player_controller( ) = default;

    c_photon_player* photon_player( );
    uintptr_t photon_view( );
    team_t get_team( );
    bool alive( );
    int get_health( );
    vec3_t get_position( );
    c_weapon_controller* get_weapon( );
    c_biped_map* biped_map( );
    c_aiming_data* aiming_data( );
    void fix_occlusion( );
    void set_tps_view( );
    void set_fps_view( );
    c_transform* main_camera_holder( );
    void force_move( const ImVec2& direction );
    c_component* main_camera( );
    c_transform* fps_camera( );
    view_mode_t view_mode( );
    bool in_air( );
    c_mesh* mesh_lod_group( );
    bool visible( );
    bool crouching( );
    void force_lag( const bool& lag );
    bool has_weapon( int idx );
    bool firing_grenade( );
    c_component* component( );
};