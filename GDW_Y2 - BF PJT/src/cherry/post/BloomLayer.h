#pragma once

#include "PostLayer.h"

namespace cherry
{
	class BloomLayer
	{
	public:
		// default constructor
		BloomLayer();

		// creates bloom with a threshold.
		BloomLayer(float threshold);

		// gets the threshold
		float GetThreshold();

		// sets the threshold.
		void SetThreshold(float threshold);

		// returns the shader for this bloom effect.
		cherry::Shader::Sptr GetShader() const;

		// returns the frame buffer for this bloom effect.
		cherry::FrameBuffer::Sptr GetFrameBuffer() const;

		// generates a box blur shader
		static cherry::Shader::Sptr GenerateBoxBlur();
		
		// generates a guassian blur (3 X 3) shader
		static cherry::Shader::Sptr GenerateGuassianBlur3();
		
		// generates a guassian blur 5 shader
		static cherry::Shader::Sptr GenerateGuassianBlur5();

		// generates a radial blur
		// static cherry::Shader::Sptr GenerateRadialBlurCircular();

		// static cherry::Shader::Sptr GenerateRadicalBlurLinear();

		// static cherry::Shader::Sptr GenerateRadicalBlurZoom();

		// adds a pass to the bloom object.
		void AddPass(Shader::Sptr& shader, FrameBuffer::Sptr& buffer);

		// removes a pass from the bloom object.
		void RemovePass(unsigned int index);

		// gets the post processing layer.
		cherry::PostLayer::Sptr GetPostLayer() const;

	private:

		// threshold
		float threshold = 0;

		// post processing layer, shader, and buffer
		PostLayer::Sptr layer;

		Shader::Sptr mainShader;
		FrameBuffer::Sptr mainBuffer;


		// accumulation layer, shader and buffer
		Shader::Sptr accumShader;
		FrameBuffer::Sptr accumBuffer;

	protected:

	};
}