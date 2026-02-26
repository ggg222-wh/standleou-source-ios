#pragma once

#include "singleton.hpp"

class c_chat_spam : public singleton< c_chat_spam >
{
public:
    ~c_chat_spam( ) = default;
    c_chat_spam( ) = default;

    void execute( ) const;
};