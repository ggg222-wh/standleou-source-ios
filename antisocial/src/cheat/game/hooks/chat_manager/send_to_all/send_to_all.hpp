#pragma once

#include <cstdint>

#include "game/sdk/unity/unity.hpp"

namespace hk::chat_manager::send_to_all
{
    inline void ( *og_send )( uintptr_t instance, unity::string* msg );
    extern void hk_send( uintptr_t instance, unity::string* msg );

    extern void hook( );
} // namespace hk::chat_manager::send_to_all