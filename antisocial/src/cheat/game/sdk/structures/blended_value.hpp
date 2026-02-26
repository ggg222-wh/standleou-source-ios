#pragma once

template < typename T >
struct blended_value_t
{
    char pad[ sizeof( void* ) * 2 ];

public:
    T m_actual;
    T m_actual_prev_frame;
    T m_blended;
    T m_previous;
    float m_time_fix;
    float m_blend_duration;
};
