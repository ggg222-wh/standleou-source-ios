#include "math.hpp"

#include "../../unity/camera/camera.hpp"
#include "../../unity/physics/physics.hpp"
#include "../../unity/transform/transform.hpp"
#include "../../unity/component/component.hpp"

w2s_t math::world_to_screen( vec3_t position )
{
    c_camera* const main_camera = c_camera::get_main( );
    if ( !main_camera )
        return w2s_t { ImVec2 { 0, 0 }, 0 };

    vec3_t world_point = main_camera->world_to_viewport( position, 2 );

    world_point.x = static_cast< int >( ImGui::GetIO( ).DisplaySize.x * world_point.x );
    world_point.y = static_cast< int >( ImGui::GetIO( ).DisplaySize.y - world_point.y * ImGui::GetIO( ).DisplaySize.y );

    return w2s_t { ImVec2 { world_point.x, world_point.y }, world_point.z > 0 };
}

ImRect math::calculate_player_ent_bbox( vec3_t position, float height )
{
    ImVec2 const top = world_to_screen( vec3_t( position.x, position.y + height, position.z ) ).position;
    ImVec2 const bottom = world_to_screen( vec3_t( position.x, position.y - 0.15f, position.z ) ).position;

    float calc_top = top.x;
    float calc_bottom = bottom.x;
    if ( top.x > bottom.x )
    {
        calc_top = bottom.x;
        calc_bottom = top.x;
    }

    float const width = abs( ( top.y - bottom.y ) / 2.f ); // abs((top.y - bottom.y) / 3.75f)

    ImVec2 const min = { calc_top - width / 2 - 1, top.y - 1 };

    ImRect const bounds = {
        { static_cast< int >( min.x ),
          static_cast< int >( min.y ) },
        { static_cast< int >( ( calc_bottom + width / 2 + 1 ) ),
          static_cast< int >( ( bottom.y + 1 ) ) } };

    return bounds;
}

bool math::check_surface_by_type( const int type )
{
    return type == surface_type::cardboard ||
           type == surface_type::thin_wood ||
           type == surface_type::glass ||
           type == surface_type::plaster ||
           type == surface_type::wood ||
           type == surface_type::water ||
           type == surface_type::character;
}

void math::draw_3d_dotted_circle( vec3_t pos, float radius, ImColor color, float segments )
{
    for ( float i = 0; i < segments; i++ )
    {
        if ( i < segments )
        {
            vec3_t const pos1 = vec3_t( pos.x + radius * cos( i * ( PI * 2 ) / segments ), pos.y, pos.z + radius * sin( i * ( PI * 2 ) / segments ) );
            vec3_t const pos2 = vec3_t( pos.x + radius * cos( ( i + 1 ) * ( PI * 2 ) / segments ), pos.y, pos.z + radius * sin( ( i + 1 ) * ( PI * 2 ) / segments ) );

            vec3_t const midpoint = vec3_t(
                ( pos1.x + pos2.x ) / 2.0f,
                ( pos1.y + pos2.y ) / 2.0f,
                ( pos1.z + pos2.z ) / 2.0f );

            w2s_t const w2s = world_to_screen( midpoint );

            if ( w2s.check )
                ImGui::GetBackgroundDrawList( )->AddCircleFilled( w2s.position, 3, color, 120 );
        }
    }
}

vec3_t math::calculate_world_position( vec3_t input )
{
    c_transform* const camera_transform = reinterpret_cast< c_component* >( c_camera::get_main( ) )->get_transform( );

    vec3_t const cam_pos = camera_transform->get_position( );
    vec3_t const cam_normal = camera_transform->get_forward( );
    vec3_t const dir = input - cam_pos;

    float const cam_dot = vec3_t::dot( cam_normal, dir );
    return cam_dot <= 0 ? cam_pos + ( dir - ( cam_normal * cam_dot * 1.01 ) ) : input;
}

void math::draw_3d_circle( vec3_t pos, float radius, float stroke, ImColor color, float segments, bool filled, float fillopacity )
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList( );
    ImDrawListSplitter splitter;
    splitter.Split( draw_list, 2 );

    splitter.SetCurrentChannel( draw_list, 1 );
    auto old_flags = draw_list->Flags;
    draw_list->Flags |= ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;

    for ( float i = 0; i < segments; i++ )
    {
        vec3_t pos1 = vec3_t(
            pos.x + radius * cos( i * ( PI * 2 ) / segments ),
            pos.y,
            pos.z + radius * sin( i * ( PI * 2 ) / segments ) );
        vec3_t pos2 = vec3_t(
            pos.x + radius * cos( ( i + 1 ) * ( PI * 2 ) / segments ),
            pos.y,
            pos.z + radius * sin( ( i + 1 ) * ( PI * 2 ) / segments ) );

        w2s_t const pos1_screen = world_to_screen( pos1 );
        w2s_t const pos2_screen = world_to_screen( pos2 );

        if ( pos1_screen.check && pos2_screen.check )
        {
            draw_list->AddLine( pos1_screen.position, pos2_screen.position, color, stroke );

            if ( filled )
            {
                draw_list->PathLineTo( pos1_screen.position );
                draw_list->PathLineTo( pos2_screen.position );
            }
        }
    }

    if ( filled )
    {
        color.Value.w = fillopacity;
        draw_list->PathFillConvex( color );
    }

    draw_list->Flags = old_flags;
    splitter.Merge( draw_list );
}

void math::add_3d_radial_circle( ImDrawList* draw_list, const vec3_t& pos, float radius, float segments, ImU32 col_in, ImU32 col_out )
{
    ImDrawListSplitter splitter;
    splitter.Split( draw_list, 2 );

    splitter.SetCurrentChannel( draw_list, 1 );
    auto old_flags = draw_list->Flags;
    draw_list->Flags |= ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;

    w2s_t const center_screen = world_to_screen( pos );
    if ( !center_screen.check )
        return;

    ImVec2 const center = center_screen.position;

    std::vector< ImVec2 > ring;
    ring.reserve( ( int ) segments );

    for ( int i = 0; i < segments; i++ )
    {
        float const a = i * ( PI * 2.0f ) / segments;
        vec3_t const ring_pos = vec3_t(
            pos.x + radius * cosf( a ),
            pos.y,
            pos.z + radius * sinf( a ) );

        w2s_t const screen = world_to_screen( ring_pos );
        if ( screen.check )
            ring.push_back( screen.position );
    }

    int const count = ( int ) ring.size( );
    if ( count < 3 )
        return;

    unsigned int const vtx_base = draw_list->_VtxCurrentIdx;
    draw_list->PrimReserve( count * 3, count + 1 );

    ImVec2 const uv = draw_list->_Data->TexUvWhitePixel;
    draw_list->PrimWriteVtx( center, uv, col_in );

    for ( int n = 0; n < count; n++ )
        draw_list->PrimWriteVtx( ring[ n ], uv, col_out );

    for ( int n = 0; n < count; n++ )
    {
        draw_list->PrimWriteIdx( ( ImDrawIdx ) ( vtx_base ) );
        draw_list->PrimWriteIdx( ( ImDrawIdx ) ( vtx_base + 1 + n ) );
        draw_list->PrimWriteIdx( ( ImDrawIdx ) ( vtx_base + 1 + ( ( n + 1 ) % count ) ) );
    }

    draw_list->Flags = old_flags;

    splitter.Merge( draw_list );
}

void math::circle_progress( ImDrawList* draw_list, float progress, float max, float radius, const ImVec2& pos, const ImColor& col, float thickness )
{
    ImDrawListSplitter splitter;
    splitter.Split( draw_list, 2 );

    splitter.SetCurrentChannel( draw_list, 1 );
    auto old_flags = draw_list->Flags;
    draw_list->Flags |= ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;

    float position = progress / max * IM_PI * 2.f;
    const ImVec2& center = pos;

    draw_list->PathClear( );
    draw_list->PathArcTo( center, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 64 );
    draw_list->PathStroke( col, 0, thickness );

    ImVec2 start_point( center.x + radius * cosf( IM_PI * 1.5f ),
                        center.y + radius * sinf( IM_PI * 1.5f ) );
    ImVec2 end_point( center.x + radius * cosf( IM_PI * 1.5f + position ),
                      center.y + radius * sinf( IM_PI * 1.5f + position ) );

    float cap_radius = thickness / 2.0f;
    draw_list->AddCircleFilled( start_point, cap_radius, col );
    draw_list->AddCircleFilled( end_point, cap_radius, col );

    draw_list->Flags = old_flags;

    splitter.Merge( draw_list );
}
