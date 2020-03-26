#include "Text.h"
#include "..\utils/Utils.h"
#include "..\utils/math/Rotation.h"

const int cherry::Text::CHAR_COUNT = 256;

// constructor
cherry::Text::Text(std::string text, std::string scene, std::string font, cherry::Vec4 color, float size)
	: Object(), text(text), filePath(font), color(color), fontSize(size)
{
	this->color = Vec4(
		glm::clamp(color.v.x, 0.0F, 1.0F),
		glm::clamp(color.v.y, 0.0F, 1.0F),
		glm::clamp(color.v.z, 0.0F, 1.0F),
		glm::clamp(color.v.w, 0.0F, 1.0F)
		);

	SetPosition(position);

	// saving the font size.
	fontSize = size > 0 ? size : 1;

	// loads the text
	LoadText(scene);
}


// constructor
cherry::Text::Text(std::string text, std::string scene, std::string font, glm::vec4 color, float size)
	: Text(text, scene, font, Vec4(color), size)
{
}

// constructor with individual colour values rather than a vector
cherry::Text::Text(std::string text, std::string scene, std::string font, float r, float g, float b, float a, float size)
	: Text(text, scene, font, Vec4(r, g, b, a), size)
{
}

// constructor with [0, 255] colour range for individual colour values.
cherry::Text::Text(std::string text, std::string scene, std::string font, int r, int g, int b, float a, float size)
	: Text(text, scene, font, Vec4((float)r / 255.0F, (float)g / 255.0F, (float)b / 255.0F, a), size)
{
}

// copy constructor
cherry::Text::Text(const cherry::Text& txt)
	: Object(txt)
{
	text = txt.text;
	filePath = txt.filePath; // text path
	fontMap = txt.fontMap; // font
	color = txt.color; // colour

	windowChild = txt.windowChild;
	sceneName = txt.sceneName;

}

// destructor.
cherry::Text::~Text()
{
	// array of characters. This doesn't get deleted since they're shared pointers.
	// for (Character* ch : chars) // shared pointers.
	//     delete ch;

	// deleting the text characters.
	for (Character* ch : textChars)
		delete ch;
}

// the text is visible.
bool cherry::Text::IsVisible() const { return visible; }

// visibility toggle
void cherry::Text::SetVisible() { SetVisible(!visible); }

// sets text visiblity.
void cherry::Text::SetVisible(bool visible)
{
	// makes all characters visible.
	this->visible = visible;

	for (Character* chr : textChars)
		chr->SetVisible(visible);
}

// returns the text.
std::string cherry::Text::GetText() const { return text; }

// sets the text
void cherry::Text::SetText(const std::string newText)
{
	// clears out all the text.
	ClearText();

	text = newText;

	// float alpha = GetAlpha();

	// creating the new characters
	for (int i = 0; i < text.size(); i++)
	{
		const char c = text[i];

		// gets the character.
		const Character* charObject = chars[(int)c].get();
		// const Character& cpy = charObject->get();

		// making a copy of the character.
		Character* charCopy = new Character(*charObject);

		charCopy->localPosition = Vec3(spacing * fontSize * i, 0, 0);
		charCopy->SetPosition(charCopy->localPosition);
		charCopy->SetScale(0.1F);
		charCopy->SetVisible(visible);

		// for some reason, the material's transparency is turned off at some point.
		// this turns it back on.
		charCopy->GetMaterial()->HasTransparency = true;
		charCopy->GetMesh()->SetWindowChild(windowChild);
		// charCopy->SetAlpha(alpha);

		// charCopy->SetRotationZDegrees(180.0F);
		textChars.push_back(charCopy);
	}

	// calculations transformations.
	CalculateTextTransform();
}

// clears all characters
void cherry::Text::ClearText()
{
	// clears all characters from the text
	for (Character* chr : textChars)
		delete chr;

	textChars.clear();

	// no text
	text = "";
}

// set a new colour for the text
void cherry::Text::SetColor(cherry::Vec4 newColor)
{
	// gets the new color
	color = glm::vec4(newColor.v.x, newColor.v.y, newColor.v.z, newColor.v.w);

	// char material
	if (knownCharMaterial != nullptr)
		knownCharMaterial->Set("a_Color", { color.v.x, color.v.y, color.v.z, color.v.w });

	// unknwon char material
	if (unknownCharMaterial != nullptr)
		unknownCharMaterial->Set("a_Color", { color.v.x, color.v.y, color.v.z, color.v.w });
}

// gets the file path for the font.
const std::string& cherry::Text::GetFilePath() const { return filePath; }

// gets the name of the font being used.
std::string cherry::Text::GetFontName() const { return name; }

// gets the font file path.
std::string cherry::Text::GetFontMapFilePath() const { return fontMap; }

// gets the cell size.
glm::vec2 cherry::Text::GetCellSize() const { return cellSize; }

// gets the font size.
int cherry::Text::GetFontSize() const { return fontSize; }

// gets the font spacing.
float cherry::Text::GetSpacing() const { return spacing; }

// load text
void cherry::Text::LoadText(const std::string scene)
{
	// shader and material for the text box.
	cherry::Shader::Sptr textShader;
	cherry::Material::Sptr textMaterial; // material

	// shader for characters
	cherry::Shader::Sptr charShader;
	cherry::Material::Sptr charMaterial;

	// used when thre is no 
	cherry::Material::Sptr noCharMaterial;

	// the font map texture.
	Texture2D::Sptr mapTexture;
	glm::vec2 mapSize(1.0F, 1.0F); // the image dimensions

	std::fstream file; // file
	std::string line;

	// saves the scene name
	sceneName = scene;

	// character shader.
	charShader = std::make_shared<Shader>();
	charShader->Load("res/shaders/text.vs.glsl", "res/shaders/text.fs.glsl");

	// character material
	charMaterial = std::make_shared<Material>(charShader);
	charMaterial->Set("a_Color", { color.v.x, color.v.y, color.v.z, color.v.w });
	charMaterial->HasTransparency = true;

	// unaddressed character.
	noCharMaterial = std::make_shared<Material>(charShader);
	noCharMaterial->Set("a_Color", { color.v.x, color.v.y, color.v.z, color.v.w });
	noCharMaterial->Set("a_FontMap", Texture2D::LoadFromFile("res/fonts/default_char.png"));
	noCharMaterial->HasTransparency = true;


	// TODO: file safety check.
	file.open(filePath, std::ios::in);

	// getting all the lines
	while (std::getline(file, line))
	{
		std::string temp = "";
		std::vector<std::string> splitLine;

		// no information
		if (line.empty())
			continue;

		// temp = line.substr(0, line.find_first_of(" "));
		splitLine = util::splitString<std::string>(line);

		if (splitLine.size() <= 1)
			continue;

		temp = splitLine[0];

		// reading the text.
		if (temp == "fnt") // font name
		{
			name = splitLine[1];
			name = util::replaceSubstring(name, "_", " ", false);
		}
		else if (temp == "src") // source
		{
			description = splitLine[1];
			description = util::replaceSubstring(name, "_", " ", false);

		}
		else if (temp == "sze") // size of each cell.
		{
			cellSize = glm::vec2(
				util::convertString<float>(splitLine[1]),
				util::convertString<float>(splitLine[2])
				);
		}
		else if (temp == "spc") // spacing
		{
			spacing = util::convertString<float>(splitLine[1]);

		}
		else if (temp == "map") // file path for the image.
		{
			fontMap = splitLine[1];
			std::string fpStr = (filePath.find("/") != std::string::npos) ? filePath.substr(0, filePath.find_last_of("/") + 1) : "";
			fontMap = fpStr + fontMap;

			// loads in the map
			mapTexture = Texture2D::LoadFromFile(fontMap);
			mapSize = glm::vec2{ mapTexture->GetWidth(), mapTexture->GetHeight() }; // saves the image dimensions.

			charMaterial->Set("a_FontMap", mapTexture);

			// TODO: add this.
		}
		else if (util::isInt(temp)) // if the string is an integer.
		{
			int index = util::convertString<int>(splitLine[0]); // gets the character.

			// Number, Symbol, PosX, PosY, Width, Height 
			glm::vec4 uvs;
			uvs.x = util::convertString<int>(splitLine[2]);
			uvs.y = util::convertString<int>(splitLine[3]);
			uvs.z = uvs.x + util::convertString<int>(splitLine[4]);
			uvs.w = uvs.y + util::convertString<int>(splitLine[5]);

			// 0 through 1 range.
			uvs.x /= mapSize.x;
			uvs.y /= mapSize.y;
			uvs.z /= mapSize.x;
			uvs.w /= mapSize.y;

			// if there are no uvs, then the default character is used.
			if (uvs != glm::vec4(0, 0, 0, 0))
			{
				chars[index] = std::make_shared<Character>((char)index, scene, charMaterial, cellSize * fontSize, uvs);
			}
			else
			{
				chars[index] = std::make_shared<Character>((char)index, scene, noCharMaterial, cellSize * fontSize, glm::vec4(0, 0, 1, 1));
			}

			chars[index]->SetVisible(false);
		}
	}

	file.close();

	// creating the characters
	// TODO: multiple lines.

	// creates all the characters.
	SetText(text);

	knownCharMaterial = charMaterial;
	unknownCharMaterial = noCharMaterial;

	// plane representing the text box.
	verticesTotal = 4;
	vertices = new Vertex[verticesTotal]
	{
		//  {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
		{{ -1.0F / 2.0F, -1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
		{{  1.0F / 2.0F, -1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
		{{ -1.0F / 2.0F,  1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
		{{  1.0F / 2.0F,  1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
	};

	// making the indices for the quad
	indicesTotal = 6;
	uint32_t* indis = new uint32_t[indicesTotal]{
		0, 1, 2,
		2, 1, 3
	};

	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

	textShader = std::make_shared<Shader>();
	textShader->Load("res/shaders/shader.vs.glsl", "res/shaders/shader.fs.glsl");

	textMaterial = std::make_shared<Material>(textShader);
	textMaterial->HasTransparency = true;

	// creating a plane to represent the size.
	// CreateEntity(scene, textMaterial);
	CreateEntity(scene, textMaterial); // empty material
	mesh->SetVisible(false); // either use the parent's function, or call the mesh directly.

	// default orientation
	SetRotationDegrees(GetRotationDegrees() + Vec3(0.0F, 0.0F, 180.0F));

	worldPos = position + Vec3(1, 1, 1);
	worldScale = scale;
	worldRotDeg = GetRotationDegrees();
}

// calculates the text's scale
void cherry::Text::CalculateTextScale()
{
	// the scale of the text.
	Vec3 textScale = GetScale();

	// setting the scale proportional to the body.
	for (Character* chr : textChars)
	{
		Vec3 charScale = chr->GetScale(); // character's scale
		Vec3 newScale; // new scale

		// calculating the new scale.
		newScale.v.x = charScale.v.x * textScale.v.x;
		newScale.v.y = charScale.v.y * textScale.v.y;
		newScale.v.z = charScale.v.z * textScale.v.z;

		// setting the new scale.
		chr->SetScale(newScale);
	}

	// saving scale
	worldScale = scale;
}

// calculates text rotation.
void cherry::Text::CalculateTextRotation()
{
	// current text rotation
	Vec3 currRotDeg = GetRotationDegrees();

	// rotation
	for (Character* chr : textChars)
		chr->SetRotationDegrees(currRotDeg + chr->GetRotationDegrees());

	// saving rotation
	worldRotDeg = currRotDeg;
}

// calculates text position
void cherry::Text::CalculateTextPosition()
{
	// the text box is the parent.
	glm::mat4 parent = glm::mat4(1.0F);

	// the resulting matrix.
	glm::mat4 result = glm::mat4(1.0F);

	// rotation and scale
	util::math::Mat3 rotScale{
		1.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 1.0F
	};

	// scale
	util::math::Mat3 scale = rotScale;

	// rotations
	util::math::Mat3 rotX = rotScale;
	util::math::Mat3 rotY = rotScale;
	util::math::Mat3 rotZ = rotScale;

	// translation
	parent[0][3] = position.v.x;
	parent[1][3] = position.v.y;
	parent[2][3] = position.v.z;
	parent[3][3] = 1.0F;

	// rotation
	rotX = util::math::getRotationMatrixX(GetRotationXDegrees(), true);
	rotY = util::math::getRotationMatrixY(GetRotationYDegrees(), true);
	rotZ = util::math::getRotationMatrixZ(GetRotationZDegrees(), true);

	// scale
	scale[0][0] = Object::scale.v.x;
	scale[1][1] = Object::scale.v.y;
	scale[2][2] = Object::scale.v.z;

	// rotation and scale.
	rotScale = scale * (rotZ * rotX * rotY);

	// saving the rotation and scale transformations.
	parent[0][0] = rotScale[0][0];
	parent[0][1] = rotScale[0][1];
	parent[0][2] = rotScale[0][2];

	parent[1][0] = rotScale[1][0];
	parent[1][1] = rotScale[1][1];
	parent[1][2] = rotScale[1][2];

	parent[2][0] = rotScale[2][0];
	parent[2][1] = rotScale[2][1];
	parent[2][2] = rotScale[2][2];

	// updates all characters.
	for (Character* chr : textChars)
	{
		Vec3 chrPos = chr->GetLocalPosition();

		// gets the position of the character.
		glm::mat4 child
		{
			chrPos.v.x, 0, 0, 0,
			chrPos.v.y, 0, 0, 0,
			chrPos.v.z, 0, 0, 0,
			0, 0, 0, 0
		};

		result = parent * child;

		chr->SetPosition(result[0][0], result[1][0], result[2][0]);
	}

	worldPos = position;
}

// calculates the text transformation
void cherry::Text::CalculateTextTransform()
{
	// calls all transform functions.
	CalculateTextScale();
	CalculateTextRotation();
	CalculateTextPosition();

	// // current rotation (degrees)
	// Vec3 currRotDeg = GetRotationDegrees();
	// 
	// // Scale
	// {
	// 	// the scale of the text.
	// 	Vec3 textScale = GetScale();
	// 
	// 	// setting the scale proportional to the body.
	// 	for (Character* chr : textChars)
	// 	{
	// 		Vec3 charScale = chr->GetScale(); // character's scale
	// 		Vec3 newScale; // new scale
	// 
	// 		// calculating the new scale.
	// 		newScale.v.x = charScale.v.x * textScale.v.x;
	// 		newScale.v.y = charScale.v.y * textScale.v.y;
	// 		newScale.v.z = charScale.v.z * textScale.v.z;
	// 
	// 		// setting the new scale.
	// 		chr->SetScale(newScale);
	// 	}
	// }
	// 
	// // Rotation
	// {
	// 	// rotation
	// 	for (Character* chr : textChars)
	// 		chr->SetRotationDegrees(GetRotationDegrees() + chr->GetRotationDegrees());
	// 
	// }
	// 
	// // Position
	// {
	// 	// the text box is the parent.
	// 	glm::mat4 parent = glm::mat4(1.0F);
	// 
	// 	// the resulting matrix.
	// 	glm::mat4 result = glm::mat4(1.0F);
	// 
	// 	// rotation and scale
	// 	util::math::Mat3 rotScale{
	// 		1.0F, 0.0F, 0.0F,
	// 		0.0F, 1.0F, 0.0F,
	// 		0.0F, 0.0F, 1.0F
	// 	};
	// 
	// 	// scale
	// 	util::math::Mat3 scale = rotScale;
	// 
	// 	// rotations
	// 	util::math::Mat3 rotX = rotScale;
	// 	util::math::Mat3 rotY = rotScale;
	// 	util::math::Mat3 rotZ = rotScale;
	// 
	// 	// translation
	// 	parent[0][3] = position.v.x;
	// 	parent[1][3] = position.v.y;
	// 	parent[2][3] = position.v.z;
	// 	parent[3][3] = 1.0F;
	// 
	// 	// rotation
	// 	rotX = util::math::getRotationMatrixX(GetRotationXDegrees(), true);
	// 	rotY = util::math::getRotationMatrixY(GetRotationYDegrees(), true);
	// 	rotZ = util::math::getRotationMatrixZ(GetRotationZDegrees(), true);
	// 
	// 	// scale
	// 	scale[0][0] = Object::scale.v.x;
	// 	scale[1][1] = Object::scale.v.y;
	// 	scale[2][2] = Object::scale.v.z;
	// 
	// 	// rotation and scale.
	// 	rotScale = scale * (rotZ * rotX * rotY);
	// 
	// 	// saving the rotation and scale transformations.
	// 	parent[0][0] = rotScale[0][0];
	// 	parent[0][1] = rotScale[0][1];
	// 	parent[0][2] = rotScale[0][2];
	// 
	// 	parent[1][0] = rotScale[1][0];
	// 	parent[1][1] = rotScale[1][1];
	// 	parent[1][2] = rotScale[1][2];
	// 
	// 	parent[2][0] = rotScale[2][0];
	// 	parent[2][1] = rotScale[2][1];
	// 	parent[2][2] = rotScale[2][2];
	// 
	// 	// updates all characters.
	// 	for (Character* chr : textChars)
	// 	{
	// 		Vec3 chrPos = chr->GetLocalPosition();
	// 
	// 		// gets the position of the character.
	// 		glm::mat4 child
	// 		{
	// 			chrPos.v.x, 0, 0, 0,
	// 			chrPos.v.y, 0, 0, 0,
	// 			chrPos.v.z, 0, 0, 0,
	// 			0, 0, 0, 0
	// 		};
	// 
	// 		result = parent * child;
	// 
	// 		chr->SetPosition(result[0][0], result[1][0], result[2][0]);
	// 	}
	// }
	// 
	// // saving the values.
	// worldPos = position;
	// worldScale = scale;
	// worldRotDeg = currRotDeg;
}

// update time
void cherry::Text::Update(float deltaTime)
{
	// changing scenes
	if (sceneName != GetSceneName())
	{
		sceneName = GetSceneName();
		for (Character* chr : textChars)
			chr->SetScene(sceneName);
	}

	// if the window status of the child has been changed.
	if (IsWindowChild() != windowChild)
	{
		windowChild = IsWindowChild();

		for (Character* chr : textChars)
			chr->GetMesh()->SetWindowChild(windowChild);
	}

	// checks to see what values need to be updated.
	bool updateScale = worldScale != scale;
	bool updateRotation = worldRotDeg != GetRotationDegrees();
	bool updatePosition = worldPos != position;

	// checking if a transformation is needed.
	if (updateScale || updateRotation || updatePosition)
	{
		// scale has changed
		if (updateScale)
			CalculateTextScale();

		// rotation has changed
		if (updateRotation)
			CalculateTextRotation();

		CalculateTextPosition();
	}

	// update loop.
	for (Character* chr : textChars)
	{
		chr->Update(deltaTime);
	}

	// update's text
	Object::Update(deltaTime);
}

// toString
std::string cherry::Text::ToString() const { return text; }


// CHARACTERS
cherry::Character::Character(const char a_CHAR, std::string scene, cherry::Material::Sptr fontMap, glm::vec2 size, glm::vec4 uvs)
	: m_CHAR(a_CHAR), uvs(uvs)
{
	// making the mesh
	verticesTotal = 4;
	vertices = new Vertex[verticesTotal]
	{
		//  {x, y, z}, {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
	   {{ -size.x / 2.0F, -size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {uvs.x, uvs.y}}, // bottom left (uv = 0,0)
	   {{  size.x / 2.0F, -size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {uvs.z, uvs.y}}, // bottom right (uv = 1,0)
	   {{ -size.x / 2.0F,  size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {uvs.x, uvs.w}}, // top left (uv = 0,1)
	   {{  size.x / 2.0F,  size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 0.3F, 1.0F }, {0.0F, 0.0F, 1.0F}, {uvs.z, uvs.w}}, // top right (uv = 1,1)
	};

	indicesTotal = 6;
	// making the indices for the quad
	indices = new uint32_t[indicesTotal]{
		0, 1, 2,
		2, 1, 3
	};

	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

	SetRotationZDegrees(180.0F);

	// fontMap->Set("a_Text", );
	CreateEntity(scene, fontMap);
}

// copy constructor
cherry::Character::Character(const Character& cpy)
	: Object(cpy), m_CHAR(cpy.m_CHAR)
{
	// copies the character.
	localPosition = cpy.GetLocalPositionGLM();
	uvs = cpy.uvs;
}

// gets the local position
cherry::Vec3 cherry::Character::GetLocalPosition() const
{
	return localPosition;
}

// gets the local position.
glm::vec3 cherry::Character::GetLocalPositionGLM() const
{
	return glm::vec3(localPosition.v.x, localPosition.v.y, localPosition.v.z);
}

// update
void cherry::Character::Update(float deltaTime)
{
	Object::Update(deltaTime);
}
