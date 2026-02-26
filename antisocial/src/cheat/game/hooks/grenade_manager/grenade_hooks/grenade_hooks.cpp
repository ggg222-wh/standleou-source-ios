#include "grenade_hooks.hpp"

#include "memory_manager/memory_manager.hpp"

#include "globals.hpp"

#include "game/features/visual/grenade_prediction/grenade_prediction.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/component/component.hpp"

void hk::grenade_manager::grenade_hooks::hook( )
{
    memory_manager::hook( ( 0x181EFD4 ), reinterpret_cast< void* >( hk_update ), reinterpret_cast< void** >( &og_update ) );
    memory_manager::hook( ( 0x181E780 ), reinterpret_cast< void* >( hk_detonated ), reinterpret_cast< void** >( &og_detonated ) );
    memory_manager::hook( ( 0x181E2D8 ), reinterpret_cast< void* >( hk_throw_dropped ), reinterpret_cast< void** >( &og_throw_dropped ) );
    memory_manager::hook( ( 0x18218C8 ), reinterpret_cast< void* >( hk_throw ), reinterpret_cast< void** >( &og_throw ) );
}

void hk::grenade_manager::grenade_hooks::hk_detonated( uintptr_t a, vec3_t b )
{
    og_detonated( a, b );

    if ( g_ctx->features.grenade_prediction->m_target_grenade == a && c::get< bool >( g_ctx->cfg.visual_grenade_prediction ) )
    {
        g_ctx->features.grenade_prediction->m_time_to_explode = false;
        g_ctx->features.grenade_prediction->m_predict_stage = 0;
        g_ctx->features.grenade_prediction->m_target_grenade = 0;
        g_ctx->features.grenade_prediction->m_predict_data_s.clear( );
        g_ctx->features.grenade_prediction->m_predict_data.clear( );
        g_ctx->features.grenade_prediction->m_grenadeid = 0;
    }
}

void hk::grenade_manager::grenade_hooks::hk_update( uintptr_t a )
{
    if ( c::get< bool >( g_ctx->cfg.visual_grenade_prediction ) && g_ctx->features.grenade_prediction->m_predict_data.size( ) > 0 && a == g_ctx->features.grenade_prediction->m_target_grenade && a && g_ctx->features.grenade_prediction->m_target_grenade )
    {
        reinterpret_cast< c_component* >( a )->get_transform( )->set_position( g_ctx->features.grenade_prediction->m_predict_data_s[ g_ctx->features.grenade_prediction->m_predict_stage ].pos );
        if ( !g_ctx->features.grenade_prediction->m_predict_data_s[ g_ctx->features.grenade_prediction->m_predict_stage ].is_final_hit && g_ctx->features.grenade_prediction->m_predict_stage < g_ctx->features.grenade_prediction->m_predict_data_s.size( ) )
            g_ctx->features.grenade_prediction->m_predict_stage++;
        if ( g_ctx->features.grenade_prediction->m_predict_data_s[ g_ctx->features.grenade_prediction->m_predict_stage ].is_final_hit || g_ctx->features.grenade_prediction->m_predict_stage >= g_ctx->features.grenade_prediction->m_predict_data_s.size( ) )
            g_ctx->features.grenade_prediction->m_time_to_explode = true;
        // else target_grenade = nullptr;
    }

    og_update( a );
}

void hk::grenade_manager::grenade_hooks::hk_throw( uintptr_t a, unsigned char b, vec3_t c, vec3_t d, float e, int f )
{
    if ( c::get< bool >( g_ctx->cfg.visual_grenade_prediction ) )
    {
        g_ctx->features.grenade_prediction->m_timetothrow = true;
        g_ctx->features.grenade_prediction->m_grenadeid = b;
    }

    og_throw( a, b, c, d, e, f );
}

void hk::grenade_manager::grenade_hooks::hk_throw_dropped( uintptr_t a, vec3_t b, vec3_t c, int d, float e, double f )
{
    if ( c::get< bool >( g_ctx->cfg.visual_grenade_prediction ) && g_ctx->features.grenade_prediction->m_timetothrow )
    {
        g_ctx->features.grenade_prediction->m_timetothrow = false;
        g_ctx->features.grenade_prediction->m_target_grenade = a;
        g_ctx->features.grenade_prediction->m_predict_stage = 0;
        g_ctx->features.grenade_prediction->m_predict_data_s = g_ctx->features.grenade_prediction->m_predict_data;
    }

    og_throw_dropped( a, b, c, d, e, f );
}