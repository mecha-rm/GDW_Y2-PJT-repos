#pragma once
#include "..\IGraphicsResource.h"
#include "TextureEnums.h"

// this was also present in the engine
// currently no unique functionality
namespace cherry
{
	class ITexture : public cherry::IGraphicsResource
	{
	public:
		ITexture() = default;

		~ITexture() = default;

		// /*
		// 	Gets the internal format of this texture, which indicates how each
		// 	individual texel is composed (ex: RGB8)
		// 	@returns The textures internal texel format
		// */
		// cherry::InternalFormat GetInternalFormat() const;
		// 
		// 
		// /*
		// 	Binds this texture to the given texture slot. Different renderers will
		// 	have different amounts of slots, in OpenGL this can be retrieved by
		// 	querying GL_MAX_TEXTURE_IMAGE_UNITS
		// 	@param slot The texture slot to bind this texture to
		// */
		// virtual void Bind(uint32_t slot);

	private:

	protected:
		// default value
		// cherry::InternalFormat myInternalFormat = InternalFormat::RGBA8;
	};
}