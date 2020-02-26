/*
 * Title: Text
 * Description: renders text to the screen
 * References:
    * https://github.com/ubawurinna/freetype-windows-binaries
    * https://learnopengl.com/In-Practice/Text-Rendering
    * https://learnopengl.com/In-Practice/2D-Game/Render-text
    * https://github.com/ubawurinna/freetype-windows-binaries
 */

#pragma once

#define FONT_ARIAL "res/fonts/arial.ttf"

// this is following this tutorial: https://learnopengl.com/In-Practice/Text-Rendering
#include <freetype/ft2build.h>
// #include FT_FREETYPE_H 
//

#include "Object.h"

// TODO: include FreeType
namespace cherry
{
    class Text : public cherry::Object
    {
    public:
        // constructors
        // Text(characters, font file path, position, color, size)
        // if the size is set to 0, then it will be set to 1.
        Text(std::string text, std::string font, cherry::Vec3 position, cherry::Vec4 color, float size);
    
        // Text(characters, font file path, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, size)
        Text(std::string text, std::string font, float x, float y, float z, float r, float g, float b, float a, float size);
        
        // Text(characters, font file path, position, color, size)
        Text(std::string text, std::string font, glm::vec3 position, glm::vec4 color, float size);

        // generates text from a text file.
       //  static cherry::Text GenerateText(std::string filePath, std::string font, cherry::Vec3 pos, cherry::Vec4 color, float size);

        // gets the text as a string
        std::string ToString() const override;

    private:
        // loads the text
        void LoadText();

        // the text
        std::string text = "";

        // the font
        std::string font = "";

          // the text colour
        cherry::Vec4 color;

    protected:

    };
}