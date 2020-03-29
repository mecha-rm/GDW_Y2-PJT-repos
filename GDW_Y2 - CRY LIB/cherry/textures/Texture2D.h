// Texture2D - loads in texture images
#pragma once
// #include <glad\glad.h>
#include <memory>
#include <string>
#include <toolkit/EnumToString.h>

#include "..\UtilsCRY.h" // including macros
#include "TextureSampler.h"
#include "ITexture.h"

namespace cherry
{
	// Moved to TextureEnums
	
	// Formats: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
	// These are some of our more common available internal formats
	// ENUM(InternalFormat, GLint,
	// 	Depth = GL_DEPTH_COMPONENT,
	// 	DepthStencil = GL_DEPTH_STENCIL,
	// 	R8 = GL_R8,
	// 	R16 = GL_R16,
	// 	RGB8 = GL_RGB8,
	// 	RGB16 = GL_RGB16,
	// 	RGBA8 = GL_RGBA8,
	// 	RGBA16 = GL_RGBA16
	// 	// Note: there are sized internal formats, but there are a LOT of them.
	// );
	// 
	// // The layout of the input pixel data
	// ENUM(PixelFormat, GLint,
	// 	Red = GL_RED,
	// 	Rg = GL_RG,
	// 	Rgb = GL_RGB,
	// 	Bgr = GL_BGR,
	// 	Rgba = GL_RGBA,
	// 	Bgra = GL_BGRA,
	// 	Depth = GL_DEPTH_COMPONENT,
	// 	DepthStencil = GL_DEPTH_STENCIL
	// );
	// 
	// // The type for each component of the pixel data
	// ENUM(PixelType, GLint,
	// 	UByte = GL_UNSIGNED_BYTE,
	// 	Byte = GL_BYTE,
	// 	UShort = GL_UNSIGNED_SHORT,
	// 	Short = GL_SHORT,
	// 	UInt = GL_UNSIGNED_INT,
	// 	Int = GL_INT,
	// 	Float = GL_FLOAT
	// );

	// Represents some of the data required to set up our texture (but does not actually load in its data).
	// This is more or less all the GPU state that we care about at the moment.
	struct Texture2DDescription {
		uint32_t Width = 0;
		uint32_t Height = 0;
		InternalFormat Format = InternalFormat::RGBA8;

		bool EnableMip = false;
		int MipLevels = -1;
		uint32_t NumSamples = 1;
		SamplerDesc Sampler = SamplerDesc();
	};

	// Represents a 2D texture in OpenGL.
	// TODO: currently ITexture does nothing.
	class Texture2D : public cherry::ITexture
	{
	public:
		GraphicsClass(Texture2D);

		// typedef std::shared_ptr<Texture2D> Sptr; // uses GraphicsClass now
		Texture2D(const Texture2DDescription& description);

		virtual ~Texture2D();

		// handles uploading data to the GPU
		void LoadData(void* data, size_t width, size_t height, PixelFormat format, PixelType type);

		// overriding the version in ITexture
		// void Bind(uint32_t slot) override;

		// binds the texture, which OpenGL gives us about a minimum of 80 slots for.
		void Bind(int slot) const;

		static void UnBind(int slot);

		// loading from the file so that we don't have to manually apply a texture.
		static Sptr LoadFromFile(const std::string& fileName, bool loadAlpha = true);

		// gets the limit on the width of the texture. This is the same as the height.
		// this is based on the GPU of the user.
		static int GetMaximumSideLength();

		// gets the width of the image.
		uint32_t GetWidth() const;

		// gets the height of the image.
		uint32_t GetHeight() const;

	protected:
		// sets up the texture
		void __SetupTexture();

		GLuint myTextureHandle; // what we'll wrap around.
		Texture2DDescription myDescription;

		// the maximum number of samples
		static uint32_t MaxNumSamples;
	};
}