//
// Created by Kai Tears on 16/08/2025.
//

#ifndef ALIVE_PLAYERS_HPP
#define ALIVE_PLAYERS_HPP

#include "../widgets.hpp"

class c_wg_alive_players : public c_base_widget
{
    std::vector< std::pair< std::string, std::string > > m_players;

public:
    ~c_wg_alive_players( ) override = default;
    explicit c_wg_alive_players( const ImVec2& pos, bool* active, const std::vector< std::pair< std::string, std::string > >& players )
        : c_base_widget( pos )
        , m_active( active )
        , m_players( players )
    {
    }

    bool* m_active;

    void render( ) override;

    bool m_push_pos = false;

    void render_player( const ImVec2& pos, const float& width, const std::pair< std::string, std::string >& module );
};

#endif // MODULES_HPP
