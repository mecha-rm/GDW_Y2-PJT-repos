// Image (Header) - loads in an image, which is mapped to a plane.
#pragma once
#include "Object.h"

namespace cherry
{
	// inherits the object
	class Image : public cherry::Object
	{
	public:
		// TODO: add variable for culling faces
		// the file path of the image
		// call createEntity to replace the image?
		// doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		// backSameAsFront: makes the front and back of the image the exact same if true. 
		//	* If false, the back is a reflection of the front. This only shows up if double sided is set to 'true'.
		Image(std::string filePath, std::string currentScene, bool doubleSided);

		~Image();

		// gets the width of the image.
		uint32_t GetWidth() const;

		// gets the height of the image.
		uint32_t GetHeight() const;

		// add update function
		// void Update();

	private:

		// loads the image
		bool LoadImage(std::string scene, bool doubleSided = true);

		// TODO: possibly change the Object header so that that file path is used instead?
		// the file path for the image
		std::string filePath;

		// the size of the image in pixels.
		glm::u32vec2 dimensions;

	protected:

	};
}
