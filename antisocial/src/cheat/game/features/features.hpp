#pragma once

#include "singleton.hpp"

class c_player_controller;

class c_features_manager : public singleton< c_features_manager >
{
public:
    ~c_features_manager( ) = default;
    c_features_manager( ) = default;

    void process_local_player( c_player_controller* local ) const;
    void process_enemy( c_player_controller* ent ) const;
    void process_pre_render( ) const;
    void process_post_render( ) const;
};