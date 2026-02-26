#pragma once

struct ImVec2;

class c_player_inputs
{
public:
    bool& fire( );
    bool& drop( );
    bool& pickup( );
    ImVec2& delta_aim_angles( );
    float& horizontal( );
    float& vertical( );
};