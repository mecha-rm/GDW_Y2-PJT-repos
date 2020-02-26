// Texture Cube - used for making cube maps; received from the Computer Graphics framework
#pragma once
#include <glad/glad.h>
#include <string>
#include <toolkit/EnumToString.h>
#include "Texture2D.h"
#include "..\UtilsCRY.h"

namespace cherry
{
	// this is how the cube map refers to its faces.
	ENUM(CubeMapFace, GLint,
		PosX = 0,
		NegX = 1,
		PosY = 2,
		NegY = 3,
		PosZ = 4,
		NegZ = 5
	);

	struct TextureCubeDesc {
		uint32_t Size = 0;
		InternalFormat Format = InternalFormat::RGBA8;
	};

	class TextureCube {
	public:
		GraphicsClass(TextureCube);
		TextureCube(const TextureCubeDesc& desc);
		virtual ~TextureCube();

		// loading from six different files for now.
		void LoadData(uint32_t width, uint32_t height, CubeMapFace face, PixelFormat format, PixelType type, void* data);
		static Sptr LoadFromFiles(const std::string faceFiles[6]);
		void Bind(int slot);
		static void Unbind(int slot);

	protected:
		GLuint myHandle;
		TextureCubeDesc myDesc;
		void __InitTexture();
	};
}