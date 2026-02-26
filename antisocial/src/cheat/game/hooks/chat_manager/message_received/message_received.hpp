#pragma once

// и вновь я здесь...

#include <cstdint>

#include "game/sdk/unity/unity.hpp"

namespace hk::chat_manager::message_received
{
    inline void ( *og_recv )( uintptr_t instance, unity::string* msg, uintptr_t idk, uintptr_t allah );
    extern void hk_recv( uintptr_t instance, unity::string* msg, uintptr_t idk, uintptr_t allah );

    extern void hook( );
} // namespace hk::chat_manager::message_received