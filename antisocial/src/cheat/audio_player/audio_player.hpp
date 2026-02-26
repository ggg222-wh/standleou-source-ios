#pragma once

#include "singleton.hpp"

enum sound_t : int
{
    button = 0,
    hitlog,
    hitsound,
    manualaachange,
    peekend,
    peekstart,
    toggle,
    horn
};

class c_audio_player : public singleton< c_audio_player >
{
public:
    ~c_audio_player( ) = default;
    c_audio_player( ) = default;

    void play( int idx );
};