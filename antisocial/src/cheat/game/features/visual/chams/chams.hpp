#pragma once

#include <map>

#include "singleton.hpp"

class c_player_controller;
class c_texture;

class c_chams : public singleton< c_chams >
{
    c_texture* m_local_texture;
    c_texture* m_enemies_texture;
    c_texture* m_weapon_fallback_texture;

    std::map< int, c_texture* > m_weapon_textures;

public:
    ~c_chams( ) = default;
    c_chams( ) = default;

    int local_frames { 0 };
    int enemies_frames { 0 };
    int weapon_frames { 0 };

    void process_enemy( c_player_controller* ent );
    void process_local( c_player_controller* ent );
    void process_weapon( c_player_controller* ent );

    void load_bundles( );
    void clear_bundles( );
    void init_textures( );
    void clear_textures( );
};
