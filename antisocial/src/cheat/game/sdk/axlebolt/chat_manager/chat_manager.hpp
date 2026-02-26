#pragma once

#include <cstdint>

#include "game/sdk/unity/unity.hpp"

class c_photon_player;

class c_chat_manager
{
    static uintptr_t m_instance;

public:
    static c_chat_manager* get_instance( );

    void send_message( unity::string* msg );
    void send_team( unity::string* msg );
};