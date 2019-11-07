#include "Texture2D.h"
#include <toolkit/Logging.h>
#include <stbs/stb_image.h> // single header file for image loading


Texture2D::Texture2D(const Texture2DDescription& desc) {
	myDescription = desc;
	myTextureHandle = 0; // checked later in case the project breaks and goes crazy.
	__SetupTexture();
}

// deletes the texture
Texture2D::~Texture2D() {
	glDeleteTextures(1, &myTextureHandle);
}

void Texture2D::__SetupTexture() {
	glCreateTextures(GL_TEXTURE_2D, 1, &myTextureHandle);
	
	// allocates space on the GPU for a 2D texture.
	glTextureStorage2D(myTextureHandle, 1,
		(GLenum)myDescription.Format, myDescription.Width, myDescription.Height);
}

// binds a texture into a given slot.
void Texture2D::Bind(int slot) const {
	// Bind to the given texture slot, OpenGL 4 guarantees that we have at least 80 texture slots
	// Note that this is part of Direct State Access added in 4.5, replacing the old glActiveTexture and glBindTexture calls
	glBindTextureUnit(slot, myTextureHandle);
}


void Texture2D::UnBind(int slot) {
	// Binding zero to a texture slot will unbind the texture
	glBindTextureUnit(slot, 0);
}

void Texture2D::LoadData(void* data, size_t width, size_t height, PixelFormat format, PixelType type) {
	
	// checks size of data against size of texture.
	// TODO: Re-create texture if our data is a different size
	LOG_ASSERT(width == myDescription.Width, "Width of data does not match the width of this texture!");
	LOG_ASSERT(height == myDescription.Height, "Height of data does not match the width of this texture!");
	
	// usually you want to always upload to 0. The first zero is followed by two more, which is the (x, y) offset.
	glTextureSubImage2D(myTextureHandle, 0, 0, 0,
		myDescription.Width, myDescription.Height, (GLenum)format, (GLenum)type, data);
}

// loads in the texture from a file.
Texture2D::Sptr Texture2D::LoadFromFile(const std::string& fileName, bool loadAlpha) {
	
	// tells the file, the size, and the number of channel.
	int width, height, numChannels;
	void* data = stbi_load(fileName.c_str(), &width, &height, &numChannels, loadAlpha ? 4 : 3);
	if (data != nullptr && width != 0 && height != 0 && numChannels != 0) {
		Texture2DDescription desc = Texture2DDescription();
		desc.Width = width;
		desc.Height = height;

		// format differs based on whether we loaded the alpha or not.
		desc.Format = loadAlpha ? InternalFormat::RGBA8 : InternalFormat::RGB8;
		Sptr result = std::make_shared<Texture2D>(desc);
		result->LoadData(data, width, height, loadAlpha ? PixelFormat::Rgba : PixelFormat::Rgb, PixelType::UByte);
		
		// image files take up a lot of data, so we call this to clean up the data once it's on our GPU.
		stbi_image_free(data);
		return result;
	}
	else {
		// not throwing exceptions since we can't use the file, but for now we just have a warning.
		LOG_WARN("Failed to load image from \"{}\"", fileName);

//#ifndef _DEBUG
//		return LoadFromFile("res/images/default.png", loadAlpha);
//#endif // !_DEBUG

		return nullptr;
	}
}