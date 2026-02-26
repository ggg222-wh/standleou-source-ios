#include "fog_manager.hpp"

#include "memory_manager/memory_manager.hpp"

#include "imgui.h"

c_fog_manager* c_fog_manager::get( )
{
    static auto func = reinterpret_cast< c_fog_manager* ( * ) ( ) >( memory_manager::get_absolute( 0x8CC380 ) );
    return func( );
}

void c_fog_manager::update( )
{
    static auto func = reinterpret_cast< void ( * )( c_fog_manager* ) >( memory_manager::get_absolute( 0x8D1440 ) );
    func( this );
}

void c_fog_manager::set_mode( int mode )
{
    *reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( this ) + 360 ) = mode + 1;
}

void c_fog_manager::set_start( float start )
{
    *reinterpret_cast< float* >( reinterpret_cast< uintptr_t >( this ) + 380 ) = start;
}

void c_fog_manager::set_end( float end )
{
    *reinterpret_cast< float* >( reinterpret_cast< uintptr_t >( this ) + 384 ) = end;
}

void c_fog_manager::set_density( float density )
{
    *reinterpret_cast< float* >( reinterpret_cast< uintptr_t >( this ) + 388 ) = density / 1000;
}

void c_fog_manager::set_color( const ImColor& color )
{
    *reinterpret_cast< ImColor* >( reinterpret_cast< uintptr_t >( this ) + 364 ) = color;
}

void c_fog_manager::set_enabled( bool enabled )
{
    *reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( this ) + 356 ) = enabled;
}