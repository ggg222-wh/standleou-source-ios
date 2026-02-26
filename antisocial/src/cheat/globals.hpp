#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <atomic>

#include "game/sdk/structures/vec3.hpp"

#include "config/data.hpp"

#include "texture_manager/texture_manager.hpp"

#include "thirdparty/oxorany/oxorany_include.h"
// #include "ay_obfuscate.hpp"
// #include "ns_crypt.hpp"

class c_interface;
class c_il2cpp_api;

class c_esp;
class c_ragebot;
class c_thirdperson;
class c_bunnyhop;
class c_peek_assist;
class c_bullet_tracers;
class c_modulation;
class c_chams;
class c_aspect_ratio;
class c_spoof;
class c_nuke;
class c_removals;
class c_fov;
class c_hit_points;
class c_inventory;
class c_fog;
class c_grenade_prediction;
class c_prop_setter;
class c_chat_spam;
class c_weapon_exploits;
class c_skybox;
class c_particles;
class c_hit_markers;

class c_texture_manager;

class c_features_manager;

class c_hooks;

class c_config;

class c_audio_player;

class c_widgets;

class c_chat;

class c_ws_pranks;

struct user_t
{
    std::string username;
    std::string telegram_id;
    bool premium;
    texture_data_t profile_picture;
    std::vector< unsigned char > profile_picture_raw;
};

struct features_t
{
    c_esp* esp;
    c_ragebot* ragebot;
    c_thirdperson* thirdperson;
    c_bunnyhop* bunnyhop;
    c_peek_assist* peek_assist;
    c_bullet_tracers* bullet_tracers;
    c_modulation* modulation;
    c_chams* chams;
    c_aspect_ratio* aspect_ratio;
    c_spoof* spoof;
    c_nuke* nuke;
    c_removals* removals;
    c_fov* fov;
    c_hit_points* hit_points;
    c_inventory* inventory;
    c_fog* fog;
    c_grenade_prediction* grenade_prediction;
    c_prop_setter* prop_setter;
    c_chat_spam* chat_spam;
    c_weapon_exploits* weapon_exploits;
    c_skybox* skybox;
    c_particles* particles;
    c_hit_markers* hit_markers;

    void init( );

    features_t( ) = default;
    ~features_t( ) = default;
};

struct globals_t
{
    globals_t( );
    ~globals_t( ) = default;

    c_interface* interface;
    c_il2cpp_api* il2cpp;

    cfg_t cfg;
    features_t features;

    c_features_manager* features_mgr;

    c_hooks* hooks;

    c_config* config;

    c_audio_player* audio_player;

    c_texture_manager* texture_manager;

    c_widgets* widgets;

    c_chat* chat;

    c_ws_pranks* ws_pranks;

    bool is_presented = false;
    std::atomic< bool > is_loading = false;
    std::atomic< bool > is_initialized = false;
    std::atomic< bool > passed = false;
    std::string documents_directory;

    user_t user_data;

    std::string backup_hardware;
    const char* backup_model = oxorany( "iPhone13,37" );
};

extern globals_t* g_ctx;

#if DEBUG
#define ay_obfuscate
#define ns_encrypt( str ) @( str )
#define crypt_string( str ) std::string( str )
#else
#define ay_obfuscate oxorany
#define ns_encrypt( str ) @( oxorany( str ) )
#define crypt_string( str ) std::string( oxorany( str ) )
#endif

void debug_log( const std::string& str );