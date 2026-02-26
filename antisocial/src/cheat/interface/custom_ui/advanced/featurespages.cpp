//
// Created by Kai Tears on 06/08/2025.
//

#include "featurespages.hpp"

#include "elements.hpp"

#include "config/config.hpp"

#include "game/entities/entities.hpp"

#include "game/sdk/structures/vec3.hpp"

#include "game/features/ragebot/ragebot.hpp"
#include "game/features/inventory/inventory.hpp"
#include "game/features/misc/prop_setter/prop_setter.hpp"

#include "audio_player/audio_player.hpp"

#include "globals.hpp"

#include "interface/widgets/widgets.hpp"

#include "alert/alert.hpp"

#include "sample_renderers/dropped_weapons/dropped_weapons.hpp"
#include "sample_renderers/thrown_grenades/thrown_grenades.hpp"
#include "sample_renderers/bomb/bomb.hpp"
#include "sample_renderers/enemies/enemies.hpp"

#include "../ui.hpp"

#include "image_selector/image_selector.hpp"

using namespace ImGui;

void show_features( int tab, int sub, int child )
{
    switch ( tab )
    {
    case 0: // esp
    {
        switch ( sub )
        {
        case 0:
        {
            if ( child == 1 )
            {
                elem::simple_category( ay_obfuscate( "Preview info" ) );
                {
                    elem::combo( ay_obfuscate( "Girl" ), &c::get< int >( g_ctx->cfg.previews_model_esp ), { crypt_string( "Stella" ), crypt_string( "Ayanami Rei" ) } ); // TBA
                    elem::end_category( );
                }

                elem::toggle( ay_obfuscate( "Offscreen" ), &c::get< bool >( g_ctx->cfg.esp_offscreen ), ay_obfuscate( "Shows enemies that are out of visibility" ) );
                elem::toggle( ay_obfuscate( "Shadows" ), &c::get< bool >( g_ctx->cfg.esp_shadows ), ay_obfuscate( "Shows shadow effect" ) );

                elem::toggle( ay_obfuscate( "Target" ), &c::get< bool >( g_ctx->cfg.esp_target ), ay_obfuscate( "Shows ragebot or legitbot target" ), [ & ]( ) -> void {
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.esp_target_clr ) } );
                } );

                elem::toggle( ay_obfuscate( "Rect" ), &c::get< bool >( g_ctx->cfg.esp_rect ), ay_obfuscate( "Shows rects through walls" ), [ & ]( ) -> void {
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.esp_rect_end ), &c::get< ImVec4 >( g_ctx->cfg.esp_rect_start ) } );
                } );

                elem::toggle( ay_obfuscate( "Nicknames" ), &c::get< bool >( g_ctx->cfg.esp_nickname ), ay_obfuscate( "Shows nicknames through walls" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Profile pictures" ), &c::get< bool >( g_ctx->cfg.esp_profile_pictures ) );
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.esp_nickname_clr_end ), &c::get< ImVec4 >( g_ctx->cfg.esp_nickname_clr ) } );
                    elem::combo( ay_obfuscate( "Font" ), &c::get< int >( g_ctx->cfg.esp_nickname_font ), { crypt_string( "Verdana" ), crypt_string( "SF Pro" ) } );
                } );

                elem::toggle( ay_obfuscate( "Distance" ), &c::get< bool >( g_ctx->cfg.esp_distance ), ay_obfuscate( "Shows distance to enemy" ) );

                elem::simple_category( ay_obfuscate( "Weapon" ) );
                {
                    elem::internal_toggle( ay_obfuscate( "Text" ), &c::get< bool >( g_ctx->cfg.esp_weapon ) );
                    elem::internal_toggle( ay_obfuscate( "Icon" ), &c::get< bool >( g_ctx->cfg.esp_weapon_icon ) );
                    elem::internal_toggle( ay_obfuscate( "Grenades" ), &c::get< bool >( g_ctx->cfg.esp_weapon_grenades ) );
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.esp_weapon_clr ) } );
                    elem::end_category( );
                }

                elem::toggle( ay_obfuscate( "Health" ), &c::get< bool >( g_ctx->cfg.esp_health ), ay_obfuscate( "Shows player's health" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Gradient" ), &c::get< bool >( g_ctx->cfg.esp_health_gradient ) );
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.esp_health_max ), &c::get< ImVec4 >( g_ctx->cfg.esp_health_min ) } );
                } );
                elem::toggle( ay_obfuscate( "Ammo" ), &c::get< bool >( g_ctx->cfg.esp_ammo ), ay_obfuscate( "Shows player's ammo" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Gradient" ), &c::get< bool >( g_ctx->cfg.esp_ammo_gradient ) );
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.esp_ammo_max ), &c::get< ImVec4 >( g_ctx->cfg.esp_ammo_min ) } );
                } );

                elem::simple_category( ay_obfuscate( "Flags" ) );
                {
                    elem::internal_toggle( ay_obfuscate( "Money" ), &c::get< bool >( g_ctx->cfg.esp_money ) );
                    elem::internal_toggle( ay_obfuscate( "Armor" ), &c::get< bool >( g_ctx->cfg.esp_armor ) );
                    elem::internal_toggle( ay_obfuscate( "Scope" ), &c::get< bool >( g_ctx->cfg.esp_scoped ) );
                    if ( g_ctx->user_data.premium )
                        elem::internal_toggle( ay_obfuscate( "Open" ), &c::get< bool >( g_ctx->cfg.esp_open ) );
                    elem::end_category( );
                }

            } else if ( child == 2 )
            {

                elem::simple_category( "Preview" );
                {
                    preview_model_t const selected_model = []( ) -> preview_model_t {
                        switch ( c::get< int >( g_ctx->cfg.previews_model_esp ) )
                        {
                        case 0:
                            return c_user_interface::m_stella;
                        case 1:
                            return c_user_interface::m_ayanami_rei;
                        default:
                            return c_user_interface::m_stella;
                        }
                    }( );

                    ImVec2 area_min = GetWindowPos( ) - ImVec2( 10, 10 );
                    ImVec2 area_max = GetWindowPos( ) + ImVec2( GetWindowSize( ).x, 370 - 116 ) + ImVec2( 10, 10 );
                    ImVec2 area_size = area_max - area_min;

                    ItemSize( ImVec2( 0, area_size.y - 20 ) );

                    float pad_x = 50.0f;
                    float pad_y = 50.0f;

                    float work_w = area_size.x - pad_x * 2.0f;
                    float work_h = area_size.y - pad_y * 2.0f;

                    float img_w = selected_model.w;
                    float img_h = selected_model.h;

                    float scale = fminf( work_w / img_w, work_h / img_h );

                    float final_w = img_w * scale;
                    float final_h = img_h * scale;

                    ImVec2 img_min = area_min + ImVec2( ( area_size.x - final_w ) / 2.0f, ( area_size.y - final_h ) / 2.0f );
                    ImVec2 img_max = img_min + ImVec2( final_w, final_h );

                    if ( c_user_interface::m_preview_base.background.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) c_user_interface::m_preview_base.background.texture_id, area_min, area_max, ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImColor( 1.f, 1.f, 1.f, 0.4f * ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.body.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.body.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( ImColor( 255, 255, 255 ), ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.arms.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.arms.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( ImColor( 255, 255, 255 ), ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.head.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.head.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( ImColor( 255, 255, 255 ), ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.legs.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.legs.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( ImColor( 255, 255, 255 ), ImGui::GetStyle( ).Alpha ) );

                    c_sample_enemies::render( GetWindowDrawList( ), area_min + area_size / 2 );

                    elem::end_category( );
                }
            }
        }
        break;

        case 1:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Dropped weapons" ), &c::get< bool >( g_ctx->cfg.visual_dropped_weapons ), ay_obfuscate( "Shows dropped weapons in world" ), [ & ]( ) -> void {
                    if ( c_sample_dropped_weapons::m_texture.loaded )
                        elem::preview( ( ImTextureID ) c_sample_dropped_weapons::m_texture.texture_id, 84, { { 204 / 2, 84 / 2 }, c_sample_dropped_weapons::render } );
                    elem::internal_toggle( ay_obfuscate( "Show ammo" ), &c::get< bool >( g_ctx->cfg.visual_dropped_weapons_ammo ) );
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_color ), &c::get< ImVec4 >( g_ctx->cfg.visual_dropped_weapons_ammo_color ) } );
                } );

            } else if ( child == 2 )
            {
                elem::toggle( ay_obfuscate( "Planted bomb" ), &c::get< bool >( g_ctx->cfg.visual_planted_bomb ), ay_obfuscate( "Shows planted bomb" ), [ & ]( ) -> void {
                    if ( c_sample_bomb::m_texture.loaded )
                        elem::preview( ( ImTextureID ) c_sample_bomb::m_texture.texture_id, 84, { { 204 / 2, 84 / 2 }, c_sample_bomb::render } );
                } );

                elem::toggle( ay_obfuscate( "Thrown grenades" ), &c::get< bool >( g_ctx->cfg.visual_grenades_esp ), ay_obfuscate( "Shows nades from other players" ), [ & ]( ) -> void {
                    if ( c_sample_thrown_grenades::m_texture.loaded )
                        elem::preview( ( ImTextureID ) c_sample_thrown_grenades::m_texture.texture_id, 84, { { 204 / 2, 84 / 2 }, c_sample_thrown_grenades::render } );
                    elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.visual_grenades_esp_color ) } );
                } );
            }
        }
        break;
        };
    }
    break;

    case 1: // chams
    {
        switch ( sub )
        {
        case 2:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Enemies" ), &c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).enabled, ay_obfuscate( "Apply chams on enemies" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Custom texture" ), &c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).custom_texture );
                    if ( elem::button( ay_obfuscate( "Select image" ) ) )
                    {
                        image::select( crypt_string( "enemies.png" ) );
                    }
                    elem::color( ay_obfuscate( "Colors" ), { &c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).visible_color, &c::get< chams_config_t >( g_ctx->cfg.chams_enemies ).invisible_color } );
                } );
            }
        }
        break;

        case 3:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Local" ), &c::get< chams_config_t >( g_ctx->cfg.chams_local ).enabled, ay_obfuscate( "Apply chams on local player" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Custom texture" ), &c::get< chams_config_t >( g_ctx->cfg.chams_local ).custom_texture );
                    if ( elem::button( ay_obfuscate( "Select image" ) ) )
                    {
                        image::select( crypt_string( "local.png" ) );
                    }
                    elem::color( ay_obfuscate( "Colors" ), { &c::get< chams_config_t >( g_ctx->cfg.chams_local ).visible_color, &c::get< chams_config_t >( g_ctx->cfg.chams_local ).invisible_color } );
                } );
            }
        }
        break;

        case 4:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Weapon" ), &c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).enabled, ay_obfuscate( "Apply chams on local player's weapon" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Custom texture" ), &c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).custom_texture );
                    if ( elem::button( ay_obfuscate( "Fallback image" ) ) )
                    {
                        image::select( crypt_string( "weapon.png" ) );
                    }
                    elem::color( ay_obfuscate( "Colors" ), { &c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).visible_color, &c::get< chams_config_t >( g_ctx->cfg.chams_weapon ).invisible_color } );
                } );
            }
        }
        break;
        };
    }
    break;

    case 2: // world
    {
        if ( child == 1 )
        {
            elem::toggle( ay_obfuscate( "Fog" ), &c::get< bool >( g_ctx->cfg.visual_fog_enabled ), ay_obfuscate( "Fog world effect" ), [ & ]( ) -> void {
                elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.visual_fog_color ) } );
                elem::slider_float( ay_obfuscate( "Density" ), &c::get< float >( g_ctx->cfg.visual_fog_density ), 0, 100 );
                elem::slider_float( ay_obfuscate( "Start" ), &c::get< float >( g_ctx->cfg.visual_fog_start ), 0, 50 );
                elem::slider_float( ay_obfuscate( "End" ), &c::get< float >( g_ctx->cfg.visual_fog_end ), 0, 50 );
                elem::slider_int( ay_obfuscate( "Mode" ), &c::get< int >( g_ctx->cfg.visual_fog_mode ), 0, 2 );
            } );
        } else if ( child == 2 )
        {

            elem::toggle( ay_obfuscate( "Particles" ), &c::get< bool >( g_ctx->cfg.world_particles_enabled ), ay_obfuscate( "3D particles around the world" ), [ & ]( ) -> void {
                elem::internal_toggle( ay_obfuscate( "On hit" ), &c::get< bool >( g_ctx->cfg.visual_hit_particles_enabled ) );
                elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.world_particles_color ) } );
            } );

            elem::toggle( ay_obfuscate( "Skybox" ), &c::get< bool >( g_ctx->cfg.visual_skybox_enabled ), ay_obfuscate( "Changes your skybox" ), [ & ]( ) -> void {
                elem::internal_toggle( ay_obfuscate( "Custom texture" ), &c::get< bool >( g_ctx->cfg.visual_skybox_texture ) );
                if ( elem::button( ay_obfuscate( "Select image" ) ) )
                {
                    image::select( crypt_string( "skybox.png" ) );
                }
                elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.visual_skybox ) } );
            } );

            elem::toggle( ay_obfuscate( "World modulation" ), &c::get< bool >( g_ctx->cfg.world_modulation_auto ), ay_obfuscate( "Modulates your world to selected color" ), [ & ]( ) -> void {
                elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.world_modulation ) } );

                if ( elem::button( ay_obfuscate( "Force" ) ) )
                    c::get< bool >( g_ctx->cfg.world_modulate ) = true;
            } );

            elem::toggle( ay_obfuscate( "Hits' points" ), &c::get< bool >( g_ctx->cfg.visual_hit_points ), ay_obfuscate( "Shows your hits' locations" ) );

            elem::toggle( ay_obfuscate( "Hits' markers" ), &c::get< bool >( g_ctx->cfg.visual_hit_markers ), ay_obfuscate( "Shows damage when enemy is hit" ) );

            elem::toggle( ay_obfuscate( "Bullets' tracers" ), &c::get< bool >( g_ctx->cfg.visual_bullet_tracers ), ay_obfuscate( "Shows your bullets' trajectories" ), [ & ]( ) -> void {
                elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.visual_bullet_tracers_color ) } );
            } );

            elem::toggle( ay_obfuscate( "Grenade prediction" ), &c::get< bool >( g_ctx->cfg.visual_grenade_prediction ), ay_obfuscate( "Shows your grenades' trajectories" ), [ & ]( ) -> void {
                elem::color( ay_obfuscate( "Color" ), { &c::get< ImVec4 >( g_ctx->cfg.visual_grenade_prediction_color ) } );
            } );
        }
    }
    break;

    case 3: // ragebot
    {
        switch ( sub )
        {
        case 6:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Ragebot" ), &c::get< bool >( g_ctx->cfg.ragebot_enabled ), ay_obfuscate( "Powerful way to dominate enemies" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Silent" ), &c::get< bool >( g_ctx->cfg.ragebot_silent ) );
                    elem::internal_toggle( ay_obfuscate( "Triggerbot" ), &c::get< bool >( g_ctx->cfg.ragebot_triggerbot ) );
                    elem::internal_toggle( ay_obfuscate( "Autowall" ), &c::get< bool >( g_ctx->cfg.ragebot_autowall ) );
                    elem::internal_toggle( ay_obfuscate( "Doubletap" ), &c::get< bool >( g_ctx->cfg.ragebot_doubletap ) );
                } );

            } else if ( child == 2 )
            {
                elem::simple_category( ay_obfuscate( "Settings" ) );
                {
                    elem::slider_int( ay_obfuscate( "Minimal damage" ), &c::get< int >( g_ctx->cfg.ragebot_minimal_damage ), 0, 200 );
                    elem::slider_int( ay_obfuscate( "Field of view" ), &c::get< int >( g_ctx->cfg.ragebot_fov ), 0, 240 );
                    elem::end_category( );
                }
            }
        }
        break;

        case 7:
        {
            if ( child == 1 )
            {
                elem::simple_category( ay_obfuscate( "Preview info" ) );
                {
                    elem::combo( ay_obfuscate( "Girl" ), &c::get< int >( g_ctx->cfg.previews_model_hitboxes ), { crypt_string( "Stella" ), crypt_string( "Ayanami Rei" ) } ); // TBA
                    elem::end_category( );
                }

                elem::simple_category( ay_obfuscate( "Selection" ) );
                {
                    elem::internal_toggle( ay_obfuscate( "Legit selection" ), &c::get< bool >( g_ctx->cfg.ragebot_legit_selection ) );
                    elem::internal_toggle( ay_obfuscate( "Head" ), &c::get< bool >( g_ctx->cfg.ragebot_head ) );
                    elem::internal_toggle( ay_obfuscate( "Body" ), &c::get< bool >( g_ctx->cfg.ragebot_body ) );
                    elem::internal_toggle( ay_obfuscate( "Arms" ), &c::get< bool >( g_ctx->cfg.ragebot_arms ) );
                    elem::internal_toggle( ay_obfuscate( "Legs" ), &c::get< bool >( g_ctx->cfg.ragebot_legs ) );
                    elem::end_category( );
                }

                elem::toggle( ay_obfuscate( "Multipoints" ), &c::get< bool >( g_ctx->cfg.ragebot_multipoints ), ay_obfuscate( "Advanced target selection technology" ), [ & ]( ) -> void {
                    elem::slider_float( ay_obfuscate( "Tightness" ), &c::get< float >( g_ctx->cfg.ragebot_multipoints_tightness ), 0.1f, 2.0f );
                    elem::slider_float( ay_obfuscate( "Head scale" ), &c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_head ), 0.1f, 2.0f );
                    elem::slider_float( ay_obfuscate( "Body scale" ), &c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_body ), 0.1f, 2.0f );
                    elem::slider_float( ay_obfuscate( "Arms scale" ), &c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_arms ), 0.1f, 2.0f );
                    elem::slider_float( ay_obfuscate( "Legs scale" ), &c::get< float >( g_ctx->cfg.ragebot_multipoints_scale_legs ), 0.1f, 2.0f );
                } );

            } else if ( child == 2 )
            {
                elem::simple_category( "Preview" );
                {
                    preview_model_t const selected_model = []( ) -> preview_model_t {
                        switch ( c::get< int >( g_ctx->cfg.previews_model_hitboxes ) )
                        {
                        case 0:
                            return c_user_interface::m_stella;
                        case 1:
                            return c_user_interface::m_ayanami_rei;
                        default:
                            return c_user_interface::m_stella;
                        }
                    }( );

                    ImVec2 area_min = GetWindowPos( ) - ImVec2( 10, 10 );
                    ImVec2 area_max = GetWindowPos( ) + ImVec2( GetWindowSize( ).x, 370 - 116 ) + ImVec2( 10, 10 );
                    ImVec2 area_size = area_max - area_min;

                    static struct
                    {
                        c_animator body { ImVec4( 0, 0, 0, 0 ) };
                        c_animator head { ImVec4( 0, 0, 0, 0 ) };
                        c_animator arms { ImVec4( 0, 0, 0, 0 ) };
                        c_animator legs { ImVec4( 0, 0, 0, 0 ) };
                    } state;

                    auto const animate = [ & ]( bool checker, c_animator& anim ) -> void {
                        ImColor color = ImColor( 255, 255, 255 ).Value;

                        if ( checker )
                        {
                            if ( c::get< bool >( g_ctx->cfg.ragebot_enabled ) && c::get< bool >( g_ctx->cfg.ragebot_multipoints ) )
                            {
                                color = ImColor( 255, 25, 25 ).Value;
                            } else if ( c::get< bool >( g_ctx->cfg.ragebot_legit_selection ) && c::get< bool >( g_ctx->cfg.ragebot_enabled ) && c::get< int >( g_ctx->cfg.ragebot_fov ) > 0 )
                            {
                                color = ImColor( 100, 255, 90 ).Value;
                            } else
                            {
                                color = ImColor( 120, 123, 173 ).Value;
                            }
                        }

                        anim.update< ImVec4 >( color.Value );
                    };

                    animate( c::get< bool >( g_ctx->cfg.ragebot_body ), state.body );
                    animate( c::get< bool >( g_ctx->cfg.ragebot_head ), state.head );
                    animate( c::get< bool >( g_ctx->cfg.ragebot_arms ), state.arms );
                    animate( c::get< bool >( g_ctx->cfg.ragebot_legs ), state.legs );

                    ItemSize( ImVec2( 0, area_size.y - 20 ) );

                    float pad_x = 30.0f;
                    float pad_y = 30.0f;

                    float work_w = area_size.x - pad_x * 2.0f;
                    float work_h = area_size.y - pad_y * 2.0f;

                    float img_w = selected_model.w;
                    float img_h = selected_model.h;

                    float scale = fminf( work_w / img_w, work_h / img_h );

                    float final_w = img_w * scale;
                    float final_h = img_h * scale;

                    ImVec2 img_min = area_min + ImVec2( ( area_size.x - final_w ) / 2.0f, ( area_size.y - final_h ) / 2.0f );
                    ImVec2 img_max = img_min + ImVec2( final_w, final_h );

                    if ( c_user_interface::m_preview_base.background.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) c_user_interface::m_preview_base.background.texture_id, area_min, area_max, ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImColor( 1.f, 1.f, 1.f, 0.4f * ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.body.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.body.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( state.body.get< ImVec4 >( ), ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.arms.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.arms.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( state.arms.get< ImVec4 >( ), ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.head.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.head.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( state.head.get< ImVec4 >( ), ImGui::GetStyle( ).Alpha ) );

                    if ( selected_model.legs.loaded )
                        GetWindowDrawList( )->AddImage( ( ImTextureID ) selected_model.legs.texture_id, img_min, img_max, { 0, 0 }, { 1, 1 }, c::alpha_interpolate( state.legs.get< ImVec4 >( ), ImGui::GetStyle( ).Alpha ) );

                    elem::end_category( );
                }
            }
        }
        break;

        case 8:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Anti-Aim" ), &c::get< bool >( g_ctx->cfg.ragebot_antiaim ), ay_obfuscate( "Changes your player's look trajectory" ), [ & ]( ) -> void {
                    elem::internal_toggle( ay_obfuscate( "Manuals" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).manuals );

                    elem::internal_toggle( ay_obfuscate( "Jitter" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter );
                    elem::internal_toggle( ay_obfuscate( "Defensive in air" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_in_air );
                    elem::internal_toggle( ay_obfuscate( "Ranged jitter" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_ranged );
                } );

            } else if ( child == 2 )
            {
                elem::simple_category( ay_obfuscate( "Settings" ) );
                {
                    elem::combo( ay_obfuscate( "Pitch" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).pitch_type, { crypt_string( "Default" ), crypt_string( "Down" ), crypt_string( "Up" ) } );
                    elem::combo( ay_obfuscate( "Yaw" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).yaw_type, { crypt_string( "Default" ), crypt_string( "Backwards" ), crypt_string( "Forwards" ) } );
                    elem::combo( ay_obfuscate( "Yaw select" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).yaw_select, { crypt_string( "At local" ), crypt_string( "At enemy" ) } );

                    if ( c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_ranged )
                        elem::slider_int( ay_obfuscate( "Jitter range" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_range, 1, 50 );
                    else
                    {
                        elem::slider_int( ay_obfuscate( "Yaw 1" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_first, -180, 180 );
                        elem::slider_int( ay_obfuscate( "Yaw 2" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_second, -180, 180 );
                    }
                    elem::slider_int( ay_obfuscate( "Jitter ticks*" ), &c::get< antiaim_config_t >( g_ctx->cfg.ragebot_antiaim_config ).jitter_frames, 0, 25 );

                    elem::end_category( );
                }
            }
        }
        break;

        case 9:
        {
            if ( g_ctx->user_data.premium )
            {
                if ( child == 1 )
                {
                    elem::simple_category( ay_obfuscate( "Exploits" ) );
                    {
                        elem::internal_toggle( ay_obfuscate( "Infinite ammo" ), &c::get< bool >( g_ctx->cfg.exploits_infinite_ammo ) );
                        elem::internal_toggle( ay_obfuscate( "Rapid fire" ), &c::get< bool >( g_ctx->cfg.exploits_rapid_fire ) );
                        elem::internal_toggle( ay_obfuscate( "No spread" ), &c::get< bool >( g_ctx->cfg.exploits_no_spread ) );
                        elem::end_category( );
                    }

                } else if ( child == 2 )
                {
                    elem::toggle( ay_obfuscate( "Fake lags" ), &c::get< bool >( g_ctx->cfg.ragebot_fakelags ), ay_obfuscate( "Fake-lag yourself" ), [ & ]( ) -> void {
                        elem::slider_int( ay_obfuscate( "Rate" ), &c::get< int >( g_ctx->cfg.ragebot_fakelags_rate ), 0, 10 );
                    } );

                    elem::toggle( ay_obfuscate( "Nuke" ), &c::get< bool >( g_ctx->cfg.misc_nuke ), ay_obfuscate( "Nukes the fuck out of your enemies" ), [ & ]( ) -> void {
                        elem::combo( ay_obfuscate( "Type" ), &c::get< int >( g_ctx->cfg.misc_nuke_type ), { crypt_string( "HE" ), crypt_string( "Flash" ), crypt_string( "Smoke" ) } );
                    } );
                }
            }
        }
        break;
        };
    }
    break;

    case 4: // misc
    {
        switch ( sub )
        {
        case 10:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "3rd persion" ), &c::get< bool >( g_ctx->cfg.misc_thirdperson ), ay_obfuscate( "Third person way of view" ) );
                if ( g_ctx->user_data.premium )
                {
                    elem::toggle( ay_obfuscate( "Peek assist" ), &c::get< bool >( g_ctx->cfg.misc_peek_assist ), ay_obfuscate( "Auto-peeker. Helping you to peek" ) );
                }

                elem::toggle( ay_obfuscate( "Hit sound" ), &c::get< bool >( g_ctx->cfg.misc_hit_sound ), ay_obfuscate( "Plays a sound when enemy is hit" ) );

            } else if ( child == 2 )
            {
                elem::toggle( ay_obfuscate( "Bunnyhop" ), &c::get< bool >( g_ctx->cfg.movement_bunnyhop ), ay_obfuscate( "Allows you to move faster in-jump" ) );

                elem::toggle( ay_obfuscate( "Aspect ratio" ), &c::get< bool >( g_ctx->cfg.misc_aspect_ratio_enabled ), ay_obfuscate( "Changes your screen ratio" ), [ & ]( ) -> void {
                    elem::slider_float( ay_obfuscate( "Ratio" ), &c::get< float >( g_ctx->cfg.misc_aspect_ratio ), 0.1f, 4.0f );
                } );

                elem::toggle( ay_obfuscate( "Field of view" ), &c::get< bool >( g_ctx->cfg.misc_fov_enabled ), ay_obfuscate( "Changes your player's field of view" ), [ & ]( ) -> void {
                    elem::slider_int( ay_obfuscate( "Value" ), &c::get< int >( g_ctx->cfg.misc_fov ), 40, 120 );
                } );

                elem::simple_category( ay_obfuscate( "Removals" ) );
                {
                    elem::internal_toggle( ay_obfuscate( "Scope overlay" ), &c::get< bool >( g_ctx->cfg.removals_scope_overlay ) );
                    elem::end_category( );
                }
            }
        }
        break;

        case 11:
        {
            if ( g_ctx->user_data.premium )
            {
                if ( child == 1 )
                {
                    elem::toggle( ay_obfuscate( "Spoof ID" ), &c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_uid ).enabled, ay_obfuscate( "Spoofs your user id to desired value. Default - antisocial" ), [ & ]( ) -> void {
                        Text( c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_uid ).spoof.c_str( ) );
                        if ( elem::button( "Set custom ID" ) )
                            alert::create( crypt_string( "Enter ID" ), crypt_string( "OK" ), crypt_string( "Spoof" ), c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_uid ).spoof );
                    } );

                } else if ( child == 2 )
                {
                    elem::toggle( ay_obfuscate( "Spoof clan" ), &c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_clan ).enabled, ay_obfuscate( "Spoofs your clan tag to desired value. Default - antisocial" ), [ & ]( ) -> void {
                        Text( c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_clan ).spoof.c_str( ) );
                        if ( elem::button( ay_obfuscate( "Set custom tag" ) ) )
                            alert::create( crypt_string( "Enter Tag" ), crypt_string( "OK" ), crypt_string( "Spoof" ), c::get< spoof_config_t >( g_ctx->cfg.misc_spoof_clan ).spoof );
                    } );
                }
            }
        }
        break;

        case 12:
        {
            if ( child == 1 )
            {
                elem::toggle( ay_obfuscate( "Trash talk" ), &c::get< bool >( g_ctx->cfg.misc_trash_talk ), ay_obfuscate( "Trash talks when you hit enemy" ) );

            } else if ( child == 2 )
            {
                elem::toggle( ay_obfuscate( "IRC chat" ), &c::get< bool >( g_ctx->cfg.misc_irc_chat ), ay_obfuscate( "Allow you to communicate with other cheat users by using @ prefix in your message" ) );

                if ( g_ctx->user_data.premium )
                    elem::toggle( ay_obfuscate( "Chat spam" ), &c::get< bool >( g_ctx->cfg.misc_chat_spam ), ay_obfuscate( "Spams cheat advertising in chat" ) );
            }
        }
        break;
        };
    }
    break;

    case 5: // config
    {
        switch ( sub )
        {
        case 13:
        {
            if ( child == 1 )
            {
                g_ctx->config->on_gui( 1 );
            } else if ( child == 2 )
            {
                g_ctx->config->on_gui( 2 );
            }
        }
        break;

        case 14:
        {
            if ( child == 1 )
            {
                elem::simple_category( ay_obfuscate( "Inventory" ) );
                {
                    if ( elem::button( ay_obfuscate( "Save" ) ) )
                    {
                        g_ctx->features.inventory->save_inventory( );
                    }

                    if ( elem::button( ay_obfuscate( "Load" ) ) )
                    {
                        g_ctx->features.inventory->load_inventory( );
                    }

                    elem::end_category( );
                }
            }
        }
        break;
        };
    }
    break;

    case 6: // inventory
    {
        if ( child == 1 )
        {
            g_ctx->features.inventory->on_gui( 1 );
        } else if ( child == 2 )
        {
            g_ctx->features.inventory->on_gui( 2 );
        }
    }
    break;
    }
}