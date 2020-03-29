#pragma once

#include "..\Camera.h"
#include "PostLayer.h"

namespace cherry
{
	typedef class DepthOfFieldLayer
	{
	public:
		// constructor
		DepthOfFieldLayer();

		// constructor
		DepthOfFieldLayer(float focalDepth, float lenseDist, float aperture);

		// gets the focal depth
		float GetFocalDepth() const;

		// sets the focal depth of the camera. This is used for the Depth of Field post processing layer.
		void SetFocalDepth(float a_FocalDepth);

		// gets the lense distance of the camera
		float GetLenseDistance() const;

		// sets the focal depth of the camera. This is used for the Depth of Field post processing layer.
		void SetLenseDistance(float a_LensDistance);

		// gets the aperture size of the camera.
		float GetAperture() const;

		// sets the aperture of the camera. This is used for the Depth of Field post processing layer.
		void SetAperture(float a_Aperture);

		// gets the post processing layer
		cherry::PostLayer::Sptr GetPostLayer() const;

	private:
		// depth of field 
		PostLayer::Sptr layer;
		Shader::Sptr shader;
		FrameBuffer::Sptr frameBuffer;

		float aperture = 20.0F;
		float lenseDistance = 0.0F;
		float focalDepth = 0.0F;

	protected:
	} DOFLayer;
}