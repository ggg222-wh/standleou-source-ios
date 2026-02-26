#pragma once

#include <singleton.hpp>
#include <unordered_map>
#include <vector>

class c_player_controller;

struct ImFont;

struct esp_fonts_t
{
    ImFont* verdana { };
    ImFont* pixel { };
    ImFont* weapons { };
    ImFont* inferno_svg { };
};

class c_esp : public singleton< c_esp >
{
public:
    ~c_esp( ) = default;
    c_esp( ) = default;

    static esp_fonts_t m_fonts;

    static std::vector< std::pair< ImFont*, float > > m_nickname_fonts;

    void process_dropped_weapons( ) const;
    void process_thrown_grenades( ) const;
    void process_planted_bomb( ) const;
    void process_ent_planted_bomb( uintptr_t entity ) const;
    void process_ent_player( c_player_controller* entity ) const;
};