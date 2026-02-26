#include "local_player.hpp"

#include "game/sdk/axlebolt/player_inputs/player_inputs.hpp"

team_t c_local_player::m_team { 999 };
bool c_local_player::m_in_air { false };
c_player_inputs* c_local_player::m_cmd { nullptr };
c_player_inputs c_local_player::m_old_cmd { };