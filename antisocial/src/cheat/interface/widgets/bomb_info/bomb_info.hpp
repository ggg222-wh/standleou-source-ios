//
// Created by Kai Tears on 16/08/2025.
//

#ifndef BOMB_INFO_HPP
#define BOMB_INFO_HPP

#include "../widgets.hpp"

struct bomb_info_t
{
    int time_left = 0;
};

class c_wg_bomb_info : public c_base_widget
{
public:
    ~c_wg_bomb_info( ) override = default;
    explicit c_wg_bomb_info( const ImVec2& pos, bool* active )
        : c_base_widget( pos )
        , m_active( active )
    {
    }

    bool* m_active;

    bool m_push_pos = false;

    void render( ) override;
};

#endif // MODULES_HPP
