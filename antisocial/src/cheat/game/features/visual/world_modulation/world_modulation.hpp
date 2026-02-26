#pragma once

#include "singleton.hpp"

class c_modulation : public singleton< c_modulation >
{
public:
    ~c_modulation( ) = default;
    c_modulation( ) = default;

    void modulate( ) const;
};
