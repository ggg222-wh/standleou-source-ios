#pragma once

#include "../../structures/team.hpp"

#include "../player_controller/player_controller.hpp"

class c_player_inputs;

class c_local_player : public c_player_controller
{
public:
    ~c_local_player( ) = default;
    c_local_player( ) = default;

    static team_t m_team;
    static bool m_in_air;
    static c_player_inputs* m_cmd;
    static c_player_inputs m_old_cmd;
};