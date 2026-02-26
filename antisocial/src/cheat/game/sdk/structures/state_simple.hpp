#pragma once

template < typename T >
struct state_simple_t
{
    char pad[ sizeof( void* ) * 2 ];

public:
    T m_current_state;
    T m_prev_state;
    float m_time_switched;
    bool m_just_switched;
    int m_state_no;
};
