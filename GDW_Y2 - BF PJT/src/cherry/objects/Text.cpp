#include "Text.h"
#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <toolkit/Logging.h>

#include "..\utils/Utils.h"
#include "..\textures/Texture2D.h"
#include "..\utils/math/Rotation.h"


// constructor
 cherry::Text::Text(std::string text, std::string scene, std::string font, cherry::Vec4 color, float size)
 	: Object(), text(text), fontPath(font), color(color), fontSize(size)
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
 cherry::Text::Text(std::string text, std::string scene, std::string font, float r, float g, float b, float a, float size)
 	: Text(text, scene, font, Vec4(r, g, b, a), size)
 {
 }
 
 // constructor
 cherry::Text::Text(std::string text, std::string scene, std::string font, glm::vec4 color, float size)
 	: Text(text, scene, font, Vec4(color), size)
 {
 }
 
 // load text
 void cherry::Text::LoadText(const std::string scene)
 {

     FT_Library ftLib; // free type library
     FT_Face ftFace;
     cherry::Shader::Sptr shader;
     cherry::Material::Sptr material; // material

     // TODO: allow for multiple rows.
     float tShift = 0; // shifts letters so that they're equally far apart.

    // if the font file is inaccessible, or it is not a ttf file
     if (!(util::fileAccessible(fontPath) && fontPath.substr(fontPath.find_last_of(".") + 1) == "ttf"))
     {
         LOG_ERROR("Font file could not be opened, or was not provided in ttf format. Using default font.");
         fontPath = FONT_ARIAL; // changes the font to the default.
     }

     // free type library initialization.
     if (FT_Init_FreeType(&ftLib))
         LOG_ERROR("Could not initialize FreeType library.");

     if (FT_New_Face(ftLib, fontPath.c_str(), 0, &ftFace))
         LOG_ERROR("Failed to load font.");

     // setting the font size. Width is set to (0) so that it's based on the font height.
     // FT_Set_Pixel_Sizes(ftFace, 0, fontSize);
     FT_Set_Pixel_Sizes(ftFace, 0, fontSize); // font size uses the scale instead.

     // generates a bitmap for the character, and checks if the font is readable.
     if (FT_Load_Char(ftFace, 'X', FT_LOAD_RENDER))
         LOG_ERROR("Failed to load glyph from TrueType font file");

     // disables byte-alignment restriction
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     // goes through the ASCII table
     // source: http://www.asciitable.com/
     // 0 - 127 is the standard ASCII table. 128 - 255 is the extended ASCII table.s
     for (GLubyte chr = 0; chr < 255; chr++)
     {
         // loads the character glyph
         if (FT_Load_Char(ftFace, chr, FT_LOAD_RENDER))
         {
             LOG_ERROR("Failed to load ASCII glyph.");
             continue;
         }

         // Generate texture
         GLuint texture;
         // a reference to this texture isn't being saved, so it's being lost to the void.
         glGenTextures(1, &texture);
         // glCreateTextures(GL_TEXTURE_2D, 1, &texture);

         // std::cout << "\nBitmap.Width: " << ftFace->glyph->bitmap.width << "\n";
         // std::cout << "Bitmap.Rows: " << ftFace->glyph->bitmap.rows << "\n";
         // std::cout << "Bitmap.Width: " << ftFace->glyph->bitmap.width << "\n";

         glBindTexture(GL_TEXTURE_2D, texture);
         // glBindTextureUnit(texture, chr);
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
        //  glTextureStorage2D(texture, 1, GL_RED, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows);



         // setting the texture options
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


         // glTexParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         // glTexParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         // glTexParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         // glTexParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



         // storing the character for later use.
         Glyph glyph{
             chr,
             texture,
             glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
             glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
             ftFace->glyph->advance.x
         };
         glyphs.push_back(glyph);

     }
     // glBindTexture(GL_TEXTURE_2D, 0);

     // destroying the free type the texture is now loaded.
     FT_Done_Face(ftFace);
     FT_Done_FreeType(ftLib);

     // TODO: make this the pane for the text.
     // making the vertices for each character

     // goes through each character and creates them.
     for (int i = 0; i < text.size(); i++)
     {
         // gets the character and its glyph.
         const char& c = text[i];
         const Glyph& g = glyphs[(int)c];


         // calculates the size.
         glm::vec2 size{ g.bearing.x + g.size.x, g.size.y - g.bearing.y + g.size.y};

         // creates the character.
         Character* chr = new Character(c, g.textureID, size);
         chr->CreateEntity(scene, chr->GetMaterial());

         chr->textPos = glm::vec2(tShift, 0);
         // chr->SetScale(fontSize); // font size uses the scale function.
         chars.push_back(chr);

         // bit shifts to get the value in pixels.
         tShift += (g.advance >> 6);
     }
    
     // glBindTexture(GL_TEXTURE_2D, 0);

     verticesTotal = 4;
     vertices = new Vertex[verticesTotal]
     {
         //  {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
         {{ -1.0F, -1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
         {{  1.0F, -1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
         {{ -1.0F,  1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
         {{  1.0F,  1.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
     };

     // making the indices for the quad
     indicesTotal = 6;
     uint32_t* indis = new uint32_t[indicesTotal]{
         0, 1, 2,
         2, 1, 3
     };

     mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

     shader = std::make_shared<Shader>();
     shader->Load("res/shaders/shader.vs.glsl", "res/shaders/shader.fs.glsl");
     material = std::make_shared<Material>(shader);

     // creating a plane to represent the size.
     CreateEntity(scene, material);
     SetVisible(false);
 }

 // update time
 void cherry::Text::Update(float deltaTime)
 {
     Object::Update(deltaTime);

     // TODO: optimize.

     // updating scale
     {
         // the scale of the text.
         Vec3 textScale = GetScale();

         // setting the scale proportional to the body.
         for (Character * chr : chars)
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
     }

     // updating rotation
     {
         // rotation
         for (Character * chr : chars)
             chr->SetRotationDegrees(GetRotationDegrees() + chr->GetRotationDegrees());
         
     }

     // updating position.
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
         for (Character * chr : chars)
         {
             Vec3 chrPos = chr->GetPosition();

             // gets the position of the character.
             glm::mat4 child
             {
                 chrPos.v.x, 0, 0, 0,
                 chrPos.v.y, 0, 0, 0,
                 chrPos.v.z, 0, 0, 0,
                 0, 0, 0, 0
             };

             result = parent * child;

             chr->SetPosition(result[0][3], result[1][3], result[2][3]);
         }
     }

     for (Character* chr : chars)
         chr->Update(deltaTime);
 }

 // toString
 std::string cherry::Text::ToString() const { return text; }

 
 // CHARACTERS
 // character constructor.
 // cherry::Character::Character(char a_char)
 //     : m_char(a_char)
 // {
 // }

 // constructor
 cherry::Character::Character(const char a_CHAR, GLuint textureID, glm::vec2 size)
     : m_CHAR(a_CHAR)
 {
     // shader and material
     Shader::Sptr shader;



     //  Material::Sptr material;
     // glActiveTexture(GL_TEXTURE0);

      // making the mesh
     verticesTotal = 4;
     vertices = new Vertex[verticesTotal]
     {
         //  {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
         // {{ -size.x / 2.0F, -size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
         // {{  size.x / 2.0F, -size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
         // {{ -size.x / 2.0F,  size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
         // {{  size.x / 2.0F,  size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
          //  {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
         {{ -size.x / 2.0F, -size.y / 2.0F, 0.0f }, { 1.0F, 0.5F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
         {{  size.x / 2.0F, -size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
         {{ -size.x / 2.0F,  size.y / 2.0F, 0.0f }, { 1.0F, 0.7F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
         {{  size.x / 2.0F,  size.y / 2.0F, 0.0f }, { 1.0F, 1.0F, 0.3F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
     };

     indicesTotal = 6;
     // making the indices for the quad
     indices = new uint32_t[indicesTotal]{
         0, 1, 2,
         2, 1, 3
     };

     mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
 
     shader = std::make_shared<Shader>();
     shader->Load("res/shaders/text.vs.glsl", "res/shaders/text.fs.glsl");
     // shader->Bind();

    // glBindTexture(GL_TEXTURE_2D, textureID);

     material = std::make_shared<Material>(shader);
     // material->GetShader()->Bind(); // don't bind?

     // glBindTextureUnit(textureID, textureID);
     // material->Set("a_Text", (int)textureID);
     // material->GetShader()->Bind();

    
     
    //  glBindTextureUnit(GL_TEXTURE_2D, textureID);
    // glBindTextureUnit(textureID, 0);
     material->Set("a_Text", (int)textureID);
    
     // glActiveTexture(GL_TEXTURE0); // GL_TEXTUREi where i can be replaced to be any unused texture slot
     // glBindTexture(GL_TEXTURE_2D, textureID);

     // glBindTexture(GL_TEXTURE_2D, textureID);
     // material->Set("a_Text", (int)textureID);
     // material->GetShader()->Bind();

     // material->HasTransparency = true;

     // shader->SetUniform("a_Text", 0);

    //  material->Set("a_Text", (int)textureID);

     // glBindTexture(GL_TEXTURE_2D, 0);
    //  material->Set("a_Text", 0);

 }
 
 
