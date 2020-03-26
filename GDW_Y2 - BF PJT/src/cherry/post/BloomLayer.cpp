#include "BloomLayer.h"

// default constructor
cherry::BloomLayer::BloomLayer()
{
	shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_BLOOM_HIGHLIGHT_FS);

	frameBuffer = FrameBuffer::GenerateDefaultBuffer();

	layer = std::make_shared<PostLayer>(shader, frameBuffer);
}

// constructor with threshold
cherry::BloomLayer::BloomLayer(float threshold)
	: BloomLayer()
{
	SetThreshold(threshold);
}

// gets the threshold.
float cherry::BloomLayer::GetThreshold() { return threshold; }

// sets the threshold
void cherry::BloomLayer::SetThreshold(float threshold)
{
	// [0 - 1] range.
	this->threshold = glm::clamp(threshold, 0.0F, 1.0F);

	shader->SetUniform("a_Threshold", this->threshold);
}

// returns the shader
cherry::Shader::Sptr cherry::BloomLayer::GetShader() const
{
	return shader;
}

// gets the frame buffer.
cherry::FrameBuffer::Sptr cherry::BloomLayer::GetFrameBuffer() const
{
	return frameBuffer;
}

// generates a box blur shader
cherry::Shader::Sptr cherry::BloomLayer::GenerateBoxBlur()
{
	Shader::Sptr shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_BOX_BLUR_FS);

	return shader;
}

// generates a guassian blur (3 X 3) shader
cherry::Shader::Sptr cherry::BloomLayer::GenerateGuassianBlur3()
{
	Shader::Sptr shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_GAUSSIAN_BLUR3_FS);

	return shader;
}

// generates a guassian blur (5 X 5) shader
cherry::Shader::Sptr cherry::BloomLayer::GenerateGuassianBlur5()
{
	Shader::Sptr shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_GAUSSIAN_BLUR5_FS);

	return shader;
}

// returns the radial blur
cherry::Shader::Sptr cherry::BloomLayer::GenerateRadialBlur()
{
	Shader::Sptr shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_RADIAL_BLUR_CIRCULAR_FS);

	return shader;
}


// adds a pass
void cherry::BloomLayer::AddPass(Shader::Sptr& shader, FrameBuffer::Sptr& buffer)
{
	layer->AddLayer(shader, buffer);
}

// removes a pass
void cherry::BloomLayer::RemovePass(unsigned int index)
{
	layer->RemoveLayer(index);
}

// returns the post processing layer.
cherry::PostLayer::Sptr cherry::BloomLayer::GetPostLayer() const
{
	return layer;
}
