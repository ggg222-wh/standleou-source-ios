#include "world_modulation.hpp"

#include "game/sdk/axlebolt/player_controller/player_controller.hpp"
#include "game/sdk/unity/transform/transform.hpp"
#include "game/sdk/unity/camera/camera.hpp"
#include "game/sdk/unity/component/component.hpp"
#include "game/sdk/unity/physics/physics.hpp"
#include "game/sdk/unity/type/type.hpp"
#include "game/sdk/unity/renderer/renderer.hpp"
#include "game/sdk/unity/material/material.hpp"
#include "game/sdk/unity/objects/objects.hpp"
#include "game/sdk/unity/unity.hpp"
#include "game/sdk/unity/shader/shader.hpp"
#include "game/sdk/unity/resources/resources.hpp"

#include "game/entities/entities.hpp"

#include "globals.hpp"

void c_modulation::modulate( ) const
{
    if ( c::get< bool >( g_ctx->cfg.world_modulate ) )
    {
        c::get< bool >( g_ctx->cfg.world_modulate ) = false;

        uintptr_t type = c_type::get_type( unity::string::create( ay_obfuscate( "UnityEngine.MeshRenderer, UnityEngine.CoreModule" ) ) );
        unity::array< c_renderer* >* type_array = c_objects::get_objects_of_type< c_renderer* >( type );
        if ( type_array )
        {
            for ( c_renderer* renderer : type_array->to_cpp_array( ) )
            {
                if ( !renderer )
                    continue;

                unity::array< c_material* >* materials = renderer->get_materials( );

                if ( materials->capacity <= 0 )
                    continue;

                for ( int i = 0; i < materials->capacity; i++ )
                {
                    if ( materials->get_ptr( )[ i ] && !materials->get_ptr( )[ i ]->has_property( unity::string::create( ay_obfuscate( "_Color" ) ) ) )
                    {
                        uintptr_t texture = materials->get_ptr( )[ i ]->get_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ) );

                        auto* const material = materials->get_ptr( )[ i ];

                        auto const shader = c_resources::find_shader( unity::string::create( ay_obfuscate( "Legacy Shaders/Diffuse" ) ) );

                        material->set_shader( shader );

                        material->set_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ), texture );

                        // material->c_material* custom_material = c_material::create( ay_obfuscate( "Legacy Shaders/Diffuse" ) );
                        // if ( custom_material )
                        // {
                        //     uintptr_t texture = materials->get_ptr( )[ i ]->get_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ) );

                        //     custom_material->set_texture( unity::string::create( ay_obfuscate( "_MainTex" ) ), texture );

                        //     materials->get_ptr( )[ i ] = custom_material;
                        // } else
                        //     continue;
                    }

                    materials->get_ptr( )[ i ]->set_color( unity::string::create( ay_obfuscate( "_Color" ) ), c::get< ImVec4 >( g_ctx->cfg.world_modulation ) );
                }

                renderer->set_materials( materials );
            }
        }
    }
}