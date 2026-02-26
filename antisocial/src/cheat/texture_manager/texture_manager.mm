#define STB_IMAGE_IMPLEMENTATION

#import <string>
#import <vector>

#import "stb_image.h"

#import "device_decl.h"
#import "texture_manager.hpp"

#import "imgui.h"
#import "imgui_internal.h"

id< MTLDevice > self_device;

NSMutableDictionary< NSString*, id< MTLTexture > >* textures_map;

bool c_texture_manager::load_grayscale( unsigned char* data, int length, texture_data_t* texture, const std::string& uuid )
{
    if ( !textures_map )
        textures_map = [NSMutableDictionary new];

    if ( !texture || !data || length <= 0 )
        return false;

    unload( texture );

    int channels;
    unsigned char* pixels = stbi_load_from_memory( data, length, &texture->width, &texture->height, &channels, 4 );
    if ( !pixels )
        return false;

    std::vector< unsigned char > grayPixels( texture->width * texture->height * 4 );

    for ( int i = 0; i < texture->width * texture->height; i++ )
    {
        unsigned char r = pixels[ i * 4 + 0 ];
        unsigned char g = pixels[ i * 4 + 1 ];
        unsigned char b = pixels[ i * 4 + 2 ];
        unsigned char a = pixels[ i * 4 + 3 ];

        unsigned char gray = static_cast< unsigned char >( 0.299f * r + 0.587f * g + 0.114f * b );

        grayPixels[ i * 4 + 0 ] = gray;
        grayPixels[ i * 4 + 1 ] = gray;
        grayPixels[ i * 4 + 2 ] = gray;
        grayPixels[ i * 4 + 3 ] = a;
    }

    MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                    width:texture->width
                                                                                   height:texture->height
                                                                                mipmapped:NO];
    desc.usage = MTLTextureUsageShaderRead;
    desc.storageMode = MTLStorageModeShared;

    id< MTLTexture > tex = [self_device newTextureWithDescriptor:desc];
    [tex replaceRegion:MTLRegionMake2D( 0, 0, texture->width, texture->height )
           mipmapLevel:0
             withBytes:grayPixels.data( )
           bytesPerRow:texture->width * 4];

    stbi_image_free( pixels );

    NSString* key = [NSString stringWithUTF8String:uuid.c_str( )];
    key = [key stringByAppendingString:@"_gray"];
    textures_map[ key ] = tex;

    texture->texture_id = ( __bridge void* ) tex;
    texture->loaded = true;

    return true;
}

void c_texture_manager::unload( texture_data_t* texture )
{
    if ( !texture || !texture->loaded )
        return;

    id< MTLTexture > tex = ( __bridge id< MTLTexture > ) texture->texture_id;
    if ( tex )
    {
        for ( NSString* key in textures_map )
        {
            if ( textures_map[ key ] == tex )
            {
                [textures_map removeObjectForKey:key];
                break;
            }
        }
    }

    texture->loaded = false;
    texture->texture_id = 0;
}

bool c_texture_manager::load( unsigned char* data, int length, texture_data_t* texture, const std::string& uuid )
{
    if ( !textures_map )
        textures_map = [NSMutableDictionary new];

    if ( !texture || !data || length <= 0 )
        return false;

    unload( texture );

    int channels;
    unsigned char* pixels = stbi_load_from_memory( data, length, &texture->width, &texture->height, &channels, 4 );
    if ( !pixels )
    {
        return false;
    }

    MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                    width:texture->width
                                                                                   height:texture->height
                                                                                mipmapped:NO];
    desc.usage = MTLTextureUsageShaderRead;
    desc.storageMode = MTLStorageModeShared;

    id< MTLTexture > tex = [self_device newTextureWithDescriptor:desc];
    [tex replaceRegion:MTLRegionMake2D( 0, 0, texture->width, texture->height )
           mipmapLevel:0
             withBytes:pixels
           bytesPerRow:texture->width * 4];

    stbi_image_free( pixels );

    NSString* key = [NSString stringWithUTF8String:uuid.c_str( )];
    textures_map[ key ] = tex;

    texture->texture_id = ( __bridge void* ) tex;
    texture->loaded = true;

    return true;
}
