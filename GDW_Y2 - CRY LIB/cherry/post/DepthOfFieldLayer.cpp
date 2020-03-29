#include "DepthOfFieldLayer.h"

// constructor
cherry::DepthOfFieldLayer::DepthOfFieldLayer()
{
	shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_DOF_FS);

	// gets the default frame buffer
	frameBuffer = FrameBuffer::GenerateDefaultBuffer();

	layer = std::make_shared<PostLayer>(shader, frameBuffer);

	// default values (based on the tutorial)
	SetFocalDepth(3.0F);
	SetLenseDistance(1.0F);
	SetAperture(20.0F);
}

cherry::DepthOfFieldLayer::DepthOfFieldLayer(float focalDepth, float lenseDist, float aperture)
	: DepthOfFieldLayer()
{
	SetFocalDepth(focalDepth);
	SetLenseDistance(lenseDist);
	SetAperture(aperture);
}

// gets the focal depth
float cherry::DepthOfFieldLayer::GetFocalDepth() const { return focalDepth; }

// sets the focal depth
void cherry::DepthOfFieldLayer::SetFocalDepth(float a_FocalDepth)
{
	focalDepth = a_FocalDepth;
	shader->SetUniform("a_FocalDepth", focalDepth);
}

// gets the lense distance
float cherry::DepthOfFieldLayer::GetLenseDistance() const { return lenseDistance; }

void cherry::DepthOfFieldLayer::SetLenseDistance(float a_LensDistance)
{
	lenseDistance = a_LensDistance;
	shader->SetUniform("a_LenseDistance", lenseDistance);
}

// gets the aperture size
float cherry::DepthOfFieldLayer::GetAperture() const { return aperture; }

// sets the aperture
void cherry::DepthOfFieldLayer::SetAperture(float a_Aperture)
{
	aperture = a_Aperture;
	shader->SetUniform("a_Aperture", aperture);
}

// gets the post layer
cherry::PostLayer::Sptr cherry::DepthOfFieldLayer::GetPostLayer() const
{
	return layer;
}