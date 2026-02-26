#pragma once

#include "singleton.hpp"

#include "game/sdk/unity/unity.hpp"

#include "imgui.h"

class c_inventory : public singleton< c_inventory >
{
    unity::array< uintptr_t >* m_skins;
    std::vector< int > m_og_skins;
    unity::array< uintptr_t >* m_definitions;

    int m_selected_idx = -1;

public:
    ~c_inventory( ) = default;
    c_inventory( ) = default;

    std::string m_filter_string = "";

    void change( int idx, int new_id );
    void change_by_id( int orig_id, int new_id );

    void on_gui( int child );

    void save_inventory( );
    void load_inventory( );

private:
    bool case_insensitive_search( const std::string& str, const std::string& substr );
    ImVec4 get_rarity( int rarity );
};