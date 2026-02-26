#pragma once

#include <vector>

class c_transform;

class c_biped_map
{
public:
    c_transform* head( );
    c_transform* neck( );
    c_transform* spine1( );
    c_transform* spine2( );
    c_transform* left_shoulder( );
    c_transform* left_upperarm( );
    c_transform* left_forearm( );
    c_transform* left_hand( );
    c_transform* right_shoulder( );
    c_transform* right_upperarm( );
    c_transform* right_forearm( );
    c_transform* right_hand( );
    c_transform* hip( );
    c_transform* left_thigh( );
    c_transform* left_calf( );
    c_transform* left_foot( );
    c_transform* right_thigh( );
    c_transform* right_calf( );
    c_transform* right_foot( );
    c_transform* right_thumb0( );
    c_transform* left_thumb0( );
    c_transform* r_finger0( );
    c_transform* r_finger1( );
    c_transform* r_finger2( );
    c_transform* r_finger3( );
    c_transform* r_finger4( );

    void get_customized( std::vector< c_transform* >& result, bool include_head, bool include_body, bool include_arms, bool include_legs );
    std::vector< c_transform* > get_all( );
    std::vector< c_transform* > get_cut( );
};