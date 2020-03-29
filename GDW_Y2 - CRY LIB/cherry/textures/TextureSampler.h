// Texture Sampler - used for mipmapping; taken from the Computer Graphics framework
#pragma once
#include <glad/glad.h>
#include <toolkit/EnumToString.h>
#include <GLM/glm.hpp>

#include "..\UtilsCRY.h"
#include "TextureEnums.h"

namespace cherry
{
	// // These are our options for GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_S and GL_TEXTURE_WRAP_R
	// ENUM(WrapMode, GLint,
	// 	ClampToEdge = GL_CLAMP_TO_EDGE,
	// 	ClampToBorder = GL_CLAMP_TO_BORDER,
	// 	MirroredRepeat = GL_MIRRORED_REPEAT,
	// 	Repeat = GL_REPEAT, // Default
	// 	MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	// );
	// 
	// // These are our available options for the GL_TEXTURE_MIN_FILTER setting
	// ENUM(MinFilter, GLint,
	// 	// (PER-TEXTURE-LEVEL)_MIPMAP_ (BETWEEN TEXTURE LEVELS)
	// 	Nearest = GL_NEAREST,
	// 	Linear = GL_LINEAR, // also referred to as tri-linear filtering
	// 	NearestMipNearest = GL_NEAREST_MIPMAP_NEAREST,
	// 	LinearMipNearest = GL_LINEAR_MIPMAP_NEAREST,
	// 	NearestMipLinear = GL_NEAREST_MIPMAP_LINEAR, // This is the default setting
	// 	LinearMipLinear = GL_LINEAR_MIPMAP_LINEAR
	// 
	// );
	// 
	// // These are our available options for the GL_TEXTURE_MAG_FILTER setting
	// ENUM(MagFilter, GLint,
	// 	Nearest = GL_NEAREST,
	// 	Linear = GL_LINEAR // This is the default setting
	// );

	struct SamplerDesc {
		WrapMode WrapS = WrapMode::Repeat;
		WrapMode WrapT = WrapMode::Repeat;
		WrapMode WrapR = WrapMode::Repeat;
		MinFilter MinFilter = MinFilter::NearestMipLinear;
		MagFilter MagFilter = MagFilter::Linear;

		glm::vec4 BorderColor = glm::vec4(0.0f);

		bool AnisotropicEnabled = true;
		float MaxAnisotropy = 1.0f;
	};

	class TextureSampler
	{
	public:
		GraphicsClass(TextureSampler);
		TextureSampler(const SamplerDesc& desc = SamplerDesc());
		~TextureSampler();

		// works on texture slots
		void Bind(uint32_t slot);

		// works on texure slots
		static void Unbind(uint32_t slot);

		const SamplerDesc& GetDescription() const { return myDesc; }

	private:
		GLuint myHandle;
		SamplerDesc myDesc;
	};
}
