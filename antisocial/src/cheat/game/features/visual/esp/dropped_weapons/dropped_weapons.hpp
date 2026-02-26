#pragma once

#include <singleton.hpp>

#include <cstdint>

class c_dropped_weapons_esp : public singleton< c_dropped_weapons_esp >
{
public:
    static void execute( uintptr_t weapon );
};