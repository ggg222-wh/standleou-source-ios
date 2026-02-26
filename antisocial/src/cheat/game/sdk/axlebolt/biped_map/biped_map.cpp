#include "biped_map.hpp"

#include "../../unity/transform/transform.hpp"

#include <cstdint>

c_transform* c_biped_map::head( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x18 );
}

c_transform* c_biped_map::neck( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x20 );
}

c_transform* c_biped_map::spine1( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x28 );
}

c_transform* c_biped_map::spine2( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );
}

c_transform* c_biped_map::left_shoulder( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x38 );
}

c_transform* c_biped_map::left_upperarm( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x40 );
}

c_transform* c_biped_map::left_forearm( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x48 );
}

c_transform* c_biped_map::left_hand( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x50 );
}

c_transform* c_biped_map::right_shoulder( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x58 );
}

c_transform* c_biped_map::right_upperarm( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x60 );
}

c_transform* c_biped_map::right_forearm( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x68 );
}

c_transform* c_biped_map::right_hand( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x70 );
}

c_transform* c_biped_map::hip( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x78 );
}

c_transform* c_biped_map::left_thigh( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x80 );
}

c_transform* c_biped_map::left_calf( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x88 );
}

c_transform* c_biped_map::left_foot( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x90 );
}

c_transform* c_biped_map::right_thigh( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0x98 );
}

c_transform* c_biped_map::right_calf( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xA0 );
}

c_transform* c_biped_map::right_foot( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xA8 );
}

c_transform* c_biped_map::right_thumb0( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xB0 );
}

c_transform* c_biped_map::left_thumb0( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xB8 );
}

c_transform* c_biped_map::r_finger0( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xC0 );
}

c_transform* c_biped_map::r_finger1( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xC8 );
}

c_transform* c_biped_map::r_finger2( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xD0 );
}

c_transform* c_biped_map::r_finger3( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xD8 );
}

c_transform* c_biped_map::r_finger4( )
{
    return *reinterpret_cast< c_transform** >( reinterpret_cast< uintptr_t >( this ) + 0xE0 );
}

void c_biped_map::get_customized( std::vector< c_transform* >& result, bool include_head, bool include_body, bool include_arms, bool include_legs )
{
    if ( include_head )
    {
        result.push_back( head( ) );
        result.push_back( neck( ) );
    }

    if ( include_body )
    {
        result.push_back( spine1( ) );
        result.push_back( spine2( ) );
        result.push_back( hip( ) );
    }

    if ( include_arms )
    {
        result.push_back( left_shoulder( ) );
        result.push_back( left_upperarm( ) );
        result.push_back( left_forearm( ) );
        result.push_back( left_hand( ) );
        result.push_back( right_shoulder( ) );
        result.push_back( right_upperarm( ) );
        result.push_back( right_forearm( ) );
        result.push_back( right_hand( ) );
    }

    if ( include_legs )
    {
        result.push_back( left_thigh( ) );
        result.push_back( left_calf( ) );
        result.push_back( left_foot( ) );
        result.push_back( right_thigh( ) );
        result.push_back( right_calf( ) );
        result.push_back( right_foot( ) );
    }
}

std::vector< c_transform* > c_biped_map::get_all( )
{
    return {
        head( ), neck( ), spine1( ), spine2( ),
        left_shoulder( ), left_upperarm( ), left_forearm( ), left_hand( ),
        right_shoulder( ), right_upperarm( ), right_forearm( ), right_hand( ),
        hip( ), left_thigh( ), left_calf( ), left_foot( ),
        right_thigh( ), right_calf( ), right_foot( ) };
}

std::vector< c_transform* > c_biped_map::get_cut( )
{
    return {
        head( ), neck( ), spine1( ), spine2( ),
        left_shoulder( ), left_upperarm( ), left_forearm( ), left_hand( ),
        right_shoulder( ), right_upperarm( ), right_forearm( ), right_hand( ),
        left_thigh( ), left_calf( ), left_foot( ),
        right_thigh( ), right_calf( ), right_foot( ) };
}