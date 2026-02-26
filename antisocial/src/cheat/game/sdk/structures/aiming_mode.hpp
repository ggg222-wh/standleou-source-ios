#pragma once

#include <cstdint>

enum aiming_mode : uint8_t
{
    aiming_mode_aiming = 0,
    aiming_mode_aiming_reload = 1,
    aiming_mode_not_aiming = 2,
    aiming_mode_start_aiming = 3,
    aiming_mode_finish_aiming = 4,
};
