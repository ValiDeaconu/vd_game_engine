//
// Created by Vali on 11/25/2020.
//

#ifndef VDGE_GLTYPES_HPP
#define VDGE_GLTYPES_HPP

#include "Context.hpp"

namespace vd::gl {

    enum TextureFormat {
        eR = GL_RED,
        eRedInteger = GL_RED_INTEGER,
        eRG = GL_RG,
        eRGInteger = GL_RG_INTEGER,
        eRGBInteger = GL_RGB_INTEGER,
        eRGBAInteger = GL_RGBA_INTEGER,
        eDepthStencil = GL_DEPTH_STENCIL,

        eAlpha = GL_ALPHA,
        eAlpha4 = GL_ALPHA4,
        eAlpha8 = GL_ALPHA8,
        eAlpha12 = GL_ALPHA12,
        eAlpha16 = GL_ALPHA16,
        eCompressedAlpha = GL_COMPRESSED_ALPHA,
        eCompressedLuminance = GL_COMPRESSED_LUMINANCE,
        eCompressedLuminanceAlpha = GL_COMPRESSED_LUMINANCE_ALPHA,
        eCompressedIntensity = GL_COMPRESSED_INTENSITY,
        eCompressedRGB = GL_COMPRESSED_RGB,
        eCompressedRGBA = GL_COMPRESSED_RGBA,
        eDepthComponent = GL_DEPTH_COMPONENT,
        eDepthComponent16 = GL_DEPTH_COMPONENT16,
        eDepthComponent24 = GL_DEPTH_COMPONENT24,
        eDepthComponent32 = GL_DEPTH_COMPONENT32,
        eDepthComponent32F = GL_DEPTH_COMPONENT32F,
        eDepth24Stencil8 = GL_DEPTH24_STENCIL8,
        eLuminance = GL_LUMINANCE,
        eLuminance4 = GL_LUMINANCE4,
        eLuminance8 = GL_LUMINANCE8,
        eLuminance12 = GL_LUMINANCE12,
        eLuminance16 = GL_LUMINANCE16,
        eLuminanceAlpha = GL_LUMINANCE_ALPHA,
        eLuminance4Alpha4 = GL_LUMINANCE4_ALPHA4,
        eLuminance6Alpha2 = GL_LUMINANCE6_ALPHA2,
        eLuminance8Alpha8 = GL_LUMINANCE8_ALPHA8,
        eLuminance12Alpha4 = GL_LUMINANCE12_ALPHA4,
        eLuminance12Alpha12 = GL_LUMINANCE12_ALPHA12,
        eLuminance16Alpha16 = GL_LUMINANCE16_ALPHA16,
        eIntensity = GL_INTENSITY,
        eIntensity4 = GL_INTENSITY4,
        eIntensity8 = GL_INTENSITY8,
        eIntensity12 = GL_INTENSITY12,
        eIntensity16 = GL_INTENSITY16,
        eR3G3B2 = GL_R3_G3_B2,
        eRGB = GL_RGB,
        eRGB4 = GL_RGB4,
        eRGB5 = GL_RGB5,
        eRGB8 = GL_RGB8,
        eRGB10 = GL_RGB10,
        eRGB12 = GL_RGB12,
        eRGB16 = GL_RGB16,
        eRGBA = GL_RGBA,
        eRGBA2 = GL_RGBA2,
        eRGBA4 = GL_RGBA4,
        eRGB5A1 = GL_RGB5_A1,
        eRGBA8 = GL_RGBA8,
        eRGB10A2 = GL_RGB10_A2,
        eRGBA12 = GL_RGBA12,
        eRGBA16 = GL_RGBA16,
        eSLuminance = GL_SLUMINANCE,
        eSLuminance8 = GL_SLUMINANCE8,
        eSLuminanceAlpha = GL_SLUMINANCE_ALPHA,
        eSLuminance8Alpha8 = GL_SLUMINANCE8_ALPHA8,
        eSRGB = GL_SRGB,
        eSRGB8 = GL_SRGB8,
        eSRGBAlpha = GL_SRGB_ALPHA,
        eSRGB8Alpha8 = GL_SRGB8_ALPHA8,
        eR8 = GL_R8,
        eR8SNorm = GL_R8_SNORM,
        eR16 = GL_R16,
        eR16SNorm = GL_R16_SNORM,
        eRG8 = GL_RG8,
        eRG8SNorm = GL_RG8_SNORM,
        eRG16 = GL_RG16,
        eRG16SNorm = GL_RG16_SNORM,
        eRGB8SNorm = GL_RGB8_SNORM,
        eRGB16SNorm = GL_RGB16_SNORM,
        eRGBA8SNorm = GL_RGBA8_SNORM,
        eRGB10A2UI = GL_RGB10_A2UI,
        eR16F = GL_R16F,
        eRG16F = GL_RG16F,
        eRGB16F = GL_RGB16F,
        eRGBA16F = GL_RGBA16F,
        eR32F = GL_R32F,
        eRG32F = GL_RG32F,
        eRGB32F = GL_RGB32F,
        eRGBA32F = GL_RGBA32F,
        eR11FG11FB10F = GL_R11F_G11F_B10F,
        eRGB9E5 = GL_RGB9_E5,
        eR8I = GL_R8I,
        eR8UI = GL_R8UI,
        eR16I = GL_R16I,
        eR16UI = GL_R16UI,
        eR32I = GL_R32I,
        eR32UI = GL_R32UI,
        eRG8I = GL_RG8I,
        eRG16I = GL_RG16I,
        eRG16UI = GL_RG16UI,
        eRG32I = GL_RG32I,
        eRG32UI = GL_RG32UI,
        eRGB8I = GL_RGB8I,
        eRGB16I = GL_RGB16I,
        eRGB16UI = GL_RGB16UI,
        eRGB32I = GL_RGB32I,
        eRGB32UI = GL_RGB32UI,
        eRGBA8I = GL_RGBA8I,
        eRGBA16I = GL_RGBA16I,
        eRGBA16UI = GL_RGBA16UI,
        eRGBA32I = GL_RGBA32I,
        eRGBA32UI = GL_RGBA32UI
    };


    enum TextureParameter {
        eDepthStencilTextureMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
        eTextureBaseLevel = GL_TEXTURE_BASE_LEVEL,
        eTextureCompareFunc = GL_TEXTURE_COMPARE_FUNC,
        eTextureCompareMode = GL_TEXTURE_COMPARE_MODE,
        eTextureLodBias = GL_TEXTURE_LOD_BIAS,
        eTextureMinFilter = GL_TEXTURE_MIN_FILTER,
        eTextureMagFilter = GL_TEXTURE_MAG_FILTER,
        eTextureMinLod = GL_TEXTURE_MIN_LOD,
        eTextureMaxLod = GL_TEXTURE_MAX_LOD,
        eTextureMaxLevel = GL_TEXTURE_MAX_LEVEL,
        eTextureSwizzleR = GL_TEXTURE_SWIZZLE_R,
        eTextureSwizzleG = GL_TEXTURE_SWIZZLE_G,
        eTextureSwizzleB = GL_TEXTURE_SWIZZLE_B,
        eTextureSwizzleA = GL_TEXTURE_SWIZZLE_A,
        eTextureWrapS = GL_TEXTURE_WRAP_S,
        eTextureWrapT = GL_TEXTURE_WRAP_T,
        eTextureWrapR = GL_TEXTURE_WRAP_R,
        eTextureMaxAnisotropy = GL_TEXTURE_MAX_ANISOTROPY_EXT,
        eTextureBorderColor = GL_TEXTURE_BORDER_COLOR
    };

    enum CubeMapTextureFace {
        eRight = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        eLeft = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        eTop = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        eBottom = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        eBack = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        eFront = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    const uint8_t g_kCubeMapFaceCount = 6;

    enum DataType {
        eUnsignedByte = GL_UNSIGNED_BYTE,
        eByte = GL_BYTE,
        eUnsignedShort = GL_UNSIGNED_SHORT,
        eShort = GL_SHORT,
        eUnsignedInt = GL_UNSIGNED_INT,
        eInt = GL_INT,
        eHalfFloat = GL_HALF_FLOAT,
        eFloat = GL_FLOAT,
        eUnsignedShort565 = GL_UNSIGNED_SHORT_5_6_5,
        eUnsignedShort4444 = GL_UNSIGNED_SHORT_4_4_4_4,
        eUnsignedShort5551 = GL_UNSIGNED_SHORT_5_5_5_1,
        eUnsignedInt2101010Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
        eUnsignedInt10F11F11FRev = GL_UNSIGNED_INT_10F_11F_11F_REV,
        eUnsignedInt5999Rev = GL_UNSIGNED_INT_5_9_9_9_REV,
        eUnsignedInt248 = GL_UNSIGNED_INT_24_8,
        eUnsignedFloat32UnsignedInt248Rev = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
    };

    enum BufferType {
        eArrayBuffer = GL_ARRAY_BUFFER,
        eAtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
        eCopyReadBuffer = GL_COPY_READ_BUFFER,
        eCopyWriteBuffer = GL_COPY_WRITE_BUFFER,
        eDispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,
        eDrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
        eElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
        ePixelPackBuffer = GL_PIXEL_PACK_BUFFER,
        ePixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
        eQueryBuffer = GL_QUERY_BUFFER,
        eShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
        eTextureBuffer = GL_TEXTURE_BUFFER,
        eTransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
        eUniformBuffer = GL_UNIFORM_BUFFER
    };

    enum BufferUsageType {
        eStaticDraw = GL_STATIC_DRAW,
        eDynamicDraw = GL_DYNAMIC_DRAW
    };

    enum PrimitiveType {
        ePoints = GL_POINTS,
        eLineStrip = GL_LINE_STRIP,
        eLineLoop = GL_LINE_LOOP,
        eLines = GL_LINES,
        eLineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        eLinesAdjacency = GL_LINES_ADJACENCY,
        eTriangleStrip = GL_TRIANGLE_STRIP,
        eTriangleFan = GL_TRIANGLE_FAN,
        eTriangles = GL_TRIANGLES,
        eTriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        eTrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
        ePatches = GL_PATCHES
    };

    enum PatchParameterType {
        ePatchVertices = GL_PATCH_VERTICES,
        ePatchDefaultOuterLevel = GL_PATCH_DEFAULT_OUTER_LEVEL,
        ePatchDefaultInnerLevel = GL_PATCH_DEFAULT_INNER_LEVEL
    };
}

#endif //VDGE_GLTYPES_HPP
