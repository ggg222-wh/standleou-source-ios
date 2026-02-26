#pragma once

#include "singleton.hpp"

class c_texture;

class c_skybox : public singleton< c_skybox >
{
    c_texture* m_texture;

public:
    ~c_skybox( ) = default;
    c_skybox( ) = default;

    int passed_frames { 0 };

    void execute( );

    void init_textures( );
    void clear_textures( );
};