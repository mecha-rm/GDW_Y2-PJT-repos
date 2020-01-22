// Image (Header) - loads in an image, which is mapped to a plane.
#pragma once
#include "Object.h"

namespace cherry
{
	// inherits the object
	class Image : public cherry::Object
	{
	public:
		// TODO: allow option to provide values based on pixels rather than uvs.
		/*
		 * creates an image, and adds it to the provided scene (CreateEntity is called automatically).
		 * the uvs range from [0, 1], and the size of the image is based on the image size itself.
		 * Arguments:
		 *** doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		 *** duplicateFront: makes the front and back of the image the exact same if true.
		 *****	If false, the back is a reflection of the front. If this is 'true', the value of 'doubleSided' is disregarded.
		*/
		Image(std::string filePath, std::string scene, bool doubleSided = true, bool duplicateFront = false, bool camLock = false);

		/*
		 * creates an image with the size, taking in the size of the image. The entire image is used to, so the uvs range from [0, 1].
		 * the width and height determine the size of the default image.
		 * Variables:
		 *** doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		 *** duplicateFront: makes the front and back of the image the exact same if true.
		 ***** If false, the back is a reflection of the front. If this is 'true', the value of 'doubleSided' is disregarded.
		*/
		Image(std::string filePath, std::string scene, float width, float height, bool doubleSided = true, bool duplicateFront = false, bool camLock = false);
		
		/*
		 * creates an image with the size, taking in the size of the image. The entire image is used to, so the uvs range from [0, 1].
		 * the width and height determine the size of the default image.
		 * Variables:
		 *** size: the length and width of the image.
		 *** doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		 *** duplicateFront: makes the front and back of the image the exact same if true.
		 ***** If false, the back is a reflection of the front. If this is 'true', the value of 'doubleSided' is disregarded.
		*/
		Image(std::string filePath, std::string scene, cherry::Vec2 size, bool doubleSided = true, bool duplicateFront = false, bool camLock = false);

		/*
		 * creates an image, and adds it to the provided scene (CreateEntity is called automatically).
		 * the size of the image is based on the provided uvs.
		 * Arguments:
		 *** doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		 *** duplicateFront: makes the front and back of the image the exact same if true.
		 *****	If false, the back is a reflection of the front. If this is 'true', the value of 'doubleSided' is disregarded.
		*/
		Image(std::string filePath, std::string scene, cherry::Vec4 uvs, bool doubleSided = true, bool duplicateFront = false, bool camLock = false);

		/*
		 * creates an image with a provided width, height, and uv coordinates.
		 * Arguments:
		 *** doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		 *** duplicateFront: makes the front and back of the image the exact same if true.
		 *****	If false, the back is a reflection of the front. If this is 'true', the value of 'doubleSided' is disregarded.
		*/
		Image(std::string filePath, std::string scene, float width, float height, cherry::Vec4 uvs, bool doubleSided = true, bool duplicateFront = false, bool camLock = false);

		/*
		 * creates an image with a provided width, height, and uv coordinates.
		 * Arguments:
		 *** size: the size of the image.
		 *** doubleSided: the back faces are rendered for the image if 'doubeSided' is true.
		 *** duplicateFront: makes the front and back of the image the exact same if true.
		 *****	If false, the back is a reflection of the front. If this is 'true', the value of 'doubleSided' is disregarded.
		*/
		Image(std::string filePath, std::string scene, cherry::Vec2 size, cherry::Vec4 uvs, bool doubleSided = true, bool duplicateFront = false, bool camLock = false);


		// destructor
		~Image();

		// gets the file path of the image.
		const std::string& GetFilePath() const;

		// returns the maximum side length of the image.
		static int GetMaximumSideLength();

		// gets the width of the image.
		uint32_t GetWidth() const;

		// gets the height of the image.
		uint32_t GetHeight() const;

		// converts a range of pixels to a set of UVs.
		// fromTop determines if the coordinates are based on the top of the image, or the bottom of the image.
		// image files are based on the top left corner, while uvs are based on the bottom left corner.
		static cherry::Vec4 ConvertImagePixelsToUVSpace(cherry::Vec4 pixelArea, float imageWidth, float imageHeight, bool fromTop);

		// returns 'true' if the image is visible from both sides.
		bool IsDoubleSided() const;

		// returns 'true' if the front and back of the image is the same.
		bool HasDuplicatedFront() const;

		// gets the texture sampler
		const cherry::TextureSampler::Sptr const GetTextureSampler() const;

		// sets the alpha value.
		void SetAlpha(float a) override;

		// add update function
		void Update(float deltaTime);

	private:

		// loads the image
		// if size is left as (0, 0), then the size of the image is used instead.
		// the uvs are (0, 0, 1, 1) by default, which takes in the whole coordinate space.
		bool LoadImage(std::string scene, cherry::Vec2 size = cherry::Vec2(0, 0), cherry::Vec4 uvs = cherry::Vec4(0, 0, 1, 1));

		// TODO: possibly change the Object header so that that file path is used instead?
		// the file path for the image
		std::string filePath;

		// the size of the image in pixels.
		glm::u32vec2 dimensions;

		TextureSampler::Sptr sampler;

		// becomes 'true' if the image is double sided.
		bool doubleSided = false;

		// duplicated front.
		bool duplicatedFront = false;

		// camera lock; lodks the image to the camera.
		bool cameraLock = false;

	protected:

	};
}
