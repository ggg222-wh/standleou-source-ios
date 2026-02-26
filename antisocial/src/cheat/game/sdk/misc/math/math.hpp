#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include <array>
#include <vector>

#include "game/sdk/structures/vec3.hpp"

#include "interface/custom_ui/advanced/utilities.hpp"

struct w2s_t
{
    ImVec2 position { };
    bool check { false };
};

namespace math
{
    extern w2s_t world_to_screen( vec3_t position );
    extern ImRect calculate_player_ent_bbox( vec3_t position, float height );
    extern bool check_surface_by_type( const int type );
    extern void draw_3d_dotted_circle( vec3_t pos, float radius, ImColor color, float segments );
    extern void add_3d_radial_circle( ImDrawList* draw_list, const vec3_t& pos, float radius, float segments, ImU32 col_in, ImU32 col_out );
    extern vec3_t calculate_world_position( vec3_t input );
    extern void draw_3d_circle( vec3_t pos, float radius, float stroke, ImColor color, float segments, bool filled, float fillopacity );
    extern void circle_progress( ImDrawList* draw_list, float progress, float max, float radius, const ImVec2& pos, const ImColor& col, float thickness );

    inline static ImVec2 cos_tan_horizontal( float angle, float range, float x, float y, int length )
    {
        float our_angle = ( angle + 45.f );

        float yaw = our_angle * ( M_PI / 180.0 );

        float view_cosinus = cos( yaw );
        float view_sinus = sin( yaw );

        float x2 = range * ( -view_cosinus ) + range * view_sinus;
        float y2 = range * ( -view_cosinus ) - range * view_sinus;

        int screen_x = x + static_cast< int >( x2 / range * length );
        int screen_y = y + static_cast< int >( y2 / range * length );

        return { screen_x, screen_y };
    }

    inline static void add_shadow_text( ImDrawList* surface, ImFont* font, float size, const ImVec2& pos, const ImColor& color, const ImColor& out_color, const std::string& value )
    {
        ImVec2 const floored_pos = { ( float ) int( pos.x ), ( float ) int( pos.y ) };

        auto const text_size = font->CalcTextSizeA( size, FLT_MAX, -1, value.c_str( ) );

        surface->AddShadowRect( ImVec2( floored_pos.x + 2, floored_pos.y + text_size.y / 2 - 1 ), ImVec2( floored_pos.x + text_size.x - 2, floored_pos.y + text_size.y / 2 + 1 ), c::alpha_interpolate( out_color, 0.7f ), 28.f, ImVec2( 0, 0 ), 0, 100.f );
        surface->AddText( font, size, floored_pos, color, value.c_str( ) );
    }

    inline static void add_outlined_text( ImDrawList* surface, ImFont* font, float size, const ImVec2& pos, const ImColor& color, const ImColor& out_color, const std::string& value )
    {
        static const std::vector< ImVec2 > offsets = {
            { 1, 1 },
            { -1, 1 },
            { 1, -1 },
            { -1, -1 },
            { 0, -1 },
            { -1, 0 },
            { 1, 0 },
            { 0, 1 } };

        ImVec2 const floored_pos = { ( float ) int( pos.x ), ( float ) int( pos.y ) };

        for ( auto const& offset : offsets )
            surface->AddText( font, size, ImVec2( floored_pos.x + offset.x, floored_pos.y + offset.y ), out_color, value.c_str( ) );

        surface->AddText( font, size, floored_pos, color, value.c_str( ) );
    }

    static inline ImColor color_lerp( ImColor a, ImColor b, float f )
    {
        return { a.Value.x + f * ( b.Value.x - a.Value.x ), a.Value.y + f * ( b.Value.y - a.Value.y ), a.Value.z + f * ( b.Value.z - a.Value.z ), a.Value.w + f * ( b.Value.w - a.Value.w ) };
    }

    // https://github.com/ocornut/imgui/issues/5483
    static inline void add_radial_gradient( ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col_in, ImU32 col_out )
    {
        if ( ( ( col_in | col_out ) & IM_COL32_A_MASK ) == 0 || radius < 0.5f )
            return;

        draw_list->_PathArcToFastEx( center, radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0 );
        const int count = draw_list->_Path.Size - 1;

        unsigned int vtx_base = draw_list->_VtxCurrentIdx;
        draw_list->PrimReserve( count * 3, count + 1 );

        const ImVec2 uv = draw_list->_Data->TexUvWhitePixel;
        draw_list->PrimWriteVtx( center, uv, col_in );
        for ( int n = 0; n < count; n++ )
            draw_list->PrimWriteVtx( draw_list->_Path[ n ], uv, col_out );

        for ( int n = 0; n < count; n++ )
        {
            draw_list->PrimWriteIdx( ( ImDrawIdx ) ( vtx_base ) );
            draw_list->PrimWriteIdx( ( ImDrawIdx ) ( vtx_base + 1 + n ) );
            draw_list->PrimWriteIdx( ( ImDrawIdx ) ( vtx_base + 1 + ( ( n + 1 ) % count ) ) );
        }
        draw_list->_Path.Size = 0;
    }

    static inline void add_fov_circle( ImDrawList* draw_list, float radius, const ImColor& col1, const ImColor& col2 )
    {
        ImVec2 const center = ImGui::GetIO( ).DisplaySize / 2;

        static float rotation_angle = 0.0f;
        rotation_angle += 0.01f;

        auto rotate_point = [ & ]( const ImVec2& point ) -> ImVec2 {
            float s = sinf( rotation_angle );
            float c = cosf( rotation_angle );
            ImVec2 p = point - center;
            ImVec2 rotated;
            rotated.x = p.x * c - p.y * s;
            rotated.y = p.x * s + p.y * c;
            return rotated + center;
        };

        float const base_alpha = fminf( col1.Value.w, col2.Value.w );
        ImColor const base_col( col1.Value.x, col1.Value.y, col1.Value.z, base_alpha );

        ImDrawListSplitter splitter;
        splitter.Split( draw_list, 2 );
        splitter.SetCurrentChannel( draw_list, 1 );

        auto old_flags = draw_list->Flags;
        draw_list->Flags |= ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;

        int const vtx_start = draw_list->VtxBuffer.Size;
        draw_list->AddCircle( center, radius, base_col, static_cast< int >( radius ) );
        int const vtx_end = draw_list->VtxBuffer.Size;

        ImVec2 const grad_start = rotate_point( center - ImVec2( radius / 2, radius / 2 ) );
        ImVec2 const grad_end = rotate_point( center + ImVec2( radius / 2, radius / 2 ) );

        ImGui::ShadeVertsLinearColorGradientKeepAlpha( draw_list, vtx_start, vtx_end, grad_start, grad_end, col1, col2 );

        draw_list->Flags = old_flags;
        splitter.Merge( draw_list );
    }

    static inline bool check_fov( ImVec2 player, int fov )
    {
        auto const screen = ImGui::GetIO( ).DisplaySize;
        return ( std::powf( screen.x / 2 - player.x, 2 ) + std::powf( screen.y / 2 - player.y, 2 ) ) <= std::powf( fov, 2 );
    }

    static inline void vtx_grayscale( ImDrawList* dl, int start, int end, float factor = 1.0f )
    {
        for ( int i = start; i < end; i++ )
        {
            ImDrawVert& v = dl->VtxBuffer[ i ];

            ImU32 const c = v.col;
            float a = ( ( c >> 24 ) & 0xFF ) / 255.0f;
            float r = ( ( c >> 0 ) & 0xFF ) / 255.0f;
            float g = ( ( c >> 8 ) & 0xFF ) / 255.0f;
            float b = ( ( c >> 16 ) & 0xFF ) / 255.0f;

            float const gray = r * 0.299f + g * 0.587f + b * 0.114f;

            r = r * ( 1.0f - factor ) + gray * factor;
            g = g * ( 1.0f - factor ) + gray * factor;
            b = b * ( 1.0f - factor ) + gray * factor;

            v.col = IM_COL32( ( int ) ( r * 255 ), ( int ) ( g * 255 ), ( int ) ( b * 255 ), ( int ) ( a * 255 ) );
        }
    }

} // namespace math