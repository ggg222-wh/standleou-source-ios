#include "inventory.hpp"
#include "globals.hpp"
#include "interface/custom_ui/advanced/elements.hpp"
#include "game/sdk/axlebolt/photon_network/photon_network.hpp"
#include "game/sdk/il2cpp/il2cpp.hpp"
#include "alert/alert.hpp"
#include "game/sdk/axlebolt/inventory_manager/inventory_manager.hpp"

void c_inventory::change( int idx, int new_id )
{
    if ( m_skins->empty( ) )
        return;

    auto vec = m_skins->to_cpp_array( );
    if ( idx < 0 || idx >= vec.size( ) )
        return;

    auto get_definition = []( uintptr_t skin ) -> uintptr_t {
        return skin ? *reinterpret_cast< uintptr_t* >( skin + 0x18 ) : 0;
    };

    if ( auto def = get_definition( vec[ idx ] ) )
        *reinterpret_cast< int* >( def + 0x10 ) = new_id;
}

void c_inventory::change_by_id( int orig_id, int new_id )
{
    if ( m_skins->empty( ) )
        return;

    auto vec = m_skins->to_cpp_array( );
    if ( vec.empty( ) )
        return;

    auto get_definition = []( uintptr_t skin ) -> uintptr_t {
        return skin ? *reinterpret_cast< uintptr_t* >( skin + 0x18 ) : 0;
    };

    for ( auto& skin : vec )
    {
        if ( auto def = get_definition( skin ) )
        {
            int& id = *reinterpret_cast< int* >( def + 0x10 );
            if ( id == orig_id )
                id = new_id;
        }
    }
}

void c_inventory::on_gui( int child )
{
    auto const reset_inv_state = [ this ]( ) -> void {
        m_skins = 0;
        m_definitions = 0;
    };

    if ( c_photon_network::get_room( ) )
    {
        reset_inv_state( );
        return;
    }

    c_inventory_manager* manager = c_inventory_manager::get_instance( );
    if ( !manager )
    {
        reset_inv_state( );
        return;
    }

    m_skins = manager->get_skins( );
    m_definitions = manager->get_definitions( );

    if ( m_skins && !m_skins->empty( ) && m_definitions && !m_definitions->empty( ) )
    {
        // save orig skins
        if ( m_og_skins.empty( ) )
        {
            auto vec = m_skins->to_cpp_array( );

            auto get_definition = []( uintptr_t skin ) -> uintptr_t {
                return skin ? *reinterpret_cast< uintptr_t* >( skin + 0x18 ) : 0;
            };
            auto get_skin_id = []( uintptr_t def ) -> int {
                return def ? *reinterpret_cast< int* >( def + 0x10 ) : 0;
            };

            for ( auto& skin : vec )
            {
                if ( auto def = get_definition( skin ) )
                    m_og_skins.push_back( get_skin_id( def ) );
            }
        }

        switch ( child )
        {
        case 1:
        {
            elem::simple_category( ay_obfuscate( "Select item" ) );
            {
                auto vec = m_skins->to_cpp_array( );

                auto get_definition = []( uintptr_t skin ) -> uintptr_t {
                    return skin ? *reinterpret_cast< uintptr_t* >( skin + 0x18 ) : 0;
                };
                auto get_skin_id = []( uintptr_t def ) -> int {
                    return def ? *reinterpret_cast< int* >( def + 0x10 ) : 0;
                };
                auto get_skin_name = []( uintptr_t def ) -> std::string {
                    if ( !def )
                        return { };
                    if ( auto str = *reinterpret_cast< unity::string** >( def + 0x18 ) )
                        return str->to_cpp_string( );
                    return { };
                };

                struct _state
                {
                    c_animator text { ImVec4( 0.f, 0.f, 0.f, 0.f ) };
                };

                static std::unordered_map< ImGuiID, _state > __state;

                for ( int i = 0; i < vec.size( ); i++ )
                {
                    auto def = get_definition( vec[ i ] );
                    if ( !def )
                        continue;

                    int id = get_skin_id( def );
                    std::string name = get_skin_name( def );

                    std::string fmt = std::to_string( i ) + crypt_string( ". " ) + name;

                    _state& state = __state[ ImGui::GetID( fmt.c_str( ) ) ];

                    state.text.update( ( m_selected_idx == i )
                                           ? ImColor( 234, 193, 251 ).Value
                                           : ImColor( 255, 255, 255 ).Value );

                    ImGui::TextColored( state.text.get< ImVec4 >( ), ( fmt + " [" + std::to_string( id ) + "]" ).c_str( ) );
                    if ( ImGui::IsItemClicked( ) )
                        m_selected_idx = i;
                }
            }
            elem::end_category( );
        }
        break;

        case 2:
        {
            elem::simple_category( ay_obfuscate( "Select replacement" ) );
            {
                static char buf[ 50 ];
                elem::input_text( ay_obfuscate( "Filter" ), buf, IM_ARRAYSIZE( buf ) );
                if ( std::string( buf ) != m_filter_string )
                    m_filter_string = buf;

                auto vec = m_definitions->to_cpp_array( );

                auto get_skin_id = []( uintptr_t def ) -> int {
                    return def ? *reinterpret_cast< int* >( def + 0x10 ) : 0;
                };
                auto get_skin_name = []( uintptr_t def ) -> std::string {
                    if ( !def )
                        return { };
                    if ( auto str = *reinterpret_cast< unity::string** >( def + 0x18 ) )
                        return str->to_cpp_string( );
                    return { };
                };

                for ( int i = 0; i < vec.size( ); i++ )
                {
                    auto def = vec[ i ];
                    if ( !def )
                        continue;

                    int id = get_skin_id( def );
                    std::string name = get_skin_name( def );

                    if ( !m_filter_string.empty( ) && !case_insensitive_search( name, m_filter_string ) )
                        continue;

                    struct _state
                    {
                        c_animator text { ImVec4( 0.f, 0.f, 0.f, 0.f ) };
                    };
                    std::string fmt = std::to_string( i ) + crypt_string( ". " ) + name;

                    static std::unordered_map< ImGuiID, _state > __state;
                    _state& state = __state[ ImGui::GetID( fmt.c_str( ) ) ];

                    state.text.update( ImColor( 255, 255, 255 ).Value );

                    ImGui::TextColored( state.text.get< ImVec4 >( ), fmt.c_str( ) );
                    if ( ImGui::IsItemClicked( ) && m_selected_idx >= 0 )
                    {
                        state.text.set< ImVec4 >( ImColor( 234, 193, 251 ).Value );
                        change( m_selected_idx, id );
                        m_selected_idx = -1;
                    }
                }
            }
            elem::end_category( );
        }
        break;
        }
    } else
    {
        m_selected_idx = -1;
    }
}

bool c_inventory::case_insensitive_search( const std::string& str, const std::string& substr )
{
    auto it = std::search(
        str.begin( ), str.end( ),
        substr.begin( ), substr.end( ),
        []( char ch1, char ch2 ) {
            return std::tolower( static_cast< unsigned char >( ch1 ) ) == std::tolower( static_cast< unsigned char >( ch2 ) );
        } );

    return it != str.end( );
}

ImVec4 c_inventory::get_rarity( int rarity )
{
    constexpr ImColor colors[] = {
        ImColor( 60, 60, 60 ),
        ImColor( 120, 120, 120 ),
        ImColor( 108, 142, 196 ),
        ImColor( 28, 73, 255 ),
        ImColor( 153, 0, 255 ),
        ImColor( 255, 28, 202 ),
        ImColor( 255, 0, 0 ),
        ImColor( 255, 255, 0 ) };
    return ( rarity >= 0 && rarity < ( int ) ( std::size( colors ) ) )
               ? colors[ rarity ].Value
               : ImColor( 255, 255, 255 ).Value;
}

void c_inventory::save_inventory( )
{
    if ( !m_skins || m_skins->empty( ) || m_og_skins.empty( ) )
        return;

    std::ofstream file( g_ctx->documents_directory + crypt_string( "/inventory.cfg" ), std::ios::out );
    if ( !file.is_open( ) )
        return;

    auto vec = m_skins->to_cpp_array( );

    auto get_definition = []( uintptr_t skin ) -> uintptr_t {
        return skin ? *reinterpret_cast< uintptr_t* >( skin + 0x18 ) : 0;
    };
    auto get_skin_id = []( uintptr_t def ) -> int {
        return def ? *reinterpret_cast< int* >( def + 0x10 ) : 0;
    };

    for ( int i = 0; i < vec.size( ); i++ )
    {
        auto def = get_definition( vec[ i ] );
        if ( !def )
            continue;

        int og_id = m_og_skins[ i ];
        int new_id = get_skin_id( def );
        if ( og_id != new_id )
            file << og_id << " " << new_id << "\n";
    }

    file.close( );
}

void c_inventory::load_inventory( )
{
    std::ifstream file( g_ctx->documents_directory + crypt_string( "/inventory.cfg" ), std::ios::in );
    if ( !file.is_open( ) )
        return;

    std::string line;
    while ( std::getline( file, line ) )
    {
        std::stringstream ss( line );
        int orig_id = 0, new_id = 0;
        ss >> orig_id >> new_id;

        if ( orig_id && new_id )
            change_by_id( orig_id, new_id );
    }

    file.close( );
}
