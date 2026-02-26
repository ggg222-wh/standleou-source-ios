#include "photon_player.hpp"

#include "../../unity/unity.hpp"
#include "../../il2cpp/il2cpp.hpp"
#include "../../unity/unity.hpp"

#include "game/sdk/axlebolt/photon_network/photon_network.hpp"

#include "globals.hpp"

#include "memory_manager/memory_manager.hpp"

std::string c_photon_player::get_uuid( )
{
    if ( !this )
        return get_nickname( );

    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return get_nickname( );

    for ( int i = 0; i < properties->count; i++ )
    {
        auto& entry = properties->entries->m_items[ i ];
        if ( !entry.key )
            continue;

        if ( entry.key->to_cpp_string( ) == crypt_string( "uid" ) )
        {
            return reinterpret_cast< unity::string* >( entry.value )->to_cpp_string( ) + get_nickname( );
        }
    }

    return get_nickname( );
}

std::string c_photon_player::get_nickname( )
{
    return ( *reinterpret_cast< unity::string** >( reinterpret_cast< uintptr_t >( this ) + 0x18 ) )->to_cpp_string( );
}

std::string c_photon_player::get_id( )
{
    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return get_nickname( );

    for ( int i = 0; i < properties->count; i++ )
    {
        auto& entry = properties->entries->m_items[ i ];
        if ( !entry.key )
            continue;

        if ( entry.key->to_cpp_string( ) == crypt_string( "uid" ) )
        {
            return reinterpret_cast< unity::string* >( entry.value )->to_cpp_string( );
        }
    }

    return "null";
}

void c_photon_player::set_clan_tag( const std::string& tag )
{
    if ( !this )
        return;

    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return;

    for ( int i = 0; i < properties->count; i++ )
    {
        auto& entry = properties->entries->m_items[ i ];
        if ( !entry.key )
            continue;

        // debug_log( entry.key->to_cpp_string( ) );

        if ( entry.key->to_cpp_string( ) == crypt_string( "clantag" ) )
        {
            entry.value = reinterpret_cast< il2cpp_object* >( unity::string::create( tag ) );
            break;
        }
    }
}

void c_photon_player::set_user_id( const std::string& uid )
{
    if ( !this )
        return;

    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return;

    for ( int i = 0; i < properties->count; i++ )
    {
        auto& entry = properties->entries->m_items[ i ];
        if ( !entry.key )
            continue;

        if ( entry.key->to_cpp_string( ) == crypt_string( "uid" ) )
        {
            entry.value = reinterpret_cast< il2cpp_object* >( unity::string::create( uid ) );
            break;
        }
    }
}

void c_photon_player::add_score( int score )
{
    if ( !this )
        return;

    auto const cur_score = get_property< int >( crypt_string( "score" ) );

    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return;

    for ( int i = 0; i < properties->count; i++ )
    {
        auto& entry = properties->entries->m_items[ i ];
        if ( !entry.key )
            continue;

        if ( entry.key->to_cpp_string( ) == crypt_string( "score" ) )
        {
            auto* const int_class = g_ctx->il2cpp->get_class( ay_obfuscate( "System" ), ay_obfuscate( "Int32" ) );
            entry.value = reinterpret_cast< il2cpp_object* ( * ) ( il2cpp_class*, int ) >( g_ctx->il2cpp->functions.value_box )( int_class, cur_score + score );
            break;
        }
    }
}

template < typename T >
T c_photon_player::get_property( const std::string& key )
{
    if ( !this )
        return { };

    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return { };

    std::vector< unity::string* > keys = properties->get_keys( );

    if ( keys.empty( ) )
        return { };

    for ( int i { 0 }; i < keys.size( ); i++ )
    {
        if ( keys[ i ]->to_cpp_string( ) != key || !properties->get_values( )[ i ] )
            continue;

        return *( T* ) reinterpret_cast< uintptr_t* ( * ) ( il2cpp_object* ) >( g_ctx->il2cpp->functions.object_unbox )( properties->get_values( )[ i ] );
    }

    return { };
}

template < typename T >
T c_photon_player::get_property_boxed( const std::string& key )
{
    if ( !this )
        return { };

    auto* const properties = *reinterpret_cast< unity::dictionary< unity::string*, il2cpp_object* >** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );

    if ( !properties || properties->get_size( ) < 1 )
        return { };

    std::vector< unity::string* > keys = properties->get_keys( );

    if ( keys.empty( ) )
        return { };

    for ( int i { 0 }; i < keys.size( ); i++ )
    {
        if ( keys[ i ]->to_cpp_string( ) != key || !properties->get_values( )[ i ] )
            continue;

        return reinterpret_cast< T& >( properties->get_values( )[ i ] );
    }

    return { };
}

template int c_photon_player::get_property< int >( const std::string& );
template float c_photon_player::get_property< float >( const std::string& );
template double c_photon_player::get_property< double >( const std::string& );
template bool c_photon_player::get_property< bool >( const std::string& );
template char c_photon_player::get_property< char >( const std::string& );
template char* c_photon_player::get_property< char* >( const std::string& );

template int c_photon_player::get_property_boxed< int >( const std::string& );
template float c_photon_player::get_property_boxed< float >( const std::string& );
template double c_photon_player::get_property_boxed< double >( const std::string& );
template bool c_photon_player::get_property_boxed< bool >( const std::string& );
template unity::array< unsigned char >* c_photon_player::get_property_boxed< unity::array< unsigned char >* >( const std::string& );
template char c_photon_player::get_property_boxed< char >( const std::string& );
template char* c_photon_player::get_property_boxed< char* >( const std::string& );
