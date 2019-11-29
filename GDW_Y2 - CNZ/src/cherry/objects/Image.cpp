#include "Image.h"
#include "..\utils\Utils.h"

// TODO: save scene to a string so that UI can carry over
// creates an iamge by taking in a file path.
cherry::Image::Image(std::string filePath, std::string scene) : Object()
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
	
	LoadImage(scene); // loads in the image
}

// destructor
cherry::Image::~Image() { delete[] indices; }

// gets the width
uint32_t cherry::Image::GetWidth() const { return dimensions.x; }

// gets the height
uint32_t cherry::Image::GetHeight() const { return dimensions.y; }

// loads an image
bool cherry::Image::LoadImage(std::string scene)
{
	// gets the iamge
	Texture2D::Sptr img = Texture2D::LoadFromFile(filePath);
	
	// TODO: make a shader that doesn't use lighting but has textures?
	// mapping the image to the plane
	Shader::Sptr shader;
	Material::Sptr material;
	SamplerDesc description; // texture description
	TextureSampler::Sptr sampler;
	
	// saving the width and height
	dimensions.x = img->GetWidth();
	dimensions.y = img->GetHeight();

	
	// glm::float32_t; // glm float

	// Position, Colour, Normals, and UVs
	verticesTotal = 4;
	vertices = new Vertex[verticesTotal]
	{
		//  x			  y				  z		   r	 g	   b	 a		 // normals
		{{ -(float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
		{{  (float)(dimensions.x) / 2.0F, -(float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
		{{ -(float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
		{{  (float)(dimensions.x) / 2.0F,  (float)(dimensions.y) / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
	};

	// indices
	indicesTotal = 6;
	indices = new uint32_t[indicesTotal]{
		0, 1, 2,
		2, 1, 3
	};

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	

	// MAPPING THE TEXTURE
	// texture description and sampler
	description = SamplerDesc();
	description.MinFilter = MinFilter::LinearMipNearest;
	description.MagFilter = MagFilter::Linear;

	sampler = std::make_shared<TextureSampler>(description);


	shader = std::make_shared<Shader>();
	// TODO: probably shouldn't use lighting shader since it's an image
	shader->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl");

	// lighting has no strong effect on images currnetly
	material = std::make_shared<Material>(shader);
	material->Set("a_LightPos", { 0, 0, 0 });
	material->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
	material->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	material->Set("a_AmbientPower", 1.0f); // change this to change the main lighting power (originally value of 0.1F)
	material->Set("a_LightSpecPower", 0.0f);
	material->Set("a_LightShininess", 0.0f); // MUST be a float
	material->Set("a_LightAttenuation", 1.0f);
	
	material->Set("s_Albedos[0]", Texture2D::LoadFromFile(filePath), sampler);
	material->Set("s_Albedos[1]", Texture2D::LoadFromFile(filePath), sampler);
	material->Set("s_Albedos[2]", Texture2D::LoadFromFile(filePath), sampler);



	// TODO: should probably do error checking, but this is fine for now.
	if (util::equalsIgnoreCase(filePath.substr(filePath.find_last_of(".") + 1), "png"))
	{
		material->HasTransparency = true; // there should be transparency if it's a png
	}

	// creates the entity for the image
	CreateEntity(scene, material);
	return true;
}
