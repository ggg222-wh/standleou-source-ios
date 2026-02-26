#pragma once

#include <cstdint>

enum view_synchronization : uint8_t
{
    view_synchronization_off = 0,
    view_synchronization_reliable_delta_compressed = 1,
    view_synchronization_unreliable = 2,
    view_synchronization_unreliable_on_change = 3,
};
