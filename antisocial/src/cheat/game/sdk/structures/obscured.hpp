#pragma once

#include <cstdint>

struct obscured_float_t
{
    static void set( uint64_t location, float value )
    {
        int crypto_key = *reinterpret_cast< int* >( location );

        union intfloat
        {
            int i;
            float f;
        };

        intfloat IF;
        IF.f = value;
        intfloat IF2;
        IF2.i = IF.i ^ crypto_key;

        *reinterpret_cast< float* >( location + 0x4 ) = IF2.f;
    }
};

class obscured_bool_t
{
public:
    struct internal_t
    {
        typedef unsigned char byte;
        byte current_crypto_key;
        int hidden_value;
        bool inited;
        bool fake_value;
        bool fake_value_active;
    };

    internal_t internal { };
    std::uintptr_t address;

    explicit obscured_bool_t( const std::uintptr_t addr )
        : address( addr )
    {
        internal = *reinterpret_cast< internal_t* >( address );
    }

    static int encrypt( bool value, unsigned char key )
    {
        const int num = value ? 213 : 181;
        return num ^ key;
    }

    static bool decrypt( int value, unsigned char key )
    {
        return ( value ^ key ) != 181;
    }

    bool get( ) const
    {
        return ( internal.hidden_value ^ internal.current_crypto_key ) != 181;
    }

    void set( bool value )
    {
        internal.hidden_value = encrypt( value, internal.current_crypto_key );
        internal.fake_value = value;
        internal.fake_value_active = true;
        *reinterpret_cast< internal_t* >( address ) = internal;
    }
};
