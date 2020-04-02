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

		/*
		 * generates a circular radial blur
		 * Variables:
			* center: source of blur (in screen space) [0, 1]
			* angle (in radians): the angle of the blur
			* samples: the amount of samples to take for the blur. The higher the value the more accurate the effect.
			* direction: the direction of the blur. A positive value is clockwise, negative is counter clockwise, and 0 is both.
		*/
		static cherry::Shader::Sptr GenerateRadialBlurCircular(glm::vec2 center, float angle, int samples, int direction);

		// linear radial blur
		static cherry::Shader::Sptr GenerateRadialBlurLinear(float length, float angle, int samples);

		// zoom radial blur
		static cherry::Shader::Sptr GenerateRadialBlurZoom(glm::vec2 center, float intensity, int samples);

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