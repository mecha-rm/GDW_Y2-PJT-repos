#include "Bloom.h"

// default constructor
cherry::Bloom::Bloom()
{
	shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_BLOOM_HIGHLIGHT_FS);

	frameBuffer = FrameBuffer::GenerateDefaultBuffer();

	layer = std::make_shared<PostLayer>(shader, frameBuffer);
}

// constructor with threshold
cherry::Bloom::Bloom(float threshold)
	: Bloom()
{
	SetThreshold(threshold);
}

// gets the threshold.
float cherry::Bloom::GetThreshold() { return threshold; }

// sets the threshold
void cherry::Bloom::SetThreshold(float threshold)
{
	// [0 - 1] range.
	this->threshold = glm::clamp(threshold, 0.0F, 1.0F);

	shader->SetUniform("a_Threshold", this->threshold);
}

// adds a pass
void cherry::Bloom::AddPass(Shader::Sptr& shader, FrameBuffer::Sptr& buffer)
{
	layer->AddLayer(shader, buffer);
}

// returns the post processing layer.
cherry::PostLayer::Sptr cherry::Bloom::GetPostLayer() const
{
	return layer;
}
