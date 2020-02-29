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

#define FONT_ARIAL ("res/fonts/arial.ttf")

// this is following this tutorial: https://learnopengl.com/In-Practice/Text-Rendering

#include "Object.h"
#include <string>

// TODO: include FreeType
 namespace cherry
 {
     class Character;

     class Text : public cherry::Object
     {
     public:
         // constructors
         // Text(characters, font file path, position, color, size)
         // if the size is set to 0, then it will be set to 1.
         Text(std::string text, std::string scene, std::string font, cherry::Vec4 color, float size);
     
         // Text(characters, font file path, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, size)
         Text(std::string text, std::string scene, std::string font, float r, float g, float b, float a, float size);
         
         // Text(characters, font file path, position, color, size)
         Text(std::string text, std::string scene, std::string font, glm::vec4 color, float size);
 

         // generates text from a text file.
        //  static cherry::Text GenerateText(std::string filePath, std::string font, cherry::Vec3 pos, cherry::Vec4 color, float size);
 
         // update time.
         void Update(float deltaTime);

         // gets the text as a string
         std::string ToString() const override;
 
     private:
         // loads the text
         void LoadText(const std::string scene);
 
         std::string text = ""; // text
         std::string fontPath = ""; // font file path
         cherry::Vec4 color; // colour
         
         // the font size
         float fontSize = 1;

         // holds the information for a given glyph.
         struct Glyph
         {
             GLchar symbol;
             GLuint textureID; // ID handle of the glyph texture
             glm::ivec2 size; // glyph size
             glm::ivec2 bearing; // offset fom baseline to left/top of glyph
             GLuint advance; // offset to advance to the next glyph
         };
 
         // vector of glyphs
         std::vector<Glyph> glyphs;

         // the characters.
         std::vector<cherry::Character *> chars;
 
     protected:
 
     };

     // the class the text uses to load an individual symbol.
     // this is meant for the text class, and should not be used by anything else.
     class Character : public cherry::Object
     {
     public:
         // constructor
         // Character(const char a_char);
         
         // constructor
         // a_CHAR: the character this object is.
         // textureID: the texture ID.
         // size: the size of the character.
         Character(const char a_CHAR, GLuint textureID, glm::vec2 size);

         // void SetColor(cherry::Vec4 color);

         // text position.
         glm::vec2 textPos{};

     private:
         const char m_CHAR; // the character.

     protected:

     };
 }