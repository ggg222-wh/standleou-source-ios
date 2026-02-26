#pragma once

struct ImColor;

class c_fog_manager
{
public:
    static c_fog_manager* get( );

    void update( );
    void set_mode( int mode ); // 0 - 2
    void set_start( float start );
    void set_end( float end );
    void set_density( float density );
    void set_color( const ImColor& color );
    void set_enabled( bool enabled );
};