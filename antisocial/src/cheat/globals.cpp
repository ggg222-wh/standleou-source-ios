#include "globals.hpp"

#include "interface/interface.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"

#include "game/features/visual/esp/esp.hpp"
#include "game/features/visual/thirdperson/thirdperson.hpp"
#include "game/features/visual/bullet_tracers/bullet_tracers.hpp"
#include "game/features/visual/world_modulation/world_modulation.hpp"
#include "game/features/visual/chams/chams.hpp"
#include "game/features/ragebot/ragebot.hpp"
#include "game/features/movement/bunnyhop/bunnyhop.hpp"
#include "game/features/misc/peek_assist/peek_assist.hpp"
#include "game/features/misc/aspect_ratio/aspect_ratio.hpp"
#include "game/features/misc/spoof/spoof.hpp"
#include "game/features/misc/nuke/nuke.hpp"
#include "game/features/misc/removals/removals.hpp"
#include "game/features/misc/fov/fov.hpp"
#include "game/features/visual/hit_points/hit_points.hpp"
#include "game/features/visual/fog/fog.hpp"
#include "game/features/inventory/inventory.hpp"
#include "game/features/visual/grenade_prediction/grenade_prediction.hpp"
#include "game/features/misc/prop_setter/prop_setter.hpp"
#include "game/features/misc/chat_spam/chat_spam.hpp"
#include "game/features/exploits/weapon/weapon.hpp"
#include "game/features/visual/skybox/skybox.hpp"
#include "game/features/visual/particles/particles.hpp"
#include "game/features/visual/hit_markers/hit_markers.hpp"

#include "config/config.hpp"
#include "audio_player/audio_player.hpp"
#include "texture_manager/texture_manager.hpp"
#include "interface/widgets/widgets.hpp"
#include "chat/chat.hpp"
#include "ws_pranks/ws_pranks.hpp"

#include "game/features/features.hpp"

#include "game/hooks/hooks.hpp"

void features_t::init( )
{
    esp = c_esp::get( );
    ragebot = c_ragebot::get( );
    thirdperson = c_thirdperson::get( );
    bunnyhop = c_bunnyhop::get( );
    peek_assist = c_peek_assist::get( );
    bullet_tracers = c_bullet_tracers::get( );
    modulation = c_modulation::get( );
    chams = c_chams::get( );
    aspect_ratio = c_aspect_ratio::get( );
    spoof = c_spoof::get( );
    nuke = c_nuke::get( );
    removals = c_removals::get( );
    fov = c_fov::get( );
    hit_points = c_hit_points::get( );
    inventory = c_inventory::get( );
    fog = c_fog::get( );
    grenade_prediction = c_grenade_prediction::get( );
    prop_setter = c_prop_setter::get( );
    chat_spam = c_chat_spam::get( );
    weapon_exploits = c_weapon_exploits::get( );
    skybox = c_skybox::get( );
    particles = c_particles::get( );
    hit_markers = c_hit_markers::get( );
}

globals_t::globals_t( )
{
    interface = c_interface::get( );
    il2cpp = c_il2cpp_api::get( );

    features_mgr = c_features_manager::get( );

    hooks = c_hooks::get( );

    config = c_config::get( );

    audio_player = c_audio_player::get( );

    texture_manager = c_texture_manager::get( );

    widgets = c_widgets::get( );

    chat = c_chat::get( );

    ws_pranks = c_ws_pranks::get( );
}