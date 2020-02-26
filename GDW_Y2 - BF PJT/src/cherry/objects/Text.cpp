#include "Text.h"
#include "..\utils\Utils.h"

// constructor
cherry::Text::Text(std::string text, std::string font, cherry::Vec3 position, cherry::Vec4 color, float size)
	: Object(), text(text), font(font)
{
	this->color = Vec4(
		glm::clamp(color.v.x, 0.0F, 1.0F),
		glm::clamp(color.v.y, 0.0F, 1.0F),
		glm::clamp(color.v.z, 0.0F, 1.0F),
		glm::clamp(color.v.w, 0.0F, 1.0F)
	);

	SetPosition(position);

	SetScale(size > 0 ? size : 1);

	// loads the text
	LoadText();
}

// constructor
cherry::Text::Text(std::string text, std::string font, float x, float y, float z, float r, float g, float b, float a, float size)
	: Text(text, font, Vec3(x, y, z), Vec4(r, g, b, a), size)
{
}

// constructor
cherry::Text::Text(std::string text, std::string font, glm::vec3 position, glm::vec4 color, float size)
	: Text(text, font, Vec3(position), Vec4(color), size)
{
}

// toString
std::string cherry::Text::ToString() const
{
	return std::string();
}

// load text
void cherry::Text::LoadText()
{
	// if the font file is inaccessible, or it is not a ttf file
	if (!(util::fileAccessible(font) && font.substr(font.find_last_of(".") + 1) == "ttf"))
		std::runtime_error("The file either could not be found, or was not a ttf file.");
	


}
