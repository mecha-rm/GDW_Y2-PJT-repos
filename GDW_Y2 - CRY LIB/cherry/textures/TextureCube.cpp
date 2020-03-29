// TextureCube - used for making cube maps; taken from the Computer Graphics framework.
#include "TextureCube.h"
#include <toolkit/Logging.h>
#include <stbs/stb_image.h>

cherry::TextureCube::TextureCube(const TextureCubeDesc& desc) {
	myDesc = desc;
	myHandle = 0;
	__InitTexture();
}
cherry::TextureCube::~TextureCube() { glDeleteTextures(1, &myHandle); }

void cherry::TextureCube::Bind(int slot) { glBindTextureUnit(slot, myHandle); }

void cherry::TextureCube::Unbind(int slot) { glBindTextureUnit(slot, 0); }

void cherry::TextureCube::__InitTexture() {
	GLenum format = (GLenum)myDesc.Format;

	// only differece from Texture2D and this class is that we use GL_TEXTURE_CUBE_MAP here.
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &myHandle);

	glTextureParameteri(myHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(myHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(myHandle, GL_TEXTURE_MIN_FILTER, (GLenum)MinFilter::Linear);
	glTextureParameteri(myHandle, GL_TEXTURE_MAG_FILTER, (GLenum)MagFilter::Linear);
	glTextureStorage2D(myHandle, 1, format, myDesc.Size, myDesc.Size);
}

void cherry::TextureCube::LoadData(uint32_t width, uint32_t height, CubeMapFace face, PixelFormat format, PixelType type, void* data) {
	glTextureSubImage3D(myHandle, 0, 0, 0, (int)face, myDesc.Size, myDesc.Size, 1, (GLenum)format, (GLenum)type, data);
}

cherry::TextureCube::Sptr cherry::TextureCube::LoadFromFiles(const std::string faceFiles[6]) {
	TextureCubeDesc desc = TextureCubeDesc();
	desc.Format = InternalFormat::RGB8;
	stbi_set_flip_vertically_on_load(true);
	Sptr result = nullptr;
	
	for (int ix = 0; ix < 6; ix++) {
		int width, height, numChannels;
		void* data = stbi_load(faceFiles[ix].c_str(), &width, &height, &numChannels, 3);
		if (desc.Size != 0 && ((width != desc.Size) | (height != desc.Size))) {
			stbi_image_free(data);
			LOG_ASSERT(false, "Image file dimensions do not match the size of this cubemap! ({})", faceFiles[ix]);
		}
		if (width != height) {
			stbi_image_free(data);
			LOG_ASSERT(false, "Image for cubemap must be square! ({})", faceFiles[ix]);
		}
		
		if (ix == 0) {
			desc.Size = width;
			result = std::make_shared<TextureCube>(desc);
		}
		if (data != nullptr && width != 0 && height != 0 && numChannels != 0) {
			result->LoadData(width, height, (CubeMapFace)ix,
				numChannels == 4 ? PixelFormat::Rgba : PixelFormat::Rgb,
				PixelType::UByte, data);
			stbi_image_free(data);
		}
		else {
			LOG_WARN("Failed to load image from \"{}\"", faceFiles[ix]);
		}
		
	}
	return result;
}