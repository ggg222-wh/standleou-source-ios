#include "config.hpp"

#include <fstream>
#include <random>

#include "data.hpp"
#include "base64.hpp"

#include "thirdparty/json/json.hpp"
#include "game/sdk/structures/vec3.hpp"

#include "interface/custom_ui/advanced/elements.hpp"
#include "interface/widgets/widgets.hpp"

#include "networking/networking.hpp"
#include "alert/alert.hpp"

#include "imgui.h"

#include "globals.hpp"
#include <charconv>

void pack_vec3( nlohmann::json& config, const vec3_t& value )
{
    config[ crypt_string( "x" ) ] = value.x;
    config[ crypt_string( "y" ) ] = value.y;
    config[ crypt_string( "z" ) ] = value.z;
}

void pack_vec2( nlohmann::json& config, const ImVec2& value )
{
    config[ crypt_string( "x" ) ] = value.x;
    config[ crypt_string( "y" ) ] = value.y;
}

vec3_t get_vec3( nlohmann::json config )
{
    float x, y, z;

    x = config[ crypt_string( "x" ) ].get< float >( );
    y = config[ crypt_string( "y" ) ].get< float >( );
    z = config[ crypt_string( "z" ) ].get< float >( );

    return { x, y, z };
}

ImVec2 get_vec2( nlohmann::json config )
{
    float x, y;

    x = config[ crypt_string( "x" ) ].get< float >( );
    y = config[ crypt_string( "y" ) ].get< float >( );

    return { x, y };
}

void pack_vec4( nlohmann::json& config, const ImVec4& value )
{
    config[ crypt_string( "x" ) ] = value.x;
    config[ crypt_string( "y" ) ] = value.y;
    config[ crypt_string( "z" ) ] = value.z;
    config[ crypt_string( "w" ) ] = value.w;
}

ImVec4 get_vec4( nlohmann::json config )
{
    float r, g, b, a;

    r = config[ crypt_string( "x" ) ].get< float >( );
    g = config[ crypt_string( "y" ) ].get< float >( );
    b = config[ crypt_string( "z" ) ].get< float >( );
    a = config[ crypt_string( "w" ) ].get< float >( );

    return { r, g, b, a };
}

void pack_antiaim_config( nlohmann::json& config, const antiaim_config_t& value )
{
    config[ crypt_string( "jitter" ) ] = value.jitter;
    config[ crypt_string( "jitter_ranged" ) ] = value.jitter_ranged;
    config[ crypt_string( "jitter_in_air" ) ] = value.jitter_in_air;
    config[ crypt_string( "jitter_frames" ) ] = value.jitter_frames;
    config[ crypt_string( "jitter_range" ) ] = value.jitter_range;
    config[ crypt_string( "jitter_first" ) ] = value.jitter_first;
    config[ crypt_string( "jitter_second" ) ] = value.jitter_second;

    config[ crypt_string( "pitch_type" ) ] = value.pitch_type;
    config[ crypt_string( "yaw_type" ) ] = value.yaw_type;
    config[ crypt_string( "yaw_select" ) ] = value.yaw_select;
    config[ crypt_string( "manuals" ) ] = value.manuals;
}

antiaim_config_t get_antiaim_config( const nlohmann::json& config )
{
    antiaim_config_t value;

    value.jitter = config[ crypt_string( "jitter" ) ].get< bool >( );
    value.jitter_ranged = config[ crypt_string( "jitter_ranged" ) ].get< bool >( );
    value.jitter_in_air = config[ crypt_string( "jitter_in_air" ) ].get< bool >( );
    value.jitter_frames = config[ crypt_string( "jitter_frames" ) ].get< int >( );
    value.jitter_range = config[ crypt_string( "jitter_range" ) ].get< int >( );
    value.jitter_first = config[ crypt_string( "jitter_first" ) ].get< int >( );
    value.jitter_second = config[ crypt_string( "jitter_second" ) ].get< int >( );

    value.pitch_type = config[ crypt_string( "pitch_type" ) ].get< int >( );
    value.yaw_type = config[ crypt_string( "yaw_type" ) ].get< int >( );
    value.yaw_select = config[ crypt_string( "yaw_select" ) ].get< int >( );
    value.manuals = config[ crypt_string( "manuals" ) ].get< bool >( );

    return value;
}

void pack_spoof_config( nlohmann::json& config, const spoof_config_t& value )
{
    config[ crypt_string( "enabled" ) ] = value.enabled;
    config[ crypt_string( "spoof" ) ] = value.spoof;
}

spoof_config_t get_spoof_config( const nlohmann::json& config )
{
    spoof_config_t value;

    value.enabled = config[ crypt_string( "enabled" ) ].get< bool >( );
    value.spoof = config[ crypt_string( "spoof" ) ].get< std::string >( );

    return value;
}

void pack_chams_config( nlohmann::json& config, const chams_config_t& value )
{
    config[ crypt_string( "enabled" ) ] = value.enabled;
    config[ crypt_string( "custom_texture" ) ] = value.custom_texture;

    pack_vec4( config[ crypt_string( "invisible_color" ) ], value.invisible_color );
    pack_vec4( config[ crypt_string( "visible_color" ) ], value.visible_color );
    pack_vec4( config[ crypt_string( "outline_invisible_color" ) ], value.outline_invisible_color );
    pack_vec4( config[ crypt_string( "outline_visible_color" ) ], value.outline_visible_color );
    pack_vec4( config[ crypt_string( "glow_invisible_color" ) ], value.glow_invisible_color );
    pack_vec4( config[ crypt_string( "glow_visible_color" ) ], value.glow_visible_color );

    config[ crypt_string( "glow_width" ) ] = value.glow_width;
    config[ crypt_string( "shader_type" ) ] = value.shader_type;
    config[ crypt_string( "remove_texture" ) ] = value.remove_texture;
    config[ crypt_string( "outline_thickness" ) ] = value.outline_thickness;
    config[ crypt_string( "time_speed" ) ] = value.time_speed;
    config[ crypt_string( "pixel_density" ) ] = value.pixel_density;
}

chams_config_t get_chams_config( const nlohmann::json& config )
{
    chams_config_t value;

    value.enabled = config[ crypt_string( "enabled" ) ].get< bool >( );
    value.custom_texture = config[ crypt_string( "custom_texture" ) ].get< bool >( );

    value.invisible_color = get_vec4( config[ crypt_string( "invisible_color" ) ] );
    value.visible_color = get_vec4( config[ crypt_string( "visible_color" ) ] );
    value.outline_invisible_color = get_vec4( config[ crypt_string( "outline_invisible_color" ) ] );
    value.outline_visible_color = get_vec4( config[ crypt_string( "outline_visible_color" ) ] );
    value.glow_invisible_color = get_vec4( config[ crypt_string( "glow_invisible_color" ) ] );
    value.glow_visible_color = get_vec4( config[ crypt_string( "glow_visible_color" ) ] );

    value.glow_width = config[ crypt_string( "glow_width" ) ].get< float >( );
    value.shader_type = config[ crypt_string( "shader_type" ) ].get< int >( );
    value.remove_texture = config[ crypt_string( "remove_texture" ) ].get< bool >( );
    value.outline_thickness = config[ crypt_string( "outline_thickness" ) ].get< float >( );
    value.time_speed = config[ crypt_string( "time_speed" ) ].get< float >( );
    value.pixel_density = config[ crypt_string( "pixel_density" ) ].get< float >( );

    return value;
}

std::size_t get_variable_idx( const fnv1a_t m_name_hash )
{
    for ( size_t i = 0U; i < c::variables.size( ); i++ )
    {
        if ( c::variables.at( i ).m_name_hash == m_name_hash )
            return i;
    }

    return ( std::size_t ) ( -1 );
}

std::string c_config::to_string( ) const
{
    nlohmann::json settings { };

    try
    {
        nlohmann::json variables_json = nlohmann::json::array( );

        for ( auto& variable : c::variables )
        {
            nlohmann::json entry = { };

            entry[ crypt_string( "name-id" ) ] = variable.m_name_hash;
            entry[ crypt_string( "type-id" ) ] = variable.m_type_hash;

            switch ( variable.m_type_hash )
            {
            case fnv1a::hash_const( "int" ):
            {
                entry[ crypt_string( "value" ) ] = variable.get< int >( );

                break;
            }
            case fnv1a::hash_const( "float" ):
            {
                entry[ crypt_string( "value" ) ] = variable.get< float >( );

                break;
            }
            case fnv1a::hash_const( "bool" ):
            {
                entry[ crypt_string( "value" ) ] = variable.get< bool >( );

                break;
            }
            case fnv1a::hash_const( "std::string" ):
            {
                entry[ crypt_string( "value" ) ] = variable.get< std::string >( );

                break;
            }
            case fnv1a::hash_const( "ImVec4" ):
            {
                pack_vec4( entry[ crypt_string( "value" ) ], variable.get< ImVec4 >( ) );

                break;
            }
            case fnv1a::hash_const( "ImVec2" ):
            {
                pack_vec2( entry[ crypt_string( "value" ) ], variable.get< ImVec2 >( ) );

                break;
            }
            case fnv1a::hash_const( "vec3_t" ):
            {
                pack_vec3( entry[ crypt_string( "value" ) ], variable.get< vec3_t >( ) );

                break;
            }
            case fnv1a::hash_const( "antiaim_config_t" ):
            {
                pack_antiaim_config( entry[ crypt_string( "value" ) ], variable.get< antiaim_config_t >( ) );

                break;
            }
            case fnv1a::hash_const( "spoof_config_t" ):
            {
                pack_spoof_config( entry[ crypt_string( "value" ) ], variable.get< spoof_config_t >( ) );

                break;
            }
            case fnv1a::hash_const( "chams_config_t" ):
            {
                pack_chams_config( entry[ crypt_string( "value" ) ], variable.get< chams_config_t >( ) );

                break;
            }
            default:
                break;
            }

            variables_json.push_back( entry );
        }

        settings[ crypt_string( "variables" ) ] = variables_json;
    } catch ( const nlohmann::detail::exception& ex )
    {
        return "";
    }

    return base64::base64_encode( settings.dump( 4 ) );
}

void c_config::load_from_string( const std::string& data ) const
{
    if ( strlen( data.c_str( ) ) < 10 )
        return;

    const std::string input = base64::base64_decode( data );

    nlohmann::json settings = nlohmann::json::parse( input );

    if ( settings.is_discarded( ) )
        return;

    try
    {
        for ( const nlohmann::json variable : settings[ crypt_string( "variables" ) ] )
        {
            const std::size_t m_idx = get_variable_idx( variable[ crypt_string( "name-id" ) ].get< fnv1a_t >( ) );

            if ( m_idx == ( std::size_t ) ( -1 ) )
                continue;

            auto& entry = c::variables.at( m_idx );

            switch ( variable[ crypt_string( "type-id" ) ].get< fnv1a_t >( ) )
            {
            case fnv1a::hash_const( "bool" ):
            {
                entry.set< bool >( variable[ crypt_string( "value" ) ].get< bool >( ) );

                break;
            }
            case fnv1a::hash_const( "float" ):
            {
                entry.set< float >( variable[ crypt_string( "value" ) ].get< float >( ) );

                break;
            }
            case fnv1a::hash_const( "int" ):
            {
                entry.set< int >( variable[ crypt_string( "value" ) ].get< int >( ) );

                break;
            }
            case fnv1a::hash_const( "std::string" ):
            {
                entry.set< std::string >( variable[ crypt_string( "value" ) ].get< std::string >( ) );

                break;
            }
            case fnv1a::hash_const( "ImVec4" ):
            {
                entry.set< ImVec4 >( get_vec4( variable[ crypt_string( "value" ) ] ) );

                break;
            }
            case fnv1a::hash_const( "ImVec2" ):
            {
                entry.set< ImVec2 >( get_vec2( variable[ crypt_string( "value" ) ] ) );

                break;
            }
            case fnv1a::hash_const( "vec3_t" ):
            {
                entry.set< vec3_t >( get_vec3( variable[ crypt_string( "value" ) ] ) );

                break;
            }
            case fnv1a::hash_const( "antiaim_config_t" ):
            {
                entry.set< antiaim_config_t >( get_antiaim_config( variable[ crypt_string( "value" ) ] ) );

                break;
            }
            case fnv1a::hash_const( "spoof_config_t" ):
            {
                entry.set< spoof_config_t >( get_spoof_config( variable[ crypt_string( "value" ) ] ) );

                break;
            }
            case fnv1a::hash_const( "chams_config_t" ):
            {
                entry.set< chams_config_t >( get_chams_config( variable[ crypt_string( "value" ) ] ) );

                break;
            }

            default:
                break;
            }
        }
    } catch ( const nlohmann::detail::exception& ex )
    {
        return;
    }
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
c_config::create( const std::string& name )
{
    std::map< std::string, std::string > query = {
        { crypt_string( "game" ), crypt_string( "standleo" ) },
        { crypt_string( "user_id" ), g_ctx->user_data.telegram_id },
        { crypt_string( "name" ), name },
        { crypt_string( "allow_sharing" ), crypt_string( "0" ) } };

    std::string base64_body = to_string( );

    const std::string create_request = network::requests( )->post_base64(
        crypt_string( "cloud/configs/manage/create" ),
        base64_body,
        query );

    m_pending_refresh = true;
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
c_config::update( const int& id )
{
    std::map< std::string, std::string > query = {
        { crypt_string( "user_id" ), g_ctx->user_data.telegram_id },
        { crypt_string( "config_id" ), std::to_string( id ) } };

    std::string base64_body = to_string( );

    const std::string update_request = network::requests( )->post_base64(
        crypt_string( "cloud/configs/manage/update" ),
        base64_body,
        query );
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
c_config::remove( const int& id )
{
    const std::string delete_request = network::requests( )->end_point(
        crypt_string( "cloud/configs/manage/delete?config_id=" ) + std::to_string( id ) + crypt_string( "&user_id=" ) + g_ctx->user_data.telegram_id );

    m_pending_refresh = true;
}

__attribute__( ( annotate( "fla" ) ) )
__attribute__( ( annotate( "strenc" ) ) ) void
c_config::load( const int& id )
{
    const std::string server_response = network::requests( )->end_point(
        crypt_string( "cloud/configs/get?game=standleo&config_id=" ) + std::to_string( id ) +
        crypt_string( "&user_id=" ) + g_ctx->user_data.telegram_id );

    if ( server_response.find( crypt_string( "detail" ) ) != std::string::npos )
    {
        try
        {
            nlohmann::json response = nlohmann::json::parse( server_response );
            std::string const parsed = response[ crypt_string( "detail" ) ].get< std::string >( );

            if ( parsed.find( crypt_string( "Config is private" ) ) != std::string::npos )
            {
                return;
            }

        } catch ( const nlohmann::json::parse_error& )
        {
        }
    } else
    {
        if ( server_response.find( crypt_string( "configdata" ) ) != std::string::npos )
        {
            try
            {
                nlohmann::json response = nlohmann::json::parse( server_response );

                load_from_string( response[ crypt_string( "configdata" ) ].get< std::string >( ) );
            } catch ( const nlohmann::json::parse_error& )
            {
            }
        }
    }
}

void c_config::on_gui( int child )
{
    static int selected_cfg = -1;

    auto const refresh_configs = [ & ]( ) -> void {
        const std::string configs_utf8 = network::requests( )->end_point( crypt_string( "cloud/configs/get/all?game=standleo&user_id=" ) + g_ctx->user_data.telegram_id );

        selected_cfg = -1;

        m_configs.clear( );

        try
        {
            const nlohmann::json data = nlohmann::json::parse( configs_utf8 );

            if ( data.is_discarded( ) )
                return;

            for ( auto& it : data[ crypt_string( "configs" ) ] )
            {
                if ( it.is_discarded( ) )
                    continue;

                m_configs[ it[ crypt_string( "id" ) ].get< int >( ) ] = it[ crypt_string( "name" ) ].get< std::string >( );
            }
        } catch ( const nlohmann::detail::exception& ex )
        {
            return;
        }
    };

    switch ( child )
    {
    case 1:
    {
        if ( m_pending_refresh )
        {
            refresh_configs( );

            m_pending_refresh = false;
        }

        elem::simple_category( ay_obfuscate( "Your configs" ) );
        {
            struct _state
            {
                c_animator text { ImVec4( 0.f, 0.f, 0.f, 0.f ) };
            };

            static std::unordered_map< ImGuiID, _state > __state;

            if ( !m_configs.empty( ) )
            {
                std::for_each( m_configs.begin( ), m_configs.end( ), []( const std::pair< int, std::string > pair ) {
                    int const idx = pair.first;
                    std::string const name = pair.second;

                    std::string const fmt = std::to_string( idx ) + crypt_string( ". " ) + name;

                    _state& state = __state[ ImGui::GetID( fmt.c_str( ) ) ];

                    state.text.update( ( selected_cfg == idx )
                                           ? ImColor( 234, 193, 251 ).Value
                                           : ImColor( 255, 255, 255 ).Value );

                    ImGui::TextColored( state.text.get< ImVec4 >( ), fmt.c_str( ) );

                    if ( ImGui::IsItemClicked( ) )
                        selected_cfg = idx;
                } );
            }
        }
        elem::end_category( );
    }
    break;
    case 2:
    {
        elem::simple_category( ay_obfuscate( "Controls" ) );
        {
            if ( elem::button( ay_obfuscate( "Refresh" ) ) )
            {
                m_pending_refresh = true;
            }

            if ( elem::button( ay_obfuscate( "Load custom" ) ) )
            {
                alert::create_with_task(
                    crypt_string( "Custom config" ),
                    crypt_string( "OK" ),
                    crypt_string( "ID" ),
                    [ & ]( const std::string& key ) {
                        int numeric_idx { };
                        auto [ ptr, ec ] = std::from_chars( key.data( ), key.data( ) + key.size( ), numeric_idx );
                        if ( ec == std::errc( ) )
                        {
                            load( numeric_idx );
                            g_ctx->widgets->update( );
                            selected_cfg = -1;
                        }
                    } );
            }

            if ( elem::button( ay_obfuscate( "Generate" ) ) )
            {
                const static std::string characters = crypt_string( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" );

                std::random_device rd;
                std::mt19937 generator( rd( ) );

                std::uniform_int_distribution<> distribution( 0, characters.size( ) - 1 );

                std::string random_string;
                for ( int i = 0; i < 14; ++i )
                {
                    random_string += characters[ distribution( generator ) ];
                }

                g_ctx->widgets->save( );
                create( random_string );
            }

            // if ( elem::button( ay_obfuscate( "Load custom" ) ) )
            // {
            //     // todo
            // }

            if ( !m_configs.empty( ) && selected_cfg != -1 )
            {
                static bool delete_confirmation = false;
                if ( elem::button( delete_confirmation ? ay_obfuscate( "Confirm deleting" ) : ay_obfuscate( "Delete" ) ) )
                {
                    if ( !delete_confirmation )
                        delete_confirmation = true;
                    else
                    {
                        remove( selected_cfg );
                        selected_cfg = -1;
                        delete_confirmation = false;
                    }
                }

                static bool update_confirmation = false;
                if ( elem::button( update_confirmation ? ay_obfuscate( "Confirm updating" ) : ay_obfuscate( "Update" ) ) )
                {
                    if ( !update_confirmation )
                        update_confirmation = true;
                    else
                    {
                        g_ctx->widgets->save( );
                        update( selected_cfg );
                        selected_cfg = -1;
                        update_confirmation = false;
                    }
                }

                if ( elem::button( ay_obfuscate( "Load" ) ) )
                {
                    load( selected_cfg );
                    g_ctx->widgets->update( );
                    selected_cfg = -1;
                }
            }

            elem::end_category( );
        }
    }
    break;
    default:
        break;
    }
}