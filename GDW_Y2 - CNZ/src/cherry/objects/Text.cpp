#include "Text.h"
#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <toolkit/Logging.h>

#include "..\utils\Utils.h"
#include "..\textures\Texture2D.h"


// constructor
 cherry::Text::Text(std::string text, std::string font, cherry::Vec3 position, cherry::Vec4 color, float size)
 	: Object(), text(text), fontPath(font)
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
 std::string cherry::Text::ToString() const { return text; }
 
 // load text
 void cherry::Text::LoadText()
 {
 	// if the font file is inaccessible, or it is not a ttf file
 	if (!(util::fileAccessible(fontPath) && fontPath.substr(fontPath.find_last_of(".") + 1) == "ttf"))
 	{
 		LOG_ERROR("Font file could not be opened, or was not provided in ttf format. Using default font.");
 		fontPath = FONT_ARIAL; // changes the font to the default.
 	}
 
 	FT_Library ftLib; // free type library
 	if (FT_Init_FreeType(&ftLib))
 		LOG_ERROR("Could not initialize FreeType library.");
 
 	FT_Face ftFace;
 	if (FT_New_Face(ftLib, fontPath.c_str(), 0, &ftFace))
 		std::cout << "Failed to load font." << std::endl;
 	
 	// setting the font size. Width is set to (0) so that it's based on the font height.
 	FT_Set_Pixel_Sizes(ftFace, 0, fontSize);
 
 	// generates a bitmap for the character, and checks if the font is readable.
 	if (FT_Load_Char(ftFace, 'A', FT_LOAD_RENDER))
 		LOG_ERROR("Failed to load glyph from TrueType font file");
 
 	// disables byte-alignment restriction
 	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
 	// goes through the ASCII table
 	// source: http://www.asciitable.com/
 	// 0 - 127 is the standard ASCII table. 128 - 255 is the extended ASCII table.s
 	for (int chr = 0; chr < 128; chr ++)
 	{
 		// loads the character glyph
 		if (FT_Load_Char(ftFace, chr, FT_LOAD_RENDER))
 		{
 			LOG_ERROR("Failed to load ASCII glyph.");
 			continue;
 		}
 
 		// Generate texture
 		GLuint texture;
 		glGenTextures(1, &texture);
 		glBindTexture(GL_TEXTURE_2D, texture);
 		glTexImage2D(
 			GL_TEXTURE_2D,
 			0,
 			GL_RED,
 			ftFace->glyph->bitmap.width,
 			ftFace->glyph->bitmap.rows,
 			0,
 			GL_RED,
 			GL_UNSIGNED_BYTE,
 			ftFace->glyph->bitmap.buffer
 		);
 		
 		// setting the texture options
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
 		glBindTexture(GL_TEXTURE_2D, 0);
 
 		// storing the character for later use.
 		Character character = {
 			texture,
 			glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
 			glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
 			ftFace->glyph->advance.x
 		};
 		chars.push_back(character);
 	}
 
 	// destroying the free type the texture is now loaded.
 	FT_Done_Face(ftFace);
 	FT_Done_FreeType(ftLib);
 
 	// making the vertices for the quad
 	Vertex* verts = new Vertex[4]
 	{
 		//  {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
 		{{ -1.0F, -1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
 		{{  1.0F, -1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
 		{{ -1.0F,  1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
 		{{  1.0F,  1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
 	};
 
 	// making the indices for the quad
 	uint32_t* indis = new uint32_t[6]{
 		0, 1, 2,
 		2, 1, 3
 	};
 
 	mesh = std::make_shared<Mesh>(verts, 4, indis, 6);
 }
