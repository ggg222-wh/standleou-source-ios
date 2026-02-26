#pragma once

#include "../../structures/vec3.hpp"

#include "game/sdk/unity/component/component.hpp"

#include "imgui.h"
#include "imgui_internal.h"

class c_collider;

struct raycast_hit_t
{
public:
    vec3_t point, normal { };
    uint32_t face_id { };
    float distance { };
    ImVec2 uv { };
    uint64_t collider_id { };

    raycast_hit_t( ) = default;
    ~raycast_hit_t( ) = default;

    c_collider* get_collider( );
};

class c_collider : public c_component
{
};

/*
struct raycast_hit_t
{
    vec3_t point, normal;
    unsigned int m_face_id;
    float distance;
    ImVec2 ui;
    int m_collider;
};
*/
enum surface_type : uint8_t
{
    unknown = 0,
    glass = 1,
    cardboard = 2,
    metal_grate = 3,
    wood = 4,
    plaster = 5,
    tile = 6,
    metal = 7,
    concrete = 8,
    brick = 9,
    solid_metal = 10,
    thin_metal = 11,
    thin_wood = 12,
    character = 13,
    ground = 14,
    surface_mark_trigger = 15,
    water = 16,
    paper = 17,
    grass = 18,
    gravel = 19,
    grass_no_decal = 20
};

class c_surface_mark_trigger
{
public:
};

class c_component;

namespace unity
{
    struct string;
}

enum query_trigger_interaction : uint8_t
{
    query_trigger_interaction_use_global = 0,
    query_trigger_interaction_ignore = 1,
    query_trigger_interaction_collide = 2,
};

class c_physics
{
public:
    static float get_penetration_loss( int type, float thickness );
    static bool linecast( vec3_t start, vec3_t end, int mask );
    static bool linecast( vec3_t start, vec3_t end, raycast_hit_t* ray, int mask, query_trigger_interaction query = query_trigger_interaction_use_global );
    static c_component* raycast_get_collider( raycast_hit_t* ray );
    static surface_type surface_from_tag( unity::string* tag );
    static vec3_t get_gravity( );
    static bool sphere_cast( vec3_t start, float ebu, vec3_t end, raycast_hit_t* ray, float a, int b, int c );
};