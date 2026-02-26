#pragma once

#include <cstdint>

class il2cpp_hooker
{
public:
    il2cpp_hooker( uintptr_t& ptr )
        : m_ptr( ptr )
    {
    }

    template < auto hk, typename orig_t >
    void hook( orig_t* orig )
    {
        *orig = reinterpret_cast< orig_t >( m_ptr );
        m_ptr = reinterpret_cast< uintptr_t >( hk );
    }

private:
    uintptr_t& m_ptr;
};
