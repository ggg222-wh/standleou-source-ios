#pragma once

#include <cstdint>
#include <vector>

#include "singleton.hpp"

#include "game/sdk/axlebolt/local_player/local_player.hpp"

class c_player_controller;
class c_local_player;

class c_players_database : public singleton< c_players_database >
{
public:
    ~c_players_database( ) = default;
    c_players_database( ) = default;

    c_local_player* m_local_player;
    std::vector< c_player_controller* > m_players;

    void update( );
    c_player_controller* get_ragebot_entity( ) const;
};

class c_dropped_items_database : public singleton< c_dropped_items_database >
{
public:
    ~c_dropped_items_database( ) = default;
    c_dropped_items_database( ) = default;

    uintptr_t m_weapons_instance;

    std::vector< uintptr_t > m_weapons;
    std::vector< uintptr_t > m_grenades;

    void update_weapons( );
    void update_grenades( );

    void clear( );
};