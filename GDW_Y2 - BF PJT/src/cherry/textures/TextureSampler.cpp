// Texture Sampler - used for mipmapping; taken from the Computer Graphics framework
#include "TextureSampler.h"
#include <GLM/gtc/type_ptr.hpp>

// essentially just copying parameters into our OpenGL state.
cherry::TextureSampler::TextureSampler(const SamplerDesc& desc) {
	myDesc = desc;
	glCreateSamplers(1, &myHandle);
	glSamplerParameteri(myHandle, GL_TEXTURE_WRAP_S, (GLenum)myDesc.WrapS);
	glSamplerParameteri(myHandle, GL_TEXTURE_WRAP_T, (GLenum)myDesc.WrapT);
	glSamplerParameteri(myHandle, GL_TEXTURE_WRAP_R, (GLenum)myDesc.WrapR);
	glSamplerParameteri(myHandle, GL_TEXTURE_MIN_FILTER, (GLenum)myDesc.MinFilter);
	glSamplerParameteri(myHandle, GL_TEXTURE_MAG_FILTER, (GLenum)myDesc.MagFilter);
	glSamplerParameterfv(myHandle, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(myDesc.BorderColor));
	if (myDesc.AnisotropicEnabled)
		glSamplerParameterf(myHandle, GL_TEXTURE_MAX_ANISOTROPY, myDesc.MaxAnisotropy);
}

cherry::TextureSampler::~TextureSampler() {
	glDeleteSamplers(1, &myHandle);
}
void cherry::TextureSampler::Bind(uint32_t slot) {
	glBindSampler(slot, myHandle);
}
void cherry::TextureSampler::Unbind(uint32_t slot) {
	glBindSampler(slot, 0);
}
