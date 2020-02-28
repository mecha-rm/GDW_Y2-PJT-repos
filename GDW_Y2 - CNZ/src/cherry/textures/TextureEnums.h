#pragma once
#include "..\UtilsCRY.h"
#include <toolkit/EnumToString.h>
#include <glad/glad.h>

namespace cherry
{
	// Formats: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
	// These are some of our more common available internal formats
	ENUM(InternalFormat, GLint,
		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,
		R8 = GL_R8,
		R16 = GL_R16,
		RGB8 = GL_RGB8,
		RGB16 = GL_RGB16,
		RGBA8 = GL_RGBA8,
		RGBA16 = GL_RGBA16
		// Note: there are sized internal formats, but there are a LOT of them.
	);

	// The layout of the input pixel data
	ENUM(PixelFormat, GLint,
		Red = GL_RED,
		Rg = GL_RG,
		Rgb = GL_RGB,
		Bgr = GL_BGR,
		Rgba = GL_RGBA,
		Bgra = GL_BGRA,
		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL
	);

	// The type for each component of the pixel data
	ENUM(PixelType, GLint,
		UByte = GL_UNSIGNED_BYTE,
		Byte = GL_BYTE,
		UShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Float = GL_FLOAT
	);

	// TODO: Extras from ICG_2

	// These are our options for GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_S and GL_TEXTURE_WRAP_R
	ENUM(WrapMode, GLint,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		Repeat = GL_REPEAT, // Default
		MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	);

	// These are our available options for the GL_TEXTURE_MIN_FILTER setting
	ENUM(MinFilter, GLint,
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipLinear = GL_NEAREST_MIPMAP_LINEAR, // This is the default setting
		LinearMipLinear = GL_LINEAR_MIPMAP_LINEAR
	);

	// These are our available options for the GL_TEXTURE_MAG_FILTER setting
	ENUM(MagFilter, GLint,
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR  // This is the default setting
	);

	/*
	 * Gets the number of bytes needed to represent a single texel of the given format and type
	 * @param format The format of the texel
	 * @param type The data type of the texel
	 * @returns The size of a single texel of the given format and type, in bytes
	 */
	constexpr size_t GetTexelSize(PixelFormat format, PixelType type) {
		size_t result = 0;

		switch (type) {
		case PixelType::UByte:
		case PixelType::Byte:
			result = 1; break;
		case PixelType::UShort:
		case PixelType::Short:
			result = 2; break;
		case PixelType::Int:
		case PixelType::UInt:
			result = 4; break;
		default:
			_ASSERT(false);
		}

		switch (format)
		{
		case PixelFormat::Depth:
		case PixelFormat::DepthStencil:
		case PixelFormat::Red:
			result *= 1; break;
		case PixelFormat::Rg:
			result *= 2; break;
		case PixelFormat::Rgb:
		case PixelFormat::Bgr:
			result *= 3; break;
		case PixelFormat::Rgba:
		case PixelFormat::Bgra:
			result *= 4; break;
		default:
			_ASSERT(false);
		}

		return result;
	}
}
