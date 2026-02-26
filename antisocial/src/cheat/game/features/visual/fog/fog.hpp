#pragma once

#include "singleton.hpp"

class c_fog : public singleton< c_fog >
{
public:
    ~c_fog( ) = default;
    c_fog( ) = default;

    void execute( ) const;
};