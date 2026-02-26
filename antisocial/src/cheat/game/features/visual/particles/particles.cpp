#include "particles.hpp"

#include "globals.hpp"

#include "game/sdk/misc/math/math.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/component/component.hpp"
#include "game/sdk/unity/transform/transform.hpp"

#include "game/features/visual/thirdperson/thirdperson.hpp"

#include <random>

void c_particles::ambient( )
{
    if ( !c::get< bool >( g_ctx->cfg.world_particles_enabled ) )
        return;

    auto const generate_position = [ & ]( const vec3_t& original ) -> vec3_t {
        std::random_device rd;
        std::mt19937 gen( rd( ) );

        std::uniform_real_distribution< float > xz( -20.f, 20.f );
        std::uniform_real_distribution< float > y( -2.f, 2.f );

        float offset_x = xz( gen );
        float offset_y = y( gen );
        float offset_z = xz( gen );

        return vec3_t( original.x + offset_x, original.y + offset_y, original.z + offset_z );
    };

    auto const generate_alpha = [ & ]( ) -> float {
        std::random_device rd;
        std::mt19937 gen( rd( ) );

        std::uniform_real_distribution< float > v( 0.7f, 1.0f );

        return v( gen );
    };

    auto const generate_velocity = [ & ]( ) -> float {
        std::random_device rd;
        std::mt19937 gen( rd( ) );

        std::uniform_real_distribution< float > v( -0.12f, 0.12f );

        return v( gen );
    };

    if ( !c_camera::get_main( ) )
        return;

    auto const view = c_camera::get_main( )->get_transform( )->get_position( ); //  g_ctx->features.thirdperson->get_unmodified_view( );

    vec3_t const generated = generate_position( view );
    float const velocity = generate_velocity( );

    if ( m_ambient_particles.size( ) < 1000 && !c_physics::linecast( view, generated, 16384 ) )
        m_ambient_particles.push_back( { generated, { }, { velocity, velocity, velocity }, generate_alpha( ) } );

    for ( auto& particle : m_ambient_particles )
    {
        w2s_t const conv = math::world_to_screen( particle.position );
        particle.position_screen = conv.position;
        particle.checker = conv.check;
        particle.visible = !c_physics::linecast( view, particle.position, 16384 );
    }

    for ( int it { 0 }; it < m_ambient_particles.size( ); it++ )
    {
        auto& particle = m_ambient_particles[ it ];

        if ( particle.alpha < 0.001f || vec3_t::distance( particle.position, view ) > 50 )
        {
            m_ambient_particles.erase( m_ambient_particles.begin( ) + it );
            continue;
        }

        float const delta_time = ImGui::GetIO( ).DeltaTime;

        particle.position.x += particle.velocity.x * delta_time;
        particle.position.y += particle.velocity.y * delta_time;
        particle.position.z += particle.velocity.z * delta_time;

        particle.alpha -= 0.55f * delta_time;

        ImColor const color = c::get< ImVec4 >( g_ctx->cfg.world_particles_color );

        if ( particle.checker && particle.visible )
        {
            ImGui::GetBackgroundDrawList( )->AddCircleFilled( particle.position_screen, 2, ImColor( color.Value.x, color.Value.y, color.Value.z, color.Value.w * particle.alpha ), 40 );
        }
    }
}

void c_particles::hit_effect( const vec3_t& where )
{
    constexpr int k_particles_count = 50;

    std::random_device rd;
    std::mt19937 gen( rd( ) );

    std::uniform_real_distribution< float > xz( -0.5f, 0.5f );
    std::uniform_real_distribution< float > up( 1.2f, 2.5f );
    std::uniform_real_distribution< float > alpha( 0.8f, 1.0f );

    for ( int i = 0; i < k_particles_count; i++ )
    {
        vec3_t pos = where;
        vec3_t vel {
            xz( gen ),
            up( gen ),
            xz( gen ) };

        w2s_t const conv = math::world_to_screen( pos );

        m_ambient_particles.push_back( { pos,
                                         conv.position,
                                         vel,
                                         alpha( gen ),
                                         conv.check,
                                         true } );
    }
}