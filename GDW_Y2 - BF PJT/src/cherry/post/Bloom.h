#pragma once

#include "PostLayer.h"

namespace cherry
{
	class Bloom
	{
	public:
		// default constructor
		Bloom();

		// creates bloom with a threshold.
		Bloom(float threshold);

		// gets the threshold
		float GetThreshold();

		// sets the threshold.
		void SetThreshold(float threshold);

		// static Shader::Sptr GenerateBoxBlur();
		// 
		// static Shader::Sptr GenerateGuassianBlur();
		// 
		// static Shader::Sptr GenerateRadialBlur();

		// adds a pass to the bloom object.
		void AddPass(Shader::Sptr& shader, FrameBuffer::Sptr& buffer);

		// gets the post processing layer.
		cherry::PostLayer::Sptr GetPostLayer() const;

	private:
		// threshold
		float threshold = 0;

		// post processing layer
		PostLayer::Sptr layer;

		// shader
		Shader::Sptr shader;

		// frame buffer
		FrameBuffer::Sptr frameBuffer;

	protected:

	};
}