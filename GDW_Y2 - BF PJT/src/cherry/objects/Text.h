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

#define FONT_ARIAL ("res/fonts/arial_narrow_7.txt")

// TODO: add ability to not cull faces for text.
// TODO: add ability to reflect text on both sides.
// this is following this tutorial: https://learnopengl.com/In-Practice/Text-Rendering

#include "Object.h"
#include "..\UtilsCRY.h"

#include <string>
#include <queue>

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
         Text(std::string text, std::string scene, std::string font, cherry::Vec4 color, float size = 1.0F);
     
         // Text(characters, font file path, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, size)
         Text(std::string text, std::string scene, std::string font, float r, float g, float b, float a, float size = 1.0F);
         
         // Text(characters, font file path, position, color, size)
         Text(std::string text, std::string scene, std::string font, glm::vec4 color, float size = 1.0F);
 
         // copy constructor.
         Text(const cherry::Text&);

         // destructor
         virtual ~Text();

         // gets the text.
         std::string GetText() const;

         // sets a new color.
         void SetColor(cherry::Vec4 newColor);

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
         std::string filePath = ""; // text path
         std::string fontMap = ""; // font
         cherry::Vec4 color; // colour
         
         float spacing = 0;

         // the font size
         float fontSize = 1;
         glm::vec2 cellSize{};

         static const int CHAR_COUNT; // 256
         
         // the characters (0 - 255)
         std::shared_ptr<Character> chars[256];
 
         // the characters of the text.
         std::vector<Character*> textChars;

         // the characters for the text.
         // std::queue<Character *>textChars;

         // the world position, scale, and rotation.
         // the characters need to be updated if there are changes.
         Vec3 worldPos, worldScale, worldRotDeg;
     protected:
 
     };

     // standard ASCII character.
     class Character : public cherry::Object
     {
     public:

         // creates a character from the font map
         // character, scene, font map, size(width, height), uvs(u.min, v.min, u.max, v.max)
         // this does not save what font the character is.
         Character(const char a_CHAR, std::string scene, cherry::Material::Sptr fontMap, glm::vec2 size, glm::vec4 uvs);

         // copy constructor.
         Character(const Character&);

         // gets the local position.
         cherry::Vec3 GetLocalPosition() const;

         // gets the local position
         glm::vec3 GetLocalPositionGLM() const;

         // update
         void Update(float deltaTime) override;

         // local position.
         cherry::Vec3 localPosition;

         const char m_CHAR; // the character.
     private:
        
         glm::vec3 uvs;
         // static Character * noChar;

     protected:

     };

     // wide character
     // class WideCharacter : public cherry::Char 
     // {
     // 
     // };
 }