#include "enemies.hpp"

#include <string>

#include "game/features/visual/esp/esp.hpp"
#include "game/features/visual/esp/players/players.hpp"
#include "interface/custom_ui/advanced/utilities.hpp"

#include "globals.hpp"

void c_sample_enemies::render( ImDrawList* draw_list, const ImVec2& center )
{
    ImDrawListSplitter splitter;
    splitter.Split( draw_list, 2 );

    splitter.SetCurrentChannel( draw_list, 1 );
    auto old_flags = draw_list->Flags;
    draw_list->Flags &= ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;

    auto const uuid = crypt_string( "internal_preview_player_uid" );
    auto& param = c_ent_player_esp::m_parameters[ uuid ];

    param.alpha.set< float >( ImGui::GetStyle( ).Alpha );

    auto const pfp_texture = ( g_ctx->user_data.profile_picture.loaded && c::get< bool >( g_ctx->cfg.esp_profile_pictures ) ) ? ( ImTextureID ) g_ctx->user_data.profile_picture.texture_id : 0;

    auto const bounds = ImRect(
        { static_cast< float >( static_cast< int >( center.x - 50 ) ), static_cast< float >( static_cast< int >( center.y - 90 ) ) },
        { static_cast< float >( static_cast< int >( center.x + 50 ) ), static_cast< float >( static_cast< int >( center.y + 90 ) ) } );

    auto const is_ammo_available = []( c_player_controller* const ent ) -> short {
        return true;
    };

    auto const clip_pair = std::pair< short, short >( 3, 5 );

    ent_grenades_t grenades = {
        1,
        1,
        1 };

    esp_ent_t generated = {
        { 0, 0, 0 }, bounds, uuid, false, 80, clip_pair.first, clip_pair.second, 51, crypt_string( "@antisocial_pub" ), grenades };

    c_ent_player_esp esp( generated, draw_list );

    if ( c::get< bool >( g_ctx->cfg.esp_rect ) )
        esp.render_rect( );

    esp.render_healthbar( );

    esp.render_ammobar( true );

    esp.render_nickname( pfp_texture, pfp_texture );

    std::vector< esp_flag_t > temp_flags { };

    if ( c::get< bool >( g_ctx->cfg.esp_armor ) )
    {
        auto const armor = 100;
        auto const helmet = true;

        if ( helmet || armor > 0 )
        {
            std::string armor_value { };

            if ( helmet )
                armor_value += crypt_string( "H" );

            if ( armor > 0 )
                armor_value += crypt_string( "K" );

            temp_flags.push_back( { armor_value, ImColor( 255, 255, 255 ) } );
        }
    }

    if ( c::get< bool >( g_ctx->cfg.esp_money ) )
    {
        auto const money = 1337;

        temp_flags.push_back( { std::to_string( money ) + "$", ImColor( 55, 200, 0 ) } );
    }

    if ( c::get< bool >( g_ctx->cfg.esp_scoped ) )
    {
        temp_flags.push_back( { crypt_string( "ZOOM" ), ImColor( 25, 155, 255 ) } );
    }

    if ( c::get< bool >( g_ctx->cfg.esp_open ) && g_ctx->user_data.premium )
    {
        temp_flags.push_back( { crypt_string( "HIT" ), ImColor( 255, 15, 15 ) } );
    }

    esp.render_weapon( );
    esp.render_flags( temp_flags );

    draw_list->Flags = old_flags;
    splitter.Merge( draw_list );
}
