#include "Image.h"
#include "..\utils\Utils.h"

// TODO: save scene to a string so that UI can carry over
// creates an iamge by taking in a file path. Images call CreateEntity automatically.
cherry::Image::Image(std::string filePath, std::string scene, bool doubleSided, bool duplicateFront, bool camLock) :
	Image(filePath, scene, Vec2(0, 0), Vec4(0, 0, 1, 1), doubleSided, duplicateFront, camLock)
{

}

// image with size
cherry::Image::Image(std::string filePath, std::string scene, float width, float height, bool doubleSided, bool duplicateFront, bool camLock)
	: Image(filePath, scene, cherry::Vec2(width, height), cherry::Vec4(0.0F, 0.0F, 1.0F, 1.0F), doubleSided, duplicateFront)
{
}

// image with size
cherry::Image::Image(std::string filePath, std::string scene, cherry::Vec2 size, bool doubleSided, bool duplicateFront, bool camLock)
	: Image(filePath, scene, size, cherry::Vec4(0, 0, 1, 1), doubleSided, duplicateFront, camLock)
{
}

// image with uvs.
cherry::Image::Image(std::string filePath, std::string scene, cherry::Vec4 uvs, bool doubleSided, bool duplicateFront, bool camLock)
	: Image(filePath, scene, cherry::Vec2(0.0F, 0.0F), uvs, doubleSided, duplicateFront, camLock)
{
}

// image with size and uvs
cherry::Image::Image(std::string filePath, std::string scene, float width, float height, cherry::Vec4 uvs, bool doubleSided, bool duplicateFront, bool camLock)
	: Image(filePath, scene, cherry::Vec2(width, height), uvs, doubleSided, duplicateFront, camLock)
{
}

// image creation with size and uvs.
cherry::Image::Image(std::string filePath, std::string scene, cherry::Vec2 size, cherry::Vec4 uvs, bool doubleSided, bool duplicateFront, bool camLock)
	: Object(), doubleSided(doubleSided), duplicatedFront(duplicateFront), cameraLock(camLock)
{
	std::ifstream file(filePath, std::ios::in); // opens the file
// file.open(filePath, std::ios::in); // opens file

// file access failure check.
	if (!file)
	{
		safe = false; // file cannot be used

		// TODO: use default image if you can't find it for release mode.
		//#ifndef _DEBUG
		//#endif // !_DEBUG

		throw std::runtime_error("Error opening file. Functions for this object should not be used.");
		return;
	}
	else // if file opening was successful, it is safe to read from.
	{
		safe = true;
	}

	// default: res/images/default.png

	this->filePath = filePath; // saves the file path
	file.close(); // closing the file since the read was successful.

	LoadImage(scene, size, uvs); // loads in the image
}

// destructor
cherry::Image::~Image() { }

// returns the file path for the image.
const std::string& cherry::Image::GetFilePath() const { return filePath; }

// returns the maximum side length of the image.
int cherry::Image::GetMaximumSideLength() { return Texture2D::GetMaximumSideLength(); }

// gets the width
uint32_t cherry::Image::GetWidth() const { return dimensions.x; }

// gets the height
uint32_t cherry::Image::GetHeight() const { return dimensions.y; }

// converts a range of pixels to uvs.
cherry::Vec4 cherry::Image::ConvertImagePixelsToUVSpace(cherry::Vec4 pixelArea, float imageWidth, float imageHeight, bool fromTop)
{
	cherry::Vec4 temp;
	// not working for some reason.
	if (fromTop) // based on the top of the image
	{
		temp = Vec4(
			pixelArea.v.x / imageWidth,
			1.0F - ((imageHeight - pixelArea.v.y) / imageHeight),
			pixelArea.v.z / imageWidth,
			1.0F - ((imageHeight - pixelArea.v.w) / imageHeight)
		);
	}
	else // based on the bottom of the image.
	{
		temp = Vec4(
			pixelArea.v.x / imageWidth,
			pixelArea.v.y / imageHeight,
			pixelArea.v.z / imageWidth,
			pixelArea.v.w / imageHeight
		);
	}

	return temp;
}

// returns 'true' if the image is visible on both sides.
bool cherry::Image::IsDoubleSided() const { return doubleSided; }

// returns true if the front and the back of the image is the same.
bool cherry::Image::HasDuplicatedFront() const { return duplicatedFront; }

// returns the sampler.
const cherry::TextureSampler::Sptr const cherry::Image::GetTextureSampler() const
{
	return sampler;
}

// sets the alpha value for the image.
void cherry::Image::SetAlpha(float a)
{
	alpha = (a < 0.0F) ? 0.0F : (a > 1.0F) ? 1.0F : a;

	// if the image doesn't have a 100% alpha value, then it won't need to be sorted for proper transparency.
	// however, if the image inherently has transparency (i.e. if it's a png), then transparency is left on.
	if (alpha < 1.0F || util::equalsIgnoreCase(filePath.substr(filePath.find_last_of(".") + 1), "png"))
		material->HasTransparency = true;
	else
		material->HasTransparency = false;

	material->Set("a_Alpha", alpha);
}

// loads an image
bool cherry::Image::LoadImage(std::string scene, cherry::Vec2 size, cherry::Vec4 uvs)
{
	// gets the iamge
	// NOTE: if the image is too large, the process will fail.
	Texture2D::Sptr img = Texture2D::LoadFromFile(filePath);

	// the four uvs
	glm::vec2 uvBL(uvs.v.x, uvs.v.y); // (0, 0)
	glm::vec2 uvBR(uvs.v.z, uvs.v.y); // (1, 0)
	glm::vec2 uvTL(uvs.v.x, uvs.v.w); // (0, 1)
	glm::vec2 uvTR(uvs.v.z, uvs.v.w); // (1, 1)

	// TODO: make a shader that doesn't use lighting but has textures?
	// mapping the image to the plane
	Shader::Sptr shader;
	Material::Sptr material;
	SamplerDesc description; // texture description
	// TextureSampler::Sptr sampler; // now global variable
	
	// saving the width and height for the image. If the provided size is NOT (0, 0), then that is used instead.
	// also checks to see if the cropped space is within the boundsof the image.
	if (size.v == Vec2(0, 0).v && 
		(uvs.v.x != 0 || uvs.v.y != 0 || uvs.v.z != 1 || uvs.v.w != 1))
	{
		// if the whole image isn't being used, then the uvs are used to make the image size. 
		dimensions.x = img->GetWidth() * uvs.v.z - img->GetWidth() * uvs.v.x;
		dimensions.y = img->GetHeight() * uvs.v.w - img->GetHeight() * uvs.v.y;
	}
	else // whole image is to be used.
	{
		dimensions = glm::u32vec2(img->GetWidth(), img->GetHeight());
	}
	
	// image is too large to be loaded.
	if(dimensions.x > GetMaximumSideLength() || dimensions.y > GetMaximumSideLength())
	{
		std::cout << "File too large to be read." << std::endl;
		safe = false;
	}

	if(duplicatedFront) // the front and back are the same
	{ 
		// creates a cube that gets squished so that it appears to be a plane.
		// the front and back of the cube (which become the front and back of the plane)
		verticesTotal = 8;

		// Position, Colour, Normals, and UVs
		vertices = new Vertex[verticesTotal]
		{
			//  x			  y				  z		   r	 g	   b	 a		 // normals
			{{ -(float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvBL}}, // default: (0, 0) ~ bottom left
			{{  (float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvBR}}, // default: (1, 0) ~ bottom right
			{{ -(float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvTL}}, // default: (0, 1) ~ top left
			{{  (float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvTR}}, // default: (1, 1) ~ top right

			// replication of what's shown above.
			{{ -(float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, -1.0F}, {uvBR}}, // default: (1, 0)
			{{  (float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, -1.0F}, {uvBL}}, // default: (0, 0)
			{{ -(float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, -1.0F}, {uvTR}}, // default: (1, 1)
			{{  (float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, -1.0F}, {uvTL}}, // default: (0, 1)
		};

		// indices
		indicesTotal = 36;
		indices = new uint32_t[indicesTotal]{
			0, 1, 2,
			2, 1, 3,
			0, 2, 4,
			2, 6, 4,
			1, 5, 3,
			3, 5, 7,
			4, 6, 5,
			6, 7, 5,
			0, 4, 5,
			5, 1, 0,
			2, 7, 6,
			7, 2, 3
		};
	}
	else // the front and back are different
	{
		verticesTotal = 4;

		// Position, Colour, Normals, and UVs
		vertices = new Vertex[verticesTotal]
		{
			//  x			  y				  z		   r	 g	   b	 a		 // normals
			{{ -(float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvBL}}, // default: (0, 0) ~ bottom left
			{{  (float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvBR}}, // default: (1, 0) ~ bottom right
			{{ -(float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvTL}}, // default: (0, 1) ~ top left
			{{  (float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {uvTR}}, // default: (1, 1) ~ top right
		};

		// indices
		indicesTotal = 6;
		indices = new uint32_t[indicesTotal]{
			0, 1, 2,
			2, 1, 3
		};
	}
	

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	
	if (duplicatedFront) // if the front and back are the same, then both should be shown.
	{
		mesh->cullFaces = true; // hide the backfaces.
	}
	else // front shouldn't be duplicated, meaning that the backfaces may not be shown.
	{
		mesh->cullFaces = !doubleSided; // images do not have their faces culled. // TODO: add ability to have the image be the same on both sides
	}
	
	
	// MAPPING THE TEXTURE
	// texture description and sampler
	description = SamplerDesc();
	description.MinFilter = MinFilter::LinearMipNearest;
	description.MagFilter = MagFilter::Linear;

	sampler = std::make_shared<TextureSampler>(description);


	shader = std::make_shared<Shader>();
	
	// no lighting is applied.
	// TODO: remove this?
	if (cameraLock)
	{
		shader->Load("res/shaders/image.vs.glsl", "res/shaders/image.fs.glsl");

		material = std::make_shared<Material>(shader);

		material->Set("s_Albedos[0]", img, sampler);
		material->Set("s_Albedos[1]", img, sampler);
		material->Set("s_Albedos[2]", img, sampler);
	}
	else
	{
		shader->Load("res/shaders/image.vs.glsl", "res/shaders/image.fs.glsl");

		material = std::make_shared<Material>(shader);

		material->Set("s_Albedos[0]", img, sampler);
		material->Set("s_Albedos[1]", img, sampler);
		material->Set("s_Albedos[2]", img, sampler);
	}


	// TODO: should probably do error checking, but this is fine for now.
	if (util::equalsIgnoreCase(filePath.substr(filePath.find_last_of(".") + 1), "png"))
	{
		material->HasTransparency = true; // there should be transparency if it's a png
	}

	// TODO: fix this. The image is travelling in the wrong direction.
	// images are drawn upside down, so they are rotated to be put rightside up.
	SetRotationZDegrees(180.0F);

	// creates the entity for the image
	CreateEntity(scene, material);
	return true;
}

// update
void cherry::Image::Update(float deltaTime)
{
	// SetRotationDegrees(GetRotationDegrees() + Vec3(30.0F * deltaTime, 0, 0));
	Object::Update(deltaTime);
}