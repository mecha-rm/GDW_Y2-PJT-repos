#include "EngineScene.h"

#include "..\objects/ObjectManager.h"
#include "..\lights/LightManager.h"
#include "..\Game.h"
#include "..\Skybox.h"
#include "..\physics/PhysicsBody.h"
#include "..\textures/TextureSampler.h"
#include "..\objects/Text.h"

// creating the engine scene.
cherry::EngineScene::EngineScene(std::string sceneName) : GameplayScene(sceneName)
{
}

void cherry::EngineScene::OnOpen()
{
	GameplayScene::OnOpen();

	Game* const game = Game::GetRunningGame();
	
	if (game == nullptr)
		return;

	// gets the window size
	glm::ivec2 myWindowSize = game->GetWindowSize();

	// setting up the camera
	game->myCamera = std::make_shared<Camera>();
	Camera::Sptr& myCamera = game->myCamera; // camera reference
	game->myCameraEnabled = true;

	myCamera->clearColor = game->myClearColor; // setting the clear colour
	myCamera->SetPosition(glm::vec3(0, 5, 12));
	myCamera->LookAt(glm::vec3(0));


	// sets the camera to perspective mode for the m_Scene.
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	//myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	myCamera->SetPerspectiveMode(glm::radians(45.0f), 1.0f, 0.01f, 1000.0f);
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f, false);
	// myCamera->followTarget = true;
	// myCamera->fixedTargetDistance = true;
	myCamera->targetOffset = cherry::Vec3(0, 5, 12);

	
	// secondary camera, which is used for UI for the game.
	game->myCameraX = std::make_shared<Camera>();
	Camera::Sptr& myCameraX = game->myCameraX;

	myCameraX->clearColor = game->myClearColor;
	myCameraX->SetPosition(0, 0.001F, 1.0F); // try adjusting the position of the perspecitve cam and orthographic cam
	myCameraX->Rotate(glm::vec3(0.0F, 0.0F, glm::radians(180.0f)));
	myCameraX->LookAt(glm::vec3(0));

	// this camera is used for UI elements
	myCameraX->SetPerspectiveMode(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f, false);
	myCameraX->SetOrthographicMode(-myWindowSize.x / 2.0F, myWindowSize.x / 2.0F, -myWindowSize.y / 2.0F, myWindowSize.y / 2.0F, 0.0f, 1000.0f, true);

	// creating the object manager and light manager
	// objManager = std::make_shared<ObjectManager>();
	// lightManager = std::make_shared<LightManager>();


	// SAMPLER FOR MIP MAPPING
	// added for mip mapping. As long as its above the material, it's fine.

	// OLD VERSION
	// TODO: remove upon final submission
	// description = SamplerDesc();
	// description.MinFilter = MinFilter::NearestMipNearest;

	// description.MagFilter = MagFilter::Nearest;
	// sampler = std::make_shared<TextureSampler>(description);

	// added for mip mapping. As long as its above the material, it's fine.
	description = SamplerDesc();

	description.MinFilter = MinFilter::LinearMipNearest;
	description.MagFilter = MagFilter::Linear;
	description.WrapS = description.WrapT = WrapMode::Repeat;

	// TODO: make linear and NearestMipNearest different variables?
	// called 'Linear' in the original code
	sampler = std::make_shared<TextureSampler>(description);

	// TODO: remove upon submission
	//desc1 = SamplerDesc();
	//desc1.MinFilter = MinFilter::NearestMipNearest;
	//desc1.MagFilter = MagFilter::Nearest;

	//desc2 = SamplerDesc();
	//desc2.MinFilter = MinFilter::LinearMipLinear;
	//desc2.MagFilter = MagFilter::Linear;

	//samplerEX = std::make_shared<TextureSampler>(desc1);



	// before the mesh in the original code
	Shader::Sptr phong = std::make_shared<Shader>();
	// TODO: make version without UVs?
	phong->Load("res/shaders/lighting.vs.glsl", "res/shaders/blinn-phong.fs.glsl");

	// TODO: change this so that it uses the light manager.
	// used to make the albedo
	// dedicated variable no longer needed?

	// no longer needed since GenerateMaterial() exists.
	// matStatic = std::make_shared<Material>(phong);
	// matStatic->Set("a_LightCount", 1);
	// matStatic->Set("a_LightPos[0]", { 0, 0, 3 });
	// matStatic->Set("a_LightColor[0]", { 0.5f, 0.1f, 0.9f});
	// matStatic->Set("a_AmbientColor[0]", { 0.9f, 0.1f, 0.01f });
	// matStatic->Set("a_AmbientPower[0]", 0.4f); // change this to change the main lighting power (originally value of 0.1F)
	// matStatic->Set("a_LightSpecPower[0]", 0.5f);
	// matStatic->Set("a_LightShininess[0]", 256.0f); // MUST be a float
	// matStatic->Set("a_LightAttenuation[0]", 0.15f);
	// material->Set("s_Albedo", albedo, sampler); // sceneLists will just be blank if no texture is set.

	// testMat->Set("s_Albedo", albedo); // right now, this is using the texture state.

		// Shader was originally compiled here.
	// // Create and compile shader
	// myShader = std::make_shared<Shader>();
	// myShader->Load("res/shaders/shader.vs.glsl", "res/shaders/shader.fs.glsl");
	// 
	// myModelTransform = glm::mat4(1.0f); // initializing the model matrix
	// testMat->Set("s_Albedo", albedo, Linear); // now uses mip mapping

	// No longer needed since GenerateMaterial exists.
	// Texture2D::Sptr albedo = Texture2D::LoadFromFile("res/images/default.png");
	// matStatic->Set("s_Albedos[0]", albedo, sampler);
	// matStatic->Set("s_Albedos[1]", albedo, sampler);
	// matStatic->Set("s_Albedos[2]", albedo,sampler);




	// creating a skybox for the scene.
	cherry::Skybox skybox(
		"res/images/cubemaps/checkerboard_black-red.jpg",
		"res/images/cubemaps/checkerboard_black-green.jpg",
		"res/images/cubemaps/checkerboard_black-blue.jpg",
		"res/images/cubemaps/checkerboard_red-white.jpg",
		"res/images/cubemaps/checkerboard_green-white.jpg",
		"res/images/cubemaps/checkerboard_blue-white.jpg"
	);

	// game->CreateScene("Cherry", skybox, true); // creates the scene
	// game->SetCurrentScene(GetName(), true);
	// game->GetCurrentScene()->SkyboxMesh->SetVisible(true); // makes the skybox invisible
	
	skybox.AddSkyboxToScene(this);
	this->SkyboxMesh->SetVisible(true); // makes the skybox invisible

	ObjectManager::CreateSceneObjectList(game->GetCurrentSceneName()); // creating an object list for the scene
	objectList = ObjectManager::GetSceneObjectListByName(game->GetCurrentSceneName()); // getting the object list.

	// TODO: add sampler for light list?
	LightManager::CreateSceneLightList(game->GetCurrentSceneName());
	lightList = LightManager::GetSceneLightListByName(game->GetCurrentSceneName()); // getting the light list

	lightList->AddLight(new Light(game->GetCurrentSceneName(), Vec3(-7.0F, 0.0F, 0.0F), Vec3(1.0F, 0.1F, 0.1F),
		Vec3(0.1F, 1.0F, 0.4F), 0.4F, 0.2F, 250.0F, 0.15F));

	lightList->AddLight(new Light(game->GetCurrentSceneName(), Vec3(7.0F, 0.0F, 0.0F), Vec3(0.1, 0.1F, 1.0F),
		Vec3(0.2F, 0.7F, 0.9F), 0.3F, 0.5F, 256.0F, 0.15F));

	game->AddLightToScene(new Light(game->GetCurrentSceneName(), Vec3(0.0F, 7.0F, 0.0F), Vec3(0.3, 0.9F, 0.1F),
		Vec3(0.8F, 0.2F, 0.95F), 0.9F, 0.7F, 100.0F, 0.85F));

	// material = LightManager::GetLightList(currentScene)->at(1).GenerateMaterial(sampler);
	// replace the shader for the material if using morph tagets.
	matStatic = lightList->GenerateMaterial(STATIC_VS, STATIC_FS, sampler);
	matDynamic = lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler);


	// loads in default sceneLists
	if(true)
	{
		Material::Sptr objMat; // used for custom materials
		float offset = 3.0F; // position offset

		  //sceneLists.push_back(new PrimitiveCube(5));
		  //sceneLists.at(sceneLists.size() - 1)->CreateEntity(currentScene, matStatic);
		  //sceneLists.at(sceneLists.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);

		// Creating the sceneLists, storing them, and making them part of the default m_Scene.
		objectList->objects.push_back(new PrimitiveCapsule());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, -offset, 0.0F);
		
		objectList->objects.push_back(new PrimitiveCircle());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, 0.0f, 0.0F);
		 
		objectList->objects.push_back(new PrimitiveCone());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, offset, 0.0F);
		 
		objectList->objects.push_back(new PrimitiveCube());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, -offset, 0.0F);
		
		objectList->objects.push_back(new PrimitiveCylinder());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);
		
		objectList->objects.push_back(new PrimitiveDiamond());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, offset, 0.0F);
		
		objectList->objects.push_back(new PrimitiveUVSphere());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, -offset, 0.0F);
		
		objectList->objects.push_back(new PrimitivePlane());
		objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(game->GetCurrentSceneName(), matStatic);
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, 0.0F, 0.0F);

		// testing the copy constructor.
		// objectList->objects.push_back(new PrimitivePlane(*(PrimitivePlane *)objectList->objects.at(objectList->objects.size() - 1)));
		// objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, 3.0F, -20.0F);
		// objectList->objects.at(objectList->objects.size() - 1)->SetScale(45.0F);

	   // liquid
		{
			Liquid* water = new Liquid(game->GetCurrentSceneName(), 20.0f, 100);
			water->SetEnabledWaves(3);
			water->SetGravity(9.81F);

			water->SetWave(0, 1.0f, 0.0f, 0.50f, 6.0f);
			water->SetWave(1, 0.0f, 1.0f, 0.25f, 3.1f);
			water->SetWave(2, 1.0f, 1.4f, 0.20f, 1.8f);

			water->SetColor(0.5f, 0.5f, 0.95f, 0.75f);
			water->SetClarity(0.9f);

			water->SetFresnelPower(0.5f);
			water->SetRefractionIndex(1.0f, 1.34f);
			water->SetEnvironment(game->GetCurrentScene()->Skybox);

			water->SetPosition(0.0F, 0.0F, -70.0F);
			water->SetVisible(true);
			game->AddObjectToScene(water);
		}

		// Height Map
		{
			Terrain* terrain = new Terrain(game->GetCurrentSceneName(), "res/images/heightmaps/heightmap.bmp", 30.0f, 50, false);
			terrain->SetTexture(0, "res/images/red.png");
			terrain->SetTexture(1, "res/images/green.png");
			terrain->SetTexture(2, "res/images/blue.png");
			terrain->SetMinimumHeight(-5.0F);
			terrain->SetMaximumHeight(10.0F);
			terrain->SetPosition(0.0F, 0.0F, -15.0F);
			terrain->SetVisible(true);
			game->AddObjectToScene(terrain);
		}
		//// sceneLists.push_back(new Object("res/sceneLists/monkey.obj", currentScene, material));
		{
			// images don't need CreateEntity called.
			// Image Test (1)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_v01.png", game->GetCurrentSceneName(), true, false);

			// Image Test (2)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss.png", game->GetCurrentSceneName(), false, false);

			// sprite sheet (currently 21 frames)

			// Image Test (3)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_bw.png", game->GetCurrentSceneName(), 
			// 	Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 1185, 594), 5925, 594, false), true, false);
			// 
			// // ..ss_bw and ..ss_rb are the same size, and are good for showing image switching. However, it's slow to siwtch them.
			// cherry::ImageAnimation* imgAnime = new ImageAnimation();
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_bw.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 0, 0, 1185 * 1, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_rb.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 1, 0, 1185 * 2, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_bw.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 2, 0, 1185 * 3, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_rb.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 3, 0, 1185 * 4, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_bw.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 4, 0, 1185 * 5, 594), 5925, 594, false), 0.5F));
			// imgAnime->SetInfiniteLoop(true);
			// imgAnime->Play();
			// image->AddAnimation(imgAnime, false);

			// Image Test (4)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_bw.png", game->GetCurrentSceneName(), 
			// 	Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 1185, 594), 5925, 594, false), true, false);
			cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_sml.png", game->GetCurrentSceneName(),
				Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 395, 198), 5530, 198, false), true, false);

			// ..ss_bw and ..ss_rb are the same size, and are good for showing image switching. However, it's slow to siwtch them.
			cherry::ImageAnimation* imgAnime = new ImageAnimation();

			// 14 frames
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 0, 0, 395 * 1, 198), 5530, 198, false), 0.5F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 1, 0, 395 * 2, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 2, 0, 395 * 3, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 3, 0, 395 * 4, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 4, 0, 395 * 5, 198), 5530, 198, false), 0.0F));

			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 5, 0, 395 * 6, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 6, 0, 395 * 7, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 7, 0, 395 * 8, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 8, 0, 395 * 9, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 9, 0, 395 * 10, 198), 5530, 198, false), 0.0F));

			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 10, 0, 395 * 11, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 11, 0, 395 * 12, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 12, 0, 395 * 13, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 13, 0, 395 * 14, 198), 5530, 198, false), 0.0F));


			imgAnime->SetInfiniteLoop(true);
			imgAnime->Play();
			image->AddAnimation(imgAnime, false);
			image->SetVisible(true);

			objectList->objects.push_back(image);
			objectList->objects.at(objectList->GetObjectCount() - 1)->SetPosition(0.0F, 0.0F, 1.0F);
			objectList->objects.at(objectList->GetObjectCount() - 1)->SetScale(0.01F);

			// image->GetAnimation(0)->Play();

		}

		// image (UI element)
		{
			cherry::Image* image = new Image("res/images/codename_zero_logo.png", game->GetCurrentSceneName(), false, false);
			image->SetPosition(-30.0F, -2.0F, 0.0F);
			image->SetWindowChild(true);
			image->SetPostProcess(false); // should not be post processed.
			// image->SetPositionByScreenPortion(Vec2(0.5, 0.5), Vec2(myWindowSize), Vec2(0.5, 0.5));
			// image->SetPosition(myCamera->GetPosition() + glm::vec3(0.0F, 0.0F, -10.0F));
			image->SetScale(0.12F);
			image->SetAlpha(0.8F);
			image->SetVisible(true);
			objectList->AddObject(image);
			image->SetPositionByWindowSize(Vec2(1.0F, 1.0F) - Vec2(0.80F, 0.88F));
		}

		// text
		{
			// text
			// TODO: find out why items are layeirng on top of one another.
			Text* text = new Text("Hello World", GetName(), FONT_ARIAL, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 2.0F);
			text->SetPosition(1.0F, 1.0F, 3.0F);
			objectList->AddObject(text); 
		}

		// version 1 (finds .mtl file automatically)
		objectList->objects.push_back(new Object("res/objects/charactoereee.obj", game->GetCurrentSceneName(),
			lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler), true, true));

		// objectList->objects.push_back(new Object("res/objects/charactoereee.obj", currentScene,
		// LightManager::GetSceneLightsMerged(currentScene)->GenerateMaterial(sampler), true, true));


		objectList->objects.at(objectList->objects.size() - 1)->SetScale(10.0F);
		hitBoxIndex = objectList->objects.size() - 1;

		// sceneLists.push_back();

		// version 2 (.mtl file manually added)
		//sceneLists.push_back(new Object("res/sceneLists/MAS_1 - QIZ04 - Textured Hammer.obj", currentScene, 
		// 	LightManager::GetSceneLightsMerged(currentScene)->GenerateMaterial(STATIC_VS, STATIC_FS, sampler),
		// 	"res/sceneLists/MAS_1 - QIZ04 - Textured Hammer.mtl", false));

		// PhysicsBodyBox* temp = new PhysicsBodyBox(Vec3(0.0F, 0.0F, 0.0F), 1.0F, 3.0F, 1.0F);
		cherry::PhysicsBodyBox* temp = new PhysicsBodyBox(Vec3(0.0F, 1.0F, 0.0F), 1.0F, 3.0F, 1.0F);
		// temp->SetRotationDegrees(Vec3(0, 0, 30.0F));
		// temp->SetScale(Vec3(2.0F, 2.0F, 2.0F));
		objectList->objects.at(objectList->objects.size() - 1)->AddPhysicsBody(temp);
		objectList->objects.at(objectList->objects.size() - 1)->GetPhysicsBodies()[0]->SetVisible(true);

		// objectList->objects.at(objectList->objects.size() - 1)->SetScale(Vec3(2.0F, 2.0F, 2.0F));
		// objectList->objects.at(objectList->objects.size() - 1)->SetRotationZDegrees(45.0F);

		// path
		Path path = Path();
		path.AddNode(8.0F, 0.0F, 0.0F);
		path.AddNode(-8.0F, 8.0F, 0.0F);
		path.AddNode(20.0F, 8.0F, 8.0F);
		path.AddNode(7.0F, 9.0F, 7.0F);
		path.AddNode(8.0F, -8.0F, -8.0F);
		path.SetIncrementer(0.1f);

		path.SetInterpolationMode(1);
		path.SetOpenPath(false);
		path.SetSpeedControl(true);

		objectList->objects.at(objectList->objects.size() - 1)->SetPath(path, true);

		objectList->objects.at(objectList->objects.size() - 1)->SetScale(0.7);

		// sceneLists.at(sceneLists.size() - 1)->CreateEntity(currentScene, objMat);
		// sceneLists.at(sceneLists.size() - 1)->SetPosition(0.0F, 0.0F, -10.0F);
		// sceneLists.at(sceneLists.size() - 1)->SetScale(2.0F);

		//material->SetShader(shdr);
		// VER 1
		//sceneLists.push_back(new Object("res/sceneLists/cube_morph_target_0.obj", currentScene, matDynamic, false, true));
		//sceneLists.at(sceneLists.size() - 1)->SetPosition(offset, offset, 0.0F);
		////

		//MorphAnimation* mph = new MorphAnimation();
		//mph->AddFrame(new MorphAnimationFrame("res/sceneLists/cube_morph_target_0.obj", 2.0F));
		//mph->AddFrame(new MorphAnimationFrame("res/sceneLists/cube_morph_target_1.obj", 2.0F));

		// VER 2
		objectList->objects.push_back(new Object("res/objects/hero pose one.obj", game->GetCurrentSceneName(), matDynamic, false, true));
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, offset, 0.0F);
		//

		MorphAnimation* mph = new MorphAnimation();
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero pose one.obj", 2.0F));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero pose two.obj", 2.0F));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero pose three.obj", 2.0F));
		// mph->AddFrame(new MorphAnimationFrame("res/sceneLists/cube_target_0.obj", 2.0F));
		mph->SetInfiniteLoop(true);
		// TODO: set up ability to return to pose 0, t-pose, or stay on ending frame.
		//mph->SetLoopsTotal(3);
		mph->Play();
		objectList->objects.at(objectList->objects.size() - 1)->AddAnimation(mph, true);
		// sceneLists.at(sceneLists.size() - 1)->GetMesh()->SetVisible(false);

	}



	// Switching a scene.
	// CreateScene("AIS", false);
	// objectList->objects.at(0)->SetScene("AIS");
	// SetCurrentScene("AIS", false);

	// Create and compile shader
	// myShader = std::make_shared<Shader>();
	// myShader->Load("res/shaders/shader.vs.glsl", "res/shaders/shader.fs.glsl");

	// myModelTransform = glm::mat4(1.0f); // initializing the model matrix

	// frame buffer
	FrameBuffer::Sptr fb = std::make_shared<FrameBuffer>(myWindowSize.x, myWindowSize.y);

	// scene colour
	RenderBufferDesc sceneColor = RenderBufferDesc();
	sceneColor.ShaderReadable = true;
	sceneColor.Attachment = RenderTargetAttachment::Color0;
	sceneColor.Format = RenderTargetType::Color24; // loads with RGB

	// scene depth
	RenderBufferDesc sceneDepth = RenderBufferDesc();
	sceneDepth.ShaderReadable = true;
	sceneDepth.Attachment = RenderTargetAttachment::Depth;
	sceneDepth.Format = RenderTargetType::Depth24;

	// colour and depth attachments
	fb->AddAttachment(sceneColor);
	fb->AddAttachment(sceneDepth);


	// fb->AddAttachment()
	Registry().ctx_or_set<FrameBuffer::Sptr>(fb);

	// adds a post-processing 
	layers.push_back(new PostLayer(POST_VS, "res/shaders/post/invert.fs.glsl"));
	layers.push_back(new PostLayer(POST_VS, "res/shaders/post/greyscale.fs.glsl"));

	useFrameBuffers = true;

	// the audio engine
	AudioEngine& audioEngine = game->audioEngine;

	// TODO: streamline audio inclusion
	// Load a bank (Use the flag FMOD_STUDIO_LOAD_BANK_NORMAL)
	// TODO: put in dedicated folder with ID on it?
	audioEngine.LoadBank("res/audio/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);

	// Load an event
	audioEngine.LoadEvent("Music", "{13471b17-f4bd-4cd5-afaa-e9e60eb1ee67}");
	// Play the event
	audioEngine.PlayEvent("Music");
	audioEngine.StopEvent("Music"); // TODO: uncomment if you want the music to play.

}

// called when the scene is being closed.
void cherry::EngineScene::OnClose()
{
	GameplayScene::OnClose();
}

// mouse button has been pressed.
void cherry::EngineScene::MouseButtonPressed(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mbLeft = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mbMiddle = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mbRight = true;
		break;
	}
}

// mouse button is being held.
void cherry::EngineScene::MouseButtonHeld(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mbLeft = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mbMiddle = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mbRight = true;
		break;
	}
}

// mouse button has been released.
void cherry::EngineScene::MouseButtonReleased(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mbLeft = false;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mbMiddle = false;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mbRight = false;
		break;
	}
}

void cherry::EngineScene::KeyPressed(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks key value.
	switch (key)
	{
	case GLFW_KEY_SPACE:
		game->myCamera->SwitchViewMode();
		break;

	// CAMERA CONTROLS
	// TRANSLATIONS
	case GLFW_KEY_W: // y-direction up
		t_Dir[1] = -1;
		break;

	case GLFW_KEY_S: // y-direction down
		t_Dir[1] = 1;
		break;

	case GLFW_KEY_A: // x-direction left
		t_Dir[0] = 1;
		break;

	case GLFW_KEY_D: // x-direction right
		t_Dir[0] = -1;
		break;

	case GLFW_KEY_Q: // z-direction backward
		t_Dir[2] = 1;
		break;

	case GLFW_KEY_E: // z-direction forward
		t_Dir[2] = -1;
		break;

		// ROTATIONS
	case GLFW_KEY_UP: // y-direction +
		r_Dir[0] = -1;
		break;

	case GLFW_KEY_DOWN: // y-direction -
		r_Dir[0] = 1;
		break;

	case GLFW_KEY_LEFT: // x-direction +
		r_Dir[1] = -1;
		break;

	case GLFW_KEY_RIGHT: // x-direction -
		r_Dir[1] = 1;
		break;

	case GLFW_KEY_PAGE_UP: // z-direction -
		r_Dir[2] = -1;
		break;

	case GLFW_KEY_PAGE_DOWN: // z-direction +
		r_Dir[2] = 1;
		break;

		// resets the camera so that it looks at the origin
	case GLFW_KEY_L:
		if (game->myCamera != nullptr)
			game->myCamera->LookAt(game->myCamera->LookingAt());
		break;

		// TODO: remove these
	case GLFW_KEY_V:
		if (hitBoxIndex >= 0 && hitBoxIndex < objectList->objects.size())
			objectList->objects[hitBoxIndex]->GetPhysicsBodies()[0]->SetVisible();
		break;
	case GLFW_KEY_P:
		if (hitBoxIndex >= 0 && hitBoxIndex < objectList->objects.size())
			objectList->objects[hitBoxIndex]->followPath = !objectList->objects[hitBoxIndex]->followPath;
	case GLFW_KEY_I:
		if (hitBoxIndex >= 0 && hitBoxIndex < objectList->objects.size())
		{
			if (objectList->objects[hitBoxIndex]->GetPath().GetInterpolationMode() == 0)
			{
				objectList->objects[hitBoxIndex]->GetPath().SetInterpolationMode(1);
			}
			else if (objectList->objects[hitBoxIndex]->GetPath().GetInterpolationMode() == 1)
			{
				objectList->objects[hitBoxIndex]->GetPath().SetInterpolationMode(0);
			}

		}
		break;
	}
}

// key held
void cherry::EngineScene::KeyHeld(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	switch (key)
	{
		// CAMERA CONTROLS
			// TRANSLATIONS
	case GLFW_KEY_W: // y-direction up
		t_Dir[1] = -1;
		break;

	case GLFW_KEY_S: // y-direction down
		t_Dir[1] = 1;
		break;

	case GLFW_KEY_A: // x-direction left
		t_Dir[0] = 1;
		break;

	case GLFW_KEY_D: // x-direction right
		t_Dir[0] = -1;
		break;

	case GLFW_KEY_Q: // z-direction backward
		t_Dir[2] = 1;
		break;

	case GLFW_KEY_E: // z-direction forward
		t_Dir[2] = -1;
		break;

		// ROTATIONS
	case GLFW_KEY_UP: // y-direction +
		r_Dir[0] = -1;
		break;

	case GLFW_KEY_DOWN: // y-direction -
		r_Dir[0] = 1;
		break;

	case GLFW_KEY_LEFT: // x-direction +
		r_Dir[1] = -1;
		break;

	case GLFW_KEY_RIGHT: // x-direction -
		r_Dir[1] = 1;
		break;

	case GLFW_KEY_PAGE_UP: // z-direction -
		r_Dir[2] = -1;
		break;

	case GLFW_KEY_PAGE_DOWN: // z-direction +
		r_Dir[2] = 1;
		break;
	}
}

// key released
void cherry::EngineScene::KeyReleased(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	switch (key)
	{
		// CAMERA CONTROLS
		// TRANSLATIONS
		// y-axis movement
	case GLFW_KEY_W:
	case GLFW_KEY_S:
		t_Dir[1] = 0;
		break;

		// x-axis movement
	case GLFW_KEY_A:
	case GLFW_KEY_D:
		t_Dir[0] = 0;
		break;

		// z-axis movement
	case GLFW_KEY_Q:
	case GLFW_KEY_E:
		t_Dir[2] = 0;
		break;

		// ROTATIONS
		// y-axis rotation
	case GLFW_KEY_UP:
	case GLFW_KEY_DOWN:
		r_Dir[0] = 0;
		break;

		// x-axis rotation
	case GLFW_KEY_LEFT:
	case GLFW_KEY_RIGHT:
		r_Dir[1] = 0;
		break;

		// z-axis rotation
	case GLFW_KEY_PAGE_UP:
	case GLFW_KEY_PAGE_DOWN:
		r_Dir[2] = 0;
		break;

		// deletes an object
	case GLFW_KEY_0:
		game->DeleteObjectFromScene(objectList->objects.at(0));
		break;
	}
}

// update loop
void cherry::EngineScene::Update(float deltaTime)
{
	Game* const game = Game::GetRunningGame();
	// TODO: remove this line.
	// <the update loop for all sceneLists was originally here.>

	// updates the camera
		// moving the camera
	game->myCamera->SetPosition(game->myCamera->GetPosition()
		+ glm::vec3(t_Dir[0] * t_Inc * deltaTime, t_Dir[1] * t_Inc * deltaTime, t_Dir[2] * t_Inc * deltaTime));

	// rotating the camera
	game->myCamera->Rotate(
		glm::vec3(glm::radians(r_Dir[0] * r_Inc * deltaTime),
			glm::radians(r_Dir[1] * r_Inc * deltaTime),
			glm::radians(r_Dir[2] * r_Inc * deltaTime)
		)
	);


	// collision calculations
mainLoop:
	for (cherry::Object* obj1 : objectList->objects) // object 1
	{
		if (obj1 == nullptr)
			continue;

		if (obj1->GetIntersection() == true) // already colliding with something.
			continue;

		for (cherry::Object* obj2 : objectList->objects) // object 2
		{
			if (obj1 == obj2 || obj2 == nullptr) // if the two sceneLists are the same.
				continue;

			if (obj2->GetIntersection() == true) // if the object is already intersecting with something.
				continue;

			// gets the vectors from both sceneLists
			std::vector<cherry::PhysicsBody*> pbods1 = obj1->GetPhysicsBodies();
			std::vector<cherry::PhysicsBody*> pbods2 = obj2->GetPhysicsBodies();

			// goes through each collision body
			for (cherry::PhysicsBody* pb1 : pbods1)
			{
				for (cherry::PhysicsBody* pb2 : pbods2)
				{
					bool col = PhysicsBody::Collision(pb1, pb2);

					if (col == true) // if collision has occurred.
					{
						obj1->SetIntersection(true);
						// obj1->setColor(255, 0, 0);
						obj2->SetIntersection(true);
						// obj2->setColor(255, 0, 0);
						// std::cout << "Hit!" << std::endl;

						goto mainLoop; // goes back to the main loop
					}
				}
			}
		}
	}

}
