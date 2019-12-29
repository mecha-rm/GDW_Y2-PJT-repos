#pragma once
#include "Animation.h"

namespace cherry
{
	// Texture Animation
	class TextureAnimation : public Animation
	{
		TextureAnimation();
	};

	// Texture Animation Frame
	typedef class TextureAnimationFrame : public AnimationFrame
	{
	public:
		// uv coordinate for the current frame, which takes in the minimum uv and maximum uv.
		TextureAnimationFrame(glm::vec2 uvMin,glm::vec2 uvMax);

		// uv coordinate for the current frame
		TextureAnimationFrame(cherry::Vec2 uvMin, cherry::Vec2 uvMax);

		// uv coordinates for the current frame (as a vec4)
		// (x, y, z, w) = (x_min, y_min, x_max, y_max)
		TextureAnimationFrame(glm::vec4 uvs);

		// uv coordinates for the current frame (as a vec4)
		// (x, y, z, w) = (x_min, y_min, x_max, y_max)
		TextureAnimationFrame(cherry::Vec4 uvs);

		// gets passed a row and column number, and a width and height of a given size.
		// this goes from the top to the bottom, with row and colum starting at 0.
		// need to get maximum row and column count as well
		// TextureAnimationFrame(int row, int column, float width, float height);

		// gets the minimum u-coordinate
		float GetUMinimum() const;

		// gets the maximum u-coordinate
		float GetUMaximum() const;

		// gets the minimum v-coordinate
		float GetVMinimum() const;

		// gets the maximum v-coordinate
		float GetVMaximum() const;

	private:
	protected:

		glm::vec4 uvCoords; // (x_min, y_min, x_max, y_max)

	} TextureFrame;
}
