// loads in an image, which is mapped to a plane.
#pragma once
#include "Object.h"

namespace cherry
{
	class Image : public cherry::Object
	{
	public:
		// TODO: add variable for culling faces
		// the file path of the image
		// call createEntity to replace the image?
		Image(std::string filePath, std::string currentScene);

		// gets the width of the image.
		uint32_t GetWidth() const;

		// gets the height of the image.
		uint32_t GetHeight() const;

		// add update function
		// void Update();

	private:

		// loads the image
		bool LoadImage(std::string scene);

		// TODO: possibly change the Object header so that that file path is used instead?
		// the file path for the image
		std::string filePath;

		// the size of the image in pixels.
		glm::u32vec2 dimensions;

	protected:

	};
}
