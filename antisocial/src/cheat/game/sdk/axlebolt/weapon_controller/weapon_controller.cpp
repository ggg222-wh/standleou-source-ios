#include "weapon_controller.hpp"

#include "../gun_controller/gun_controller.hpp"

#include "game/sdk/unity/unity.hpp"
#include "game/sdk/structures/state_simple.hpp"
#include "game/sdk/structures/gun_state.hpp"
#include "game/sdk/structures/sight_type.hpp"
#include "game/sdk/structures/aiming_mode.hpp"

#include "globals.hpp"

// std::map< int, ent_weapon_t > c_weapon_controller::get_by_id {
//     { 0, { crypt_string( "-" ), crypt_string( "-" ) } },

//     // pistols
//     { 11, { crypt_string( "A" ), crypt_string( "G22" ), 20 } },
//     { 12, { crypt_string( "B" ), crypt_string( "USP" ), 12 } },
//     { 13, { crypt_string( "C" ), crypt_string( "P350" ), 13 } },
//     { 15, { crypt_string( "D" ), crypt_string( "Deagle" ), 7 } },
//     { 16, { crypt_string( "E" ), crypt_string( "TEC9" ), 18 } },
//     { 17, { crypt_string( "F" ), crypt_string( "FiveSeven" ), 20 } },
//     { 18, { crypt_string( "G" ), crypt_string( "Berettas" ), 60 } },

//     // smg
//     { 32, { crypt_string( "H" ), crypt_string( "UMP45" ), 25 } },
//     { 33, { crypt_string( "I" ), crypt_string( "AkimboUzi" ), 60 } },
//     { 34, { crypt_string( "J" ), crypt_string( "MP7" ), 30 } },
//     { 35, { crypt_string( "K" ), crypt_string( "P90" ), 50 } },
//     { 36, { crypt_string( "L" ), crypt_string( "MP5" ), 30 } },
//     { 37, { crypt_string( "M" ), crypt_string( "MAC10" ), 30 } },

//     // rifles
//     { 42, { crypt_string( "N" ), crypt_string( "Val" ), 25 } },
//     { 43, { crypt_string( "O" ), crypt_string( "M4A1" ), 25 } },
//     { 44, { crypt_string( "P" ), crypt_string( "AKR" ), 30 } },
//     { 45, { crypt_string( "Q" ), crypt_string( "AKR12" ), 30 } },
//     { 46, { crypt_string( "R" ), crypt_string( "M4" ), 30 } },
//     { 47, { crypt_string( "S" ), crypt_string( "M16" ), 30 } },
//     { 48, { crypt_string( "T" ), crypt_string( "FAMAS" ), 25 } },
//     { 49, { crypt_string( "U" ), crypt_string( "FnFal" ), 30 } },

//     // sniper
//     { 51, { crypt_string( "V" ), crypt_string( "AWM" ), 5 } },
//     { 52, { crypt_string( "W" ), crypt_string( "M40" ), 7 } },
//     { 53, { crypt_string( "X" ), crypt_string( "M110" ), 10 } },
//     { 54, { crypt_string( "r" ), crypt_string( "Mallard" ), 2 } },

//     // heavy
//     { 62, { crypt_string( "Y" ), crypt_string( "SM1014" ), 7 } },
//     { 63, { crypt_string( "Z" ), crypt_string( "FabM" ), 7 } },
//     { 64, { crypt_string( "a" ), crypt_string( "M60" ), 5 } },
//     { 65, { crypt_string( "b" ), crypt_string( "SPAS" ), 8 } },

//     // knives
//     { 70, { crypt_string( "h" ), crypt_string( "Knife" ) } },
//     { 71, { crypt_string( "n" ), crypt_string( "M9 Bayonet" ) } },
//     { 72, { crypt_string( "k" ), crypt_string( "Karambit" ) } },
//     { 73, { crypt_string( "j" ), crypt_string( "jKommando" ) } },
//     { 74, { crypt_string( "]" ), crypt_string( "Bat" ) } },
//     { 75, { crypt_string( "e" ), crypt_string( "Butterfly" ) } },
//     { 77, { crypt_string( "p" ), crypt_string( "Flip Knife" ) } },
//     { 78, { crypt_string( "i" ), crypt_string( "Kunai" ) } },
//     { 79, { crypt_string( "c" ), crypt_string( "Scorpion" ) } },
//     { 80, { crypt_string( "o" ), crypt_string( "Tanton" ) } },
//     { 81, { crypt_string( "d" ), crypt_string( "Dual Daggers" ) } },
//     { 82, { crypt_string( "q" ), crypt_string( "Kukri" ) } },
//     { 83, { crypt_string( "l" ), crypt_string( "Stilleto" ) } },
//     { 85, { crypt_string( "m" ), crypt_string( "Mantis" ) } },
//     { 86, { crypt_string( "f" ), crypt_string( "Fang" ) } },
//     { 88, { crypt_string( "g" ), crypt_string( "Sting" ) } },
//     { 89, { crypt_string( "]" ), crypt_string( "Axe" ) } },

//     // grenades / gear
//     { 91, { crypt_string( "1" ), crypt_string( "HE Grenade" ) } },
//     { 92, { crypt_string( "3" ), crypt_string( "Smoke Grenade" ) } },
//     { 93, { crypt_string( "2" ), crypt_string( "Flash Grenade" ) } },
//     { 100, { crypt_string( "0" ), crypt_string( "Bomb" ) } },
//     { 101, { crypt_string( "6" ), crypt_string( "Kit" ) } },
//     { 102, { crypt_string( "6" ), crypt_string( "Kit" ) } },
//     { 110, { crypt_string( "7" ), crypt_string( "Vest" ) } },
//     { 111, { crypt_string( "8" ), crypt_string( "Vest + Helmet" ) } },
// };

std::map< int, ent_weapon_t > c_weapon_controller::get_by_id {
    // pistols
    { 11, { crypt_string( "D" ), crypt_string( "G22" ), 20 } },
    { 12, { crypt_string( "G" ), crypt_string( "USP" ), 12 } },
    { 13, { crypt_string( "F" ), crypt_string( "P350" ), 13 } },
    { 15, { crypt_string( "A" ), crypt_string( "Desert Eagle" ), 7 } },
    { 16, { crypt_string( "H" ), crypt_string( "Tec-9" ), 18 } },
    { 17, { crypt_string( "C" ), crypt_string( "Five-Seven" ), 20 } },
    { 18, { crypt_string( "B" ), crypt_string( "Dual Berettas" ), 60 } },

    // smg
    { 32, { crypt_string( "L" ), crypt_string( "UMP45" ), 25 } },
    { 33, { crypt_string( "K" ), crypt_string( "Akimbo UZI" ), 60 } },
    { 34, { crypt_string( "N" ), crypt_string( "MP7" ), 30 } },
    { 35, { crypt_string( "P" ), crypt_string( "P90" ), 50 } },
    { 36, { crypt_string( "O" ), crypt_string( "MP5" ), 30 } },
    { 37, { crypt_string( "K" ), crypt_string( "MAC10" ), 30 } },

    // rifles
    { 43, { crypt_string( "T" ), crypt_string( "M4A1" ), 25 } },
    { 44, { crypt_string( "W" ), crypt_string( "AKR" ), 30 } },
    { 45, { crypt_string( "V" ), crypt_string( "AKR12" ), 30 } },
    { 46, { crypt_string( "S" ), crypt_string( "M4A4" ), 30 } },
    { 47, { crypt_string( "U" ), crypt_string( "M16" ), 30 } },
    { 42, { crypt_string( "Q" ), crypt_string( "VAL" ), 25 } },
    { 48, { crypt_string( "R" ), crypt_string( "FAMAS" ), 25 } },
    { 49, { crypt_string( "Q" ), crypt_string( "FnFal" ), 30 } },

    // sniper
    { 51, { crypt_string( "Z" ), crypt_string( "AWM" ), 5 } },
    { 52, { crypt_string( "a" ), crypt_string( "SCOUT" ), 7 } },
    { 53, { crypt_string( "X" ), crypt_string( "SCAR" ), 10 } },
    { 54, { crypt_string( "a" ), crypt_string( "Mallard" ), 2 } },

    // heavy
    { 62, { crypt_string( "b" ), crypt_string( "SM1014" ), 7 } },
    { 63, { crypt_string( "e" ), crypt_string( "FabM" ), 7 } },
    { 64, { crypt_string( "f" ), crypt_string( "M60" ) } },
    { 65, { crypt_string( "c" ), crypt_string( "SPAS" ), 8 } },

    // misc
    { 74, { crypt_string( "]" ), crypt_string( "Bat" ) } },
    { 89, { crypt_string( "]" ), crypt_string( "Axe" ) } },
    { 100, { crypt_string( "o" ), crypt_string( "Bomb" ) } },
    { 70, { crypt_string( "]" ), crypt_string( "Knife" ) } },
    { 71, { crypt_string( "5" ), crypt_string( "M9 Bayonet" ) } },
    { 72, { crypt_string( "4" ), crypt_string( "Karambit" ) } },
    { 73, { crypt_string( "7" ), crypt_string( "jKommando" ) } },
    { 75, { crypt_string( "8" ), crypt_string( "Butterfly" ) } },
    { 77, { crypt_string( "]" ), crypt_string( "Flip Knife" ) } },
    { 78, { crypt_string( "0" ), crypt_string( "Kunai" ) } },
    { 79, { crypt_string( "3" ), crypt_string( "Scorpion" ) } },
    { 80, { crypt_string( "]" ), crypt_string( "Tanto" ) } },
    { 81, { crypt_string( "9" ), crypt_string( "Dual Daggers" ) } },
    { 82, { crypt_string( "6" ), crypt_string( "Kukri" ) } },
    { 83, { crypt_string( "7" ), crypt_string( "Stilleto" ) } },
    { 86, { crypt_string( "2" ), crypt_string( "Fang" ) } },
    { 91, { crypt_string( "j" ), crypt_string( "HE" ) } },
    { 92, { crypt_string( "k" ), crypt_string( "Smoke" ) } },
    { 93, { crypt_string( "i" ), crypt_string( "Flash" ) } },
    { 94, { crypt_string( "l" ), crypt_string( "Molotov" ) } },
    { 95, { crypt_string( "n" ), crypt_string( "Incendiary" ) } },
    { 88, { crypt_string( "[" ), crypt_string( "Sting" ) } },
    { 85, { crypt_string( "[" ), crypt_string( "Mantis" ) } },
    { 89, { crypt_string( "[" ), crypt_string( "Hands" ) } } };

bool c_weapon_controller::is_gun( int id )
{
    return id >= 11 && id <= 65;
}

int c_weapon_controller::get_id( )
{
    uintptr_t const parameters = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x98 );
    if ( !parameters )
        return 0;

    return *reinterpret_cast< int* >( parameters + 0x18 );
}

c_gun_controller* c_weapon_controller::get_gun( )
{
    return reinterpret_cast< c_gun_controller* >( this );
}

uintptr_t c_weapon_controller::get_commands( )
{
    return *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x68 );
}

std::string c_weapon_controller::get_name( )
{
    uintptr_t const parameters = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x98 );
    if ( !parameters )
        return "null";

    return ( *reinterpret_cast< unity::string** >( parameters + 0x20 ) )->to_cpp_string( );
}

bool c_weapon_controller::is_grenade( int id )
{
    return id >= 91 && id <= 95;
}

uintptr_t c_weapon_controller::lod_group( )
{
    return *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x78 );
}

bool c_weapon_controller::ready( )
{
    if ( !is_gun( get_id( ) ) )
        return false;

    auto* const gun = get_gun( );
    if ( !gun )
        return false;

    auto* const state = *reinterpret_cast< state_simple_t< gun_state >** >( reinterpret_cast< uintptr_t >( gun ) + 0x108 );
    if ( !state )
        return false;

    return state->m_current_state == gun_state_ready;
}

bool c_weapon_controller::scoped( )
{
    if ( !is_gun( get_id( ) ) )
        return false;

    auto* const gun = get_gun( );
    if ( !gun )
        return false;

    uintptr_t const parameters = gun->parameters( );
    if ( !parameters )
        return false;

    sight_type const sight = *reinterpret_cast< sight_type* >( parameters + 0x64 );
    if ( sight == sight_type_default )
        return false;

    auto* const current_gun_state = *reinterpret_cast< state_simple_t< gun_state >** >( reinterpret_cast< uintptr_t >( gun ) + 0x108 );
    auto* const current_aiming_state = *reinterpret_cast< state_simple_t< aiming_mode >** >( reinterpret_cast< uintptr_t >( gun ) + 0x110 );
    if ( !current_aiming_state || !current_gun_state )
        return false;

    return ( ( current_aiming_state->m_current_state == aiming_mode_aiming || current_aiming_state->m_current_state == aiming_mode_start_aiming ) && ( current_aiming_state->m_current_state != gun_state_reloading || current_aiming_state->m_current_state != gun_state_pre_reloading ) );
}

ent_weapon_t c_weapon_controller::get_data( )
{
    return get_by_id[ get_id( ) ];
}
