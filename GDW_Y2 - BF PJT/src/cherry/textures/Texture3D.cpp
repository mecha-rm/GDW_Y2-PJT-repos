/*
 * Name: Roderick "R.J." Montague (100701758)
 * Date: 02/04/2020
 * Description: three-dimensional textures. This is modeled after the Texture2D class.
 * References:
*/

// 3D Textures
#include "Texture3D.h"

#include <toolkit/Logging.h>
#include <GLM/gtc/type_ptr.hpp>
#include <filesystem>
#include <stbs/stb_image.h>
#include "..\post/LookUpTable.h"


uint32_t cherry::Texture3D::MaxTextureSize = 0;
uint32_t cherry::Texture3D::MaxNumSamples = 1;

cherry::Texture3D::Texture3D(const Texture3dDescription& description) {
	if (MaxTextureSize == 0) {
		int size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
		MaxTextureSize = size;
		LOG_INFO("Maximum texture size on this renderer is {} texels", MaxTextureSize);

		glGetIntegerv(GL_MAX_SAMPLES, &size);
		MaxNumSamples = size;
		LOG_INFO("Maximum number of samples is {}", MaxNumSamples);
	}

	// Ensure that the texture size is valid
	LOG_ASSERT(description.Width > 0, "Texture width must be > 0");
	LOG_ASSERT(description.Height > 0, "Texture height must be > 0");
	LOG_ASSERT(description.Depth > 0, "Texture depth must be > 0");

	LOG_ASSERT(description.Width <= MaxTextureSize, "Texture width must be < {}", MaxTextureSize);
	LOG_ASSERT(description.Height <= MaxTextureSize, "Texture height must be < {}", MaxTextureSize);
	LOG_ASSERT(description.Depth <= MaxTextureSize, "Texture depth must be < {}", MaxTextureSize);

	// Store the texture description
	myDescription = description;
	// Creates the texture on the GPU
	__RecreateTexture();
}

cherry::Texture3D::~Texture3D() {
	LOG_INFO("Deleting texture with ID: {}", myRendererID);
	glDeleteTextures(1, &myRendererID);
}

// loading from the LUT (.CUBE)
// select an eternal format (RGB8?)
cherry::Texture3D::Sptr cherry::Texture3D::LoadFromFile(const std::string& filePath, bool loadAlpha)
{
	LookUpTable temp(filePath);

	// makes a lookup table from the file
	return LoadFromLookUpTable(temp, loadAlpha);
}

// loads from a lookup table object.
cherry::Texture3D::Sptr cherry::Texture3D::LoadFromLookUpTable(LookUpTable & lut, bool loadAlpha)
{
	std::string name = lut.GetName();
	std::string filePath = lut.GetFilePath();

	// Load the image data from the file 
	int width, height, depth, numChannels;
	// void* data = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0); 
	void* data = lut.GetDataPoints();

	// the LookUp Table 3D size (in pixels)
	width = lut.GetLut3DSize();
	height = lut.GetLut3DSize();
	depth = lut.GetLut3DSize();

	if (loadAlpha)
		numChannels = 4; // RGBA8
	else
		numChannels = 3; // RGB8 (no alpha)

	// We have data!
	InternalFormat internal_format;
	PixelFormat    image_format;
	switch (numChannels) {
	case 1:
		internal_format = InternalFormat::R8;
		image_format = PixelFormat::Red;
		break;
	case 2:
		internal_format = InternalFormat::RG8;
		image_format = PixelFormat::Rg;
		break;
	case 3:
		internal_format = InternalFormat::RGB8;
		image_format = PixelFormat::Rgb;
		break;

	case 4:
		internal_format = InternalFormat::RGBA8;
		image_format = PixelFormat::Rgba;
		break;

	default:
		LOG_WARN("Unsupported texture format. Texture may look strange. ({})", name);
		internal_format = InternalFormat::RGBA8;
		image_format = PixelFormat::Rgba;
		break;
	}

	LOG_ASSERT((numChannels * width) % 4 == 0, "The alignment of a horizontal line in a texture must be 4 bytes! Check that your number of channels * your width is a multiple of 4");

	// Only create the texture if we actually got data out
	if (data != nullptr && width != 0 && height != 0 && numChannels != 0) {
		// Create our texture description
		Texture3dDescription desc = Texture3dDescription();
		desc.Width = width;
		desc.Height = height;
		desc.Depth = depth;
		desc.Format = internal_format;

		// Define what data will be loaded into the texture, and it's format
		Texture3dData texData = Texture3dData();
		texData.Width = width;
		texData.Height = height;
		texData.Depth = depth;
		texData.Format = image_format;
		texData.Type = PixelType::UByte;
		texData.Data = data;

		// Create the image, and load the data into it
		Sptr result = std::make_shared<Texture3D>(desc);
		result->SetData(texData);
		// result->SetDebugName(std::filesystem::path(filePath).filename().string());
		result->SetDebugName(name);

		// Free the underlying image data and return the image
		// stbi_image_free(texData.Data); // not needed?
		return result;
	}
	else {
		// Free the data just, to be safe
		// stbi_image_free(data); // not needed?
		LOG_WARN("Failed to load image from \"{}\"", name);
		return nullptr;
	}

	return nullptr;
}

void cherry::Texture3D::LoadData(uint32_t width, uint32_t height, PixelFormat format, PixelType type, void* data)
{
	glTextureSubImage3D(myRendererID, 0, 0, 0, 0, myDescription.Width, myDescription.Height, myDescription.Height
		, (GLenum)format, (GLenum)type, data);
}

void cherry::Texture3D::SetData(const Texture3dData& data) {
	// We cannot upload data to a multi-sampled texture, it must be rendered to
	LOG_ASSERT(myDescription.NumSamples == 1, "Cannot upload data to a multi-sampled texture!"); // NEW

	// Make sure that the dimensions of the data match the dimension of the texture
	LOG_ASSERT(data.Width == myDescription.Width, "Data width does not match texture width");
	LOG_ASSERT(data.Height == myDescription.Height, "Data height does not match texture depth");
	LOG_ASSERT(data.Depth == myDescription.Depth, "Data depth does not match texture depth");

	// Load the data into the texture
	glTextureSubImage3D(myRendererID, 0, 0, 0, 0, myDescription.Width, myDescription.Height, myDescription.Depth, (GLenum)data.Format, (GLenum)data.Type, data.Data);
	// glTexImage3D(myRendererID, 0, GL_RGB, myDescription.Width, myDescription.Height, myDescription.Depth, 0, GL_RGB, GL_FLOAT, &data.Data);
	// This is an older version
	// glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, LUTsize, LUTsize, LUTsize, 0, GL_RGB, GL_FLOAT, &LUT[0]);

	// Generate the texture's mip maps
	if (myDescription.MipmapLevels > 1)
		glGenerateTextureMipmap(myRendererID);
}

void* cherry::Texture3D::GetData(PixelFormat format, PixelType type) const {
	size_t dataSize = GetTexelSize(format, type) * myDescription.Width * myDescription.Height * myDescription.Depth;
	void* result = malloc(dataSize);
	glGetTextureImage(myRendererID, 0, (GLenum)format, (GLenum)type, dataSize, result);
	return result;
}

void cherry::Texture3D::__RecreateTexture() {
	// If our miplevels is 0, we need to calculate how many mipmap levels we should generate
	myDescription.MipmapLevels = myDescription.MipmapLevels == 0 ?
		(1 + floor(log2(glm::max(myDescription.Width, myDescription.Height)))) :
		(myDescription.MipmapLevels);

	// Clamp our number of samples to the valid range
	myDescription.NumSamples = glm::clamp(myDescription.NumSamples, 1u, MaxNumSamples); // NEW
	myDescription.MipmapLevels = myDescription.NumSamples > 1 ? 1 : myDescription.MipmapLevels; // NEW

	// We don't want multi-sampling
	// making a 3D texture
	glCreateTextures(GL_TEXTURE_3D, 1, &myRendererID);

	// NEW
	if (myDescription.NumSamples == 1) {
		// glTextureStorage2D(myRendererID, myDescription.MipmapLevels, (GLenum)myDescription.Format, myDescription.Width, myDescription.Height);
		// we aren't doing mipmapping for 3D textures, so it's just left as 1.
		glTextureStorage3D(myRendererID, myDescription.MipmapLevels, (GLenum)myDescription.Format, myDescription.Width, myDescription.Height, myDescription.Depth);

		// Texture2D
		// glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_S, (GLenum)myDescription.WrapS);
		// glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_T, (GLenum)myDescription.WrapT);
		// glTextureParameteri(myRendererID, GL_TEXTURE_MIN_FILTER, (GLenum)myDescription.MinFilter);
		// glTextureParameteri(myRendererID, GL_TEXTURE_MAG_FILTER, (GLenum)myDescription.MagFilter);

		// Texture3D (slides) - this caused crashing, so it was removed.
		// glTextureParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTextureParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glTextureParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTextureParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// glTextureParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_S, (GLenum)myDescription.WrapS);
		glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_T, (GLenum)myDescription.WrapT);
		glTextureParameteri(myRendererID, GL_TEXTURE_MIN_FILTER, (GLenum)myDescription.MinFilter);
		glTextureParameteri(myRendererID, GL_TEXTURE_MAG_FILTER, (GLenum)myDescription.MagFilter);

		glTextureParameterfv(myRendererID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(myDescription.BorderColor));

		if (myDescription.Aniso != 1.0f)
			glTextureParameterf(myRendererID, GL_TEXTURE_MAX_ANISOTROPY, myDescription.Aniso);
	}
	else
		glTextureStorage3DMultisample(myRendererID, myDescription.NumSamples, (GLenum)myDescription.Format, myDescription.Width, myDescription.Height, myDescription.Depth, true);

}
