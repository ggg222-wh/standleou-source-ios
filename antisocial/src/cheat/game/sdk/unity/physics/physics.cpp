#include "physics.hpp"

#include "game/sdk/il2cpp/il2cpp.hpp"

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"

#include "game/sdk/unity/component/component.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/unity/objects/objects.hpp"

c_collider* raycast_hit_t::get_collider( )
{
    return ( c_collider* ) c_object::find_from_id( this->collider_id );
}

bool c_physics::linecast( vec3_t start, vec3_t end, int mask )
{
    static auto fn = reinterpret_cast< bool ( * )( vec3_t, vec3_t, int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Physics" ), ay_obfuscate( "Linecast" ), 3 ) );
    return fn( start, end, mask );
}

bool c_physics::linecast( vec3_t start, vec3_t end, raycast_hit_t* ray, int mask, query_trigger_interaction query )
{
    // il2cpp couldn't resolve this properly
    static auto fn = reinterpret_cast< bool ( * )( vec3_t, vec3_t, raycast_hit_t*, int, query_trigger_interaction ) >( memory_manager::get_absolute( 0x2D9839C ) );
    return fn( start, end, ray, mask, query );
}

float c_physics::get_penetration_loss( int type, float thickness )
{
    // standoff2 reversed code (yeah shit ik)
    float num = 1.f;
    bool flag = type == surface_type::cardboard;
    float result;
    if ( flag )
    {
        result = 3.f;
    } else
    {
        bool flag2 = type == surface_type::glass;
        if ( flag2 )
        {
            result = 5.f;
        } else
        {
            bool flag3 = type == surface_type::metal_grate;
            if ( flag3 )
            {
                result = 5.;
            } else
            {
                bool flag4 = type == surface_type::thin_wood;
                if ( flag4 )
                {
                    result = 15.f;
                } else
                {
                    bool flag5 = type == surface_type::thin_metal;
                    if ( flag5 )
                    {
                        result = 20.f;
                    } else
                    {
                        bool flag6 = type == surface_type::character;
                        if ( flag6 )
                        {
                            result = 15.f;
                        } else
                        {
                            bool flag7 = type == surface_type::wood;
                            if ( flag7 )
                            {
                                num = 5.f;
                            }
                            bool flag8 = type == surface_type::plaster;
                            if ( flag8 )
                            {
                                num = 10000.f;
                            }
                            bool flag9 = type == surface_type::tile;
                            if ( flag9 )
                            {
                                num = 10000.f;
                            }
                            bool flag10 = type == surface_type::brick;
                            if ( flag10 )
                            {
                                num = 10000.f;
                            }
                            bool flag11 = type == surface_type::concrete;
                            if ( flag11 )
                            {
                                num = 10000.f;
                            }
                            bool flag12 = type == surface_type::metal;
                            if ( flag12 )
                            {
                                num = 100.f;
                            }
                            bool flag13 = type == surface_type::solid_metal;
                            if ( flag13 )
                            {
                                num = 200.f;
                            }
                            result = thickness * 100.f * num;
                        }
                    }
                }
            }
        }
    }
    return result;
}

c_component* c_physics::raycast_get_collider( raycast_hit_t* ray )
{
    static auto fn = reinterpret_cast< c_component* ( * ) ( raycast_hit_t* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "RaycastHit" ), ay_obfuscate( "get_collider" ), 0 ) );
    return fn( ray );
}

surface_type c_physics::surface_from_tag( unity::string* tag )
{
    static auto fn = reinterpret_cast< surface_type ( * )( unity::string* ) >( memory_manager::get_absolute( 0x17FB318 ) );
    // static auto fn = reinterpret_cast< surface_type ( * )( unity::string* ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "Axlebolt.Standoff.Inventory.Hit" ), ay_obfuscate( "SurfaceTypeUtility" ), ay_obfuscate( "FromTag" ), 1 ) );
    return fn( tag );
}

vec3_t c_physics::get_gravity( )
{
    static auto fn = reinterpret_cast< vec3_t ( * )( ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Physics" ), ay_obfuscate( "get_gravity" ), 0 ) );
    return fn( );
}

bool c_physics::sphere_cast( vec3_t start, float ebu, vec3_t end, raycast_hit_t* ray, float a, int b, int c )
{
    static auto fn = reinterpret_cast< bool ( * )( vec3_t, float, vec3_t, raycast_hit_t*, float, int, int ) >( g_ctx->il2cpp->get_method_pointer( ay_obfuscate( "UnityEngine" ), ay_obfuscate( "Physics" ), ay_obfuscate( "SphereCast" ), 7 ) );
    return fn( start, ebu, end, ray, a, b, c );
}