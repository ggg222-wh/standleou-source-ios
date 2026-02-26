#include "il2cpp.hpp"

#include "globals.hpp"

il2cpp_image* c_il2cpp_api::get_img_to_asm( const char* assembly )
{
    il2cpp_domain* const domain = reinterpret_cast< il2cpp_domain* ( * ) ( ) >( functions.domain_get )( );
    il2cpp_assembly* const domain_asm = reinterpret_cast< il2cpp_assembly* ( * ) ( il2cpp_domain*, const char* ) >( functions.domain_assembly_open )( domain, assembly );
    il2cpp_image* const asm_img = reinterpret_cast< il2cpp_image* ( * ) ( il2cpp_assembly* ) >( functions.assembly_get_image )( domain_asm );
    return asm_img;
}

uintptr_t& c_il2cpp_api::get_method_pointer( const char* namespaze, const char* clazz, const char* method, int args )
{
    il2cpp_class* const m_class = get_class( namespaze, clazz );
    if ( !m_class )
        throw std::runtime_error( "Class not found" );

    method_info* const m_method = reinterpret_cast< method_info* ( * ) ( il2cpp_class*, const char*, int ) >( functions.class_get_method_from_name )( m_class, method, args );
    if ( !m_method )
        throw std::runtime_error( "Method not found" );

    return m_method->method_pointer;
}

il2cpp_class* c_il2cpp_api::get_class( const char* namespaze, const char* clazz )
{
    auto const domain = reinterpret_cast< il2cpp_domain* ( * ) ( ) >( functions.domain_get )( );
    if ( !domain )
        return nullptr;

    int asm_count { };
    auto const all = reinterpret_cast< il2cpp_assembly** ( * ) ( il2cpp_domain*, int* ) >( functions.domain_get_assemblies )( domain, &asm_count );

    for ( int idx { 0 }; idx < asm_count; idx++ )
    {
        auto const img = reinterpret_cast< il2cpp_image* ( * ) ( il2cpp_assembly* ) >( functions.assembly_get_image )( all[ idx ] );
        if ( !img )
            continue;

        auto const klass = reinterpret_cast< il2cpp_class* ( * ) ( il2cpp_image*, const char*, const char* ) >( functions.class_from_name )( img, namespaze, clazz );
        if ( klass )
            return klass;
    }

    return nullptr;
}

void c_il2cpp_api::initialize( )
{
    static const std::unordered_map< const char*, void** > export_map =
        {

            { ay_obfuscate( "il2cpp_array_new" ), &functions.array_new },
            { ay_obfuscate( "il2cpp_class_from_name" ), &functions.class_from_name },
            { ay_obfuscate( "il2cpp_get_corlib" ), &functions.get_corlib },
            { ay_obfuscate( "il2cpp_value_box" ), &functions.value_box },
            { ay_obfuscate( "il2cpp_object_unbox" ), &functions.object_unbox },
            { ay_obfuscate( "il2cpp_object_new" ), &functions.object_new },
            { ay_obfuscate( "il2cpp_class_get_method_from_name" ), &functions.class_get_method_from_name },
            { ay_obfuscate( "il2cpp_field_static_get_value" ), &functions.field_static_get_value },
            { ay_obfuscate( "il2cpp_field_static_set_value" ), &functions.field_static_set_value },
            { ay_obfuscate( "il2cpp_class_get_field_from_name" ), &functions.class_get_field_from_name },
            { ay_obfuscate( "il2cpp_domain_get" ), &functions.domain_get },
            { ay_obfuscate( "il2cpp_domain_assembly_open" ), &functions.domain_assembly_open },
            { ay_obfuscate( "il2cpp_assembly_get_image" ), &functions.assembly_get_image },
            { ay_obfuscate( "il2cpp_domain_get_assemblies" ), &functions.domain_get_assemblies },
            { ay_obfuscate( "il2cpp_resolve_icall" ), &functions.resolve_icall }

        };

    for ( auto& it : export_map )
    {
        *it.second = dlsym( RTLD_DEFAULT, it.first );
    }
}