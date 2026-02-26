
#pragma once

#include <cstdint>

namespace hk::camera_scope_zoomer::zoom
{
    inline void ( *og_zoom )( uintptr_t, float, float );
    extern void hk_zoom( uintptr_t, float, float );

    extern void hook( );
} // namespace hk::camera_scope_zoomer::zoom
