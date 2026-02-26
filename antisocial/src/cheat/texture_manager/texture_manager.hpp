#pragma once

#include "singleton.hpp"

struct texture_data_t
{
    void* texture_id = 0;
    int width = 0;
    int height = 0;
    bool loaded = false;
};

class c_texture_manager : public singleton< c_texture_manager >
{
    bool m_initialized { false };

public:
    ~c_texture_manager( ) = default;
    c_texture_manager( ) = default;

    bool load( unsigned char* data, int size, texture_data_t* texture, const std::string& uuid );
    bool load_grayscale( unsigned char* data, int length, texture_data_t* texture, const std::string& uuid );
    void unload( texture_data_t* texture );

    void init_internal( );
};