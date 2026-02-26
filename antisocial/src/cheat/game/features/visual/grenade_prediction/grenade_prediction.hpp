#pragma once

#include "singleton.hpp"

#include <deque>

#include "game/sdk/structures/vec3.hpp"

struct predict_info
{
public:
    vec3_t pos { };
    bool is_hit_pos { };
    bool is_final_hit { };
    int alpha = 255;
    bool is_visible { };
};

class c_player_controller;

class c_grenade_prediction : public singleton< c_grenade_prediction >
{
public:
    uintptr_t m_target_grenade { 0 };
    int m_predict_stage { 0 };
    bool m_time_to_explode { false };
    int m_grenadeid { 0 };
    std::deque< predict_info > m_predict_data { };
    std::deque< predict_info > m_predict_data_s { };
    bool m_timetothrow { false };

public:
    ~c_grenade_prediction( ) = default;
    c_grenade_prediction( ) = default;

    void render( );
    void execute( c_player_controller* local );

    void clear( );
};