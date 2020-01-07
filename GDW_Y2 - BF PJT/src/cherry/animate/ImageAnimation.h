#pragma once
#include "Animation.h"

namespace cherry
{
	// Texture Animation; used for the image class.
	class ImageAnimation : public Animation
	{
	public:
		// constructor.
		ImageAnimation();

		// updates the texture animation.
		void Update(float deltaTime);

	private:

		// TODO: add option to blend frames.
		// TODO: add size, orientation, and orientation changes.
		// SwitchFrames();

	protected:
	};

	// Texture Animation Frame
	typedef class ImageAnimationFrame : public AnimationFrame
	{
	public:
		// uv coordinate for the current frame, which takes in the minimum uv and maximum uv.
		ImageAnimationFrame(std::string filePath, glm::vec2 uvMin,glm::vec2 uvMax, float units = 1.0F);

		// uv coordinate for the current frame
		ImageAnimationFrame(std::string filePath, cherry::Vec2 uvMin, cherry::Vec2 uvMax, float units = 1.0F);

		// uv coordinates for the current frame (as a vec4)
		// (x, y, z, w) = (x_min, y_min, x_max, y_max)
		ImageAnimationFrame(std::string filePath, glm::vec4 uvs, float units = 1.0F);

		// uv coordinates for the current frame (as a vec4)
		// (x, y, z, w) = (x_min, y_min, x_max, y_max)
		ImageAnimationFrame(std::string filePath, cherry::Vec4 uvs, float units = 1.0F);

		// gets passed a row and column number, and a width and height of a given size.
		// this goes from the top to the bottom, with row and colum starting at 0.
		// need to get maximum row and column count as well
		// ImageAnimationFrame(int row, int column, float width, float height);

		// gets the file path of the image used for the current frame.
		const std::string GetFilePath() const;

		// gets the minimum UV values
		glm::vec2 GetUVMinimum() const;

		// gets the maximum UV values
		glm::vec2 GetUVMaxium() const;

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
		std::string image = "";

		glm::vec4 uvCoords; // (x_min, y_min, x_max, y_max)

	} ImageFrame;
}
