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
         // [0, 1] colour range
         // if the size is set to 0, then it will be set to 1.
         // the font size cannot be changed, but the text scale can.
         Text(std::string text, std::string scene, std::string font, cherry::Vec4 color, float size = 1.0F);
     
         // Text(characters, font file path, position, color, size)
         // [0, 1] colour range
         // the font size cannot be changed, but the text scale can.
         Text(std::string text, std::string scene, std::string font, glm::vec4 color, float size = 1.0F);

         // Text(characters, font file path, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, size)
         // [0, 1] colour range
         // the font size cannot be changed, but the text scale can.
         Text(std::string text, std::string scene, std::string font, float r, float g, float b, float a, float size = 1.0F);
        
         // Text(characters, font file path, position, color.r, color.g, color.b, color.a, size)
         // [0, 255] colour range
         // if the size is set to 0, then it will be set to 1.
         // the font size cannot be changed, but the text scale can.
         Text(std::string text, std::string scene, std::string font, int r, int g, int b, float a, float size = 1.0F);

         // copy constructor.
         Text(const cherry::Text&);

         // destructor
         virtual ~Text();

         // returns whether or not the object is visible
         bool IsVisible() const override;

         // toggle's visibility on/off
         void SetVisible() override;

         // sets whether the object is visible or not.
         void SetVisible(bool visible) override;

         // gets the text.
         std::string GetText() const;

         // changes the text
         void SetText(const std::string newText);

         // // edits the existing text
         // void EditText();

         // clears all characters.
         void ClearText();

         // sets a new color. If the alpha value has been changed, SetAlpha is called.
         void SetColor(cherry::Vec4 newColor);

         // gets the file path for the txt file that specifies the font name, font image, and characters.
         const std::string& GetFilePath() const override;

         // gets the name of the font being used. Calling GetName() on this object will get the same result.
         std::string GetFontName() const;

         // gets the file path of the font map.
         std::string GetFontMapFilePath() const;

         // gets the cell size of the font. This is used to determine the size of each character on the font map.
         glm::vec2 GetCellSize() const;

         // gets the font size of the text. This is the default size of the text (i.e. scale = (1.0, 1.0, 1.0)).
         int GetFontSize() const;

         // gets the spacing for the characters. 
         // While the character images may have different amounts of empty space, the spacing is the same for all character quads.
         float GetSpacing() const;

         // generates text from a text file.
        //  static cherry::Text GenerateText(std::string filePath, std::string font, cherry::Vec3 pos, cherry::Vec4 color, float size);
 
         // called to resize the text characters.
         void OnWindowResize(int newWidth, int newHeight) override;

         // update time.
         void Update(float deltaTime);

         // gets the text as a string
         std::string ToString() const override;
 
     private:
         
         // loads the text
         void LoadText(const std::string scene);
 
         std::string text = ""; // text
         std::string filePath = ""; // text path
         std::string fontMap = ""; // font // TODO: have a default file path for the font map.
         cherry::Vec4 color; // colour
         
         // checks to see if the text is visible or not.
         bool visible = true;

         // the spacing for text.
         float spacing = 0;

         // the font size
         float fontSize = 1;
         glm::vec2 cellSize{};

         static const int CHAR_COUNT; // 256
         
         // the characters (0 - 255)
         std::shared_ptr<Character> chars[256];
 
         // character uvs
         glm::vec4 chs[256];
         // bools for valid characters. If invalid, then a 'noChar' material is used.
         bool validChars[256];

         // the characters of the text.
         std::vector<Character*> textChars;

         // the characters for the text.
         // std::queue<Character *>textChars;

         // the world position, scale, and rotation.
         // the characters need to be updated if there are changes.
         Vec3 worldPos, worldScale, worldRotDeg;

         // checks to see if the text has been made into a window child.
         bool windowChild = false;

         // checks to see if the text should be post processed.
         bool postProcess = true;

         // materials for characters and characters with no symbol
         Material::Sptr knownCharMaterial = nullptr;

         // unusuable character material
         Material::Sptr unknownCharMaterial = nullptr;

         // the name of the current scene
         std::string sceneName = "";
     protected:

         // calculates the scale of the text.
         void CalculateTextScale();

         // calculates the rotation of the text
         void CalculateTextRotation();

         // calculates the position of the text.
         void CalculateTextPosition();

         // transforms the characters for scale
         void CalculateTextTransform();
 
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

         // sets the local position.
         void SetLocalPosition(Vec3 newPos);

         // sets the local position.
         void SetLocalPosition(glm::vec3 newPos);

         // gets the world position.
         cherry::Vec3 GetWorldPosition() const;

         // gets the world positon (glm). 
         glm::vec3 GetWorldPositionGLM() const;

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