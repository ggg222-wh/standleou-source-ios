#pragma once

enum gun_state : uint8_t
{
    gun_state_reloading = 0,
    gun_state_pre_reloading = 1,
    gun_state_switching = 2,
    gun_state_try_to_switch = 3,
    gun_state_ready = 4,
};
