#pragma once

#include <cstdint>

enum move_state : uint8_t
{
    move_state_not_stated = 0,
    move_state_crouch = 1,
    move_state_stand = 2,
    move_state_finished_crouch = 3,
    move_state_finished_stand = 4,
};
