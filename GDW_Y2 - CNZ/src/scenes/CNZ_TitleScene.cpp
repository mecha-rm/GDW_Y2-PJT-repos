#include "CNZ_TitleScene.h"
#include "..\CNZ_Game.h"
#include "..\LevelLoader.h"
#include "..\cherry\audio\AudioEngine.h"

// title scene
cnz::CNZ_TitleScene::CNZ_TitleScene(std::string sceneName)
	: cherry::MenuScene(sceneName)
{
}

// scene open
void cnz::CNZ_TitleScene::OnOpen()
{
	using namespace cherry;

	cherry::MenuScene::OnOpen();

	// new instances added.
	SetAllowingNewInstances(true);
	
	glm::ivec2 myWindowSize = Game::GetRunningGame()->GetWindowSize();

	const std::string SCENE_NAME = GetName();
	std::string buttonImage = "res/images/button_beta_sml.png";
	std::string textFnt = FONT_ARIAL;
	Vec4 textClr = Vec4(1.0F, 1.0F, 1.0F, 1.0F);

	glm::vec2 startPos{ 0.74, 0.65F }; // original: {0.9, 0.65F }
	glm::vec2 offset{ -0.24F, 0.0F }; // original: {-0.18F, 0.0F}

	glm::vec3 textLocalPos{ 0.0F, -3.0F, 2.0F };

	// skybox added in
	{
		// image options
		// "res/images/cubemaps/checkerboard_black-grey_d.jpg"
		// "res/images/cubemaps/blue_lightning.jpg"
		// "res/images/cubemaps/title_skybox_top.jpg"
		// "res/images/cubemaps/title_skybox_side.jpg"
		// "res/images/cubemaps/title_skybox_bottom.jpg"

		// cherry::Skybox skybox(
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg"
		// );

		cherry::Skybox skybox(
			"res/images/cubemaps/title_skybox_side.jpg",
			"res/images/cubemaps/title_skybox_side.jpg",
			"res/images/cubemaps/title_skybox_bottom.jpg",
			"res/images/cubemaps/title_skybox_top.jpg",
			"res/images/cubemaps/title_skybox_side.jpg",
			"res/images/cubemaps/title_skybox_side.jpg"
		);

		skybox.AddSkyboxToScene(this);
		Game::GetRunningGame()->SetSkyboxVisible(true);
	}

	// sound - bank loaded in CNZ_Game
	// TODO: have the audio start in CNZ_Game, and have the menu music start back up when a game scene closes.
	AudioEngine& audio = AudioEngine::GetInstance();

	// AudioEngine::GetInstance().LoadBank("Master");
	// AudioEngine::GetInstance().LoadEvent("bgm_01");
	// audio.LoadEvent("menu_accept");
	// audio.LoadEvent("menu_click"); 
	// audio.PlayEvent("bgm_01"); // gets started when closing the gameplay scene.

	// title screen
	{
		// original
		// Image * image = new Image("res/images/codename_zero_logo_small.png", sceneName, false, false);

		// animated version (3600 X 250 w/ 600 X 250 cell size)
		Image* image = new Image("res/images/codename_zero_logo_glow_ss_small.png", SCENE_NAME, 
			Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 600, 250), 3600, 250, false), false, false);

		image->SetWindowChild(true);
		image->SetPositionByWindowSize(0.5F, 0.2F);

		cherry::Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// image->SetScale(0.8F); // original
		image->SetScale(0.78F); // for animation

		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(false);

		image->AddPhysicsBody(pbb); // collision box

		entryButton = new Button();
		entryButton->object = image;

		AddButton(entryButton, false);

		// animation
		cherry::ImageAnimation* imgAnime = new ImageAnimation();
		image->AddAnimation(imgAnime, false);
		
		// 6 different frames (12 frames total)
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 0, 0, 600 * 1, 250), 3600, 250, false), 1.00F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 1, 0, 600 * 2, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 2, 0, 600 * 3, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 3, 0, 600 * 4, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 4, 0, 600 * 5, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 5, 0, 600 * 6, 250), 3600, 250, false), 0.3F));

		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 5, 0, 600 * 6, 250), 3600, 250, false), 0.3F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 4, 0, 600 * 5, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 3, 0, 600 * 4, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 2, 0, 600 * 3, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 1, 0, 600 * 2, 250), 3600, 250, false), 0.06F));
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/codename_zero_logo_glow_ss_small.png", Image::ConvertImagePixelsToUVSpace(Vec4(600 * 0, 0, 600 * 1, 250), 3600, 250, false), 1.00F));

		imgAnime->SetInfiniteLoop(true);
		imgAnime->Play();
		image->AddAnimation(imgAnime, true);
		image->SetVisible(true);
	}

	// ranking button
	{
		Image* image = new Image("res/images/ranking_button.png", SCENE_NAME, false, false);
		image->SetWindowChild(true);
		// image->SetPositionByWindowSize(Vec2(0.9, 0.8F));
		image->SetPositionByWindowSize(Vec2(0.5F, 0.83F));
		image->SetScale(0.3F);

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		rankButton = new Button();
		rankButton->object = image; // setting image as the object.

		// creating the text.
		// rankButton->text = new Text("Ranking", sceneName, textFnt, textClr, 10.0F);
		// rankButton->text->SetWindowChild(true);
		// rankButton->localTextPos = glm::vec3(0.0F, 0.0F, 2.0F);

		AddButton(rankButton, false);
		// UpdateButton(rankButton);
	}

	// button 1
	{
		Image* image = new Image("res/images/map1_button.png", SCENE_NAME, false, false);
		image->SetWindowChild(true);
		// image->SetPositionByWindowSize(Vec2(0.9, 0.8F));
		image->SetPositionByWindowSize(Vec2(startPos));
		image->SetScale(0.2F);

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		map1Button = new Button();
		map1Button->object = image; // setting image as the object.

		// creating the text.
		// map1Button->text = new Text("Map 1", sceneName, textFnt, textClr, 10.0F);
		// map1Button->text->SetWindowChild(true);
		// map1Button->localTextPos = textLocalPos;
		// map1Button->text->SetPosition(image->GetPosition() + textLocalPos);
		
		AddButton(map1Button, false);
		// UpdateButton(map1Button);
	}

	// button 2
	{
		Image* image = new Image("res/images/map2_button.png", SCENE_NAME, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(startPos.x + offset.x, startPos.y));
		image->SetScale(0.2F);

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		map2Button = new Button();
		map2Button->object = image; // setting image as the object.

		// creating the text.
		// map2Button->text = new Text("Map 2", sceneName, textFnt, textClr, 10.0F);
		// map2Button->text->SetWindowChild(true);
		// map2Button->localTextPos = textLocalPos;
		// map2Button->text->SetPosition(image->GetPosition() + textLocalPos);

		AddButton(map2Button, false);
		// UpdateButton(map2Button);
	}

	// button 3
	{
		Image* image = new Image("res/images/map3_button.png", SCENE_NAME, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(startPos.x + offset.x * 2, startPos.y));
		image->SetScale(0.2F);

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		map3Button = new Button();
		map3Button->object = image; // setting image as the object.

		// creating the text.
		// map3Button->text = new Text("Map 3", sceneName, textFnt, textClr, 10.0F);
		// map3Button->text->SetWindowChild(true);
		// map3Button->localTextPos = textLocalPos;
		// map3Button->text->SetPosition(image->GetPosition() + textLocalPos);

		AddButton(map3Button, false);
		// UpdateButton(map3Button);
	}

	// bonus fruit logo
	{
		cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_sml.png", GetName(),
			Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 395, 198), 5530, 198, false), true, false);

		// adding to list.
		objectList->objects.push_back(image);

		image->SetWindowChild(true);
		image->SetPositionByWindowSize(0.05F, 0.95F);
		image->SetScale(0.23F);
		
		// animation
		cherry::ImageAnimation* imgAnime = new ImageAnimation();
		image->AddAnimation(imgAnime, false);

		// 14 frames
		imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 0, 0, 395 * 1, 198), 5530, 198, false), 2.5F));
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
		// image->AddAnimation(imgAnime, false);
		image->SetVisible(true);
	}

	// background graphic (unused) and other background objects.
	{
		//cherry::Image* image = new Image("res/images/title_bg.png", GetName(), false, false);
		//image->SetWindowChild(true);
		//image->SetPositionByWindowSize(Vec2(0.5F, 0.5F));
		//image->SetPositionZ(-1.0F);
		//// image->SetOrthographicObject(true);
		//image->SetVisible(false); 

		//objectList->AddObject(image); 

		// terrain (inner city)
		cherry::Terrain* terrain_1 = new Terrain(SCENE_NAME, "res/images/heightmaps/city_heightmap_ver_1.jpg", 60.0F, 70);
		terrain_1->SetTexture(0, "res/images/city_heightmap_texture_1.jpg");
		terrain_1->SetTextureWeight(0, 0.1F);
		terrain_1->SetTexture(1, "res/images/city_heightmap_texture_2.jpg");
		terrain_1->SetTextureWeight(1, 0.5F);
		terrain_1->SetTexture(2, "res/images/city_heightmap_texture_3.jpg");
		terrain_1->SetTextureWeight(2, 0.4F);

		terrain_1->SetMinimumHeight(-10.0F);
		terrain_1->SetMaximumHeight(12.0F);

		// TODO: adjust height map since some buildings get cut-off.
		// terrain_1->SetPosition(0.0F, -30.0F, -50.0F);
		terrain_1->SetPosition(0.0F, -5.0F, -50.0F);
		terrain_1->SetVisible(true);

		objectList->AddObject(terrain_1); 

		// terrain 2 (outer city bottom left)
		cherry::Terrain* terrain_bl = new Terrain(SCENE_NAME, "res/images/heightmaps/city_heightmap_ver_2.jpg", 60.0F, 80);
		terrain_bl->SetTexture(0, "res/images/city_heightmap_texture_1.jpg");
		terrain_bl->SetTextureWeight(0, 0.1F);
		terrain_bl->SetTexture(1, "res/images/city_heightmap_texture_2.jpg");
		terrain_bl->SetTextureWeight(1, 0.5F);
		terrain_bl->SetTexture(2, "res/images/city_heightmap_texture_3.jpg");
		terrain_bl->SetTextureWeight(2, 0.4F);
		
		terrain_bl->SetMinimumHeight(-30.0F);
		terrain_bl->SetMaximumHeight(10.0F);
		
		terrain_bl->SetPosition(75.0F, -60.0F, -50.0F);
		terrain_bl->SetVisible(true);
		
		objectList->AddObject(terrain_bl);

		// terrain 3 (outer city bottom right)
		cherry::Terrain* terrain_br = new cherry::Terrain(*terrain_bl);
		terrain_br->SetPosition(-75.0F, -60.0F, -50.0F);
		objectList->AddObject(terrain_br);

		// terrain 3 (outer city top left)
		cherry::Terrain* terrain_tl = new cherry::Terrain(*terrain_bl);
		terrain_tl->SetPosition(75.0F, 4.5F, -50.0F);
		objectList->AddObject(terrain_tl);

		// terrain 3 (outer city top right)
		cherry::Terrain* terrain_tr = new cherry::Terrain(*terrain_bl);
		terrain_tr->SetPosition(-75.0F, 4.5F, -50.0F);
		objectList->AddObject(terrain_tr);

	}

	// instructons
	{
		cherry::Image* image = new Image("res/images/controls.png", GetName(), false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(0.5F, 0.5F));
		image->SetPositionZ(0.2F);
		image->SetScale(0.5F);
		image->SetVisible(false);

		// saving image and adding it to the list.
		controls = image;
		objectList->AddObject(image);

		cherry::Text* text = new Text("Press (I) to view controls/hide controls", SCENE_NAME, FONT_ARIAL, 
			Vec4(0.8157F, 0.9725F, 1.0F, 1.0F), 4.0F);
		text->SetWindowChild(true);
		text->SetPositionByWindowSize(Vec2(0.975F, 0.96F));
		objectList->AddObject(text);
	}

	// loading screen information
	{
		// text
		loadingText = new Text("LOADING", GetName(), textFnt, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 45.0F);
		loadingText->SetWindowChild(true);
		loadingText->SetPositionByWindowSize(Vec2(0.85F, 0.5F));
		loadingText->SetPostProcess(false);
		loadingText->SetVisible(false);

		objectList->AddObject(loadingText);

		// effect
		loadLayer = std::make_shared<PostLayer>(POST_VS, POST_GAUSSIAN_BLUR5_FS);
		loadLayer->AddLayer(POST_VS, POST_GAUSSIAN_BLUR5_FS);
		loadLayer->AddLayer(POST_VS, "res/shaders/post/bluescale.fs.glsl");
		
		loadLayer->AddLayer(POST_VS, "res/shaders/post/vibrance.fs.glsl");
		// loadLayer->GetLastPassShader()->SetUniform("a_Factor", 0.2F);


		// post processing
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


		// defaultLayer = std::make_shared<PostLayer>(POST_VS, POST_FS);
		// layers.push_back(defaultLayer);

		// enabling frame buffers
		useFrameBuffers = true;
	}

	Level::GenerateSources();

	// calling on window resize to fix aspect ratio
	Game::GetRunningGame()->Resize(myWindowSize.x, myWindowSize.y);
}

// closing
void cnz::CNZ_TitleScene::OnClose()
{
	// resetting variables back the original valeues
	loading = false;
	nextScene = "";
	controls = nullptr;

	cherry::MenuScene::OnClose();
}

// generates a new instance of the title scene.
cherry::Scene* cnz::CNZ_TitleScene::GenerateNewInstance() const
{
	return new CNZ_TitleScene(GetName());
}

// called when a mouse button has been pressed.
void cnz::CNZ_TitleScene::MouseButtonPressed(GLFWwindow* window, int button)
{
	cherry::MenuScene::MouseButtonPressed(window, button);

	cherry::AudioEngine::GetInstance().PlayEvent("menu_click");
}

// key has been pressed.
void cnz::CNZ_TitleScene::KeyPressed(GLFWwindow* window, int key)
{
	cherry::MenuScene::KeyPressed(window, key);

	switch (key)
	{
	case GLFW_KEY_I:
		if (controls != nullptr)
		{
			controls->SetVisible();
		}
		break;
	}
}

// update loop
void cnz::CNZ_TitleScene::Update(float deltaTime)
{
	using namespace cherry;

	MenuScene::Update(deltaTime);

	CNZ_Game* const game = (CNZ_Game*)Game::GetRunningGame();

	// if the load effect isn't enabled, or if the load effect is enabled and the loading screen isn't active. 
	if (enableLoadEffect == false || (enableLoadEffect == true && loading == false))
	{
		// a button has been entered and the mouse has been pressed.
		if (enteredButton != nullptr && mousePressed)
		{
			// AudioEngine::GetInstance().PlayEvent("menu_click");
			// if (!AudioEngine::GetInstance().isEventPlaying("menu_click")) {
			// 	AudioEngine::GetInstance().PlayEvent("menu_click");
			// }

			if (enteredButton == entryButton) // TODO: change the entry button to something else.
			{
				// taking out transition with logo press.
				// if (enableLoadEffect)
				// 	nextScene = game->map1Info.sceneName;
				// else
				// 	game->SetCurrentScene(game->map1Info.sceneName, true);
				
					
			}
			else if (enteredButton == rankButton) // ranking list
			{
				AudioEngine::GetInstance().PlayEvent("menu_accept");

				if (enableLoadEffect)
					nextScene = game->rankingSceneName;
				else
					game->SetCurrentScene(game->rankingSceneName, true);

			}
			else if (enteredButton == map1Button) // enters map 1
			{
				AudioEngine::GetInstance().PlayEvent("menu_accept");

				if (enableLoadEffect)
					nextScene = game->map1Info.sceneName;
				else
					game->SetCurrentScene(game->map1Info.sceneName, true);
			}
			else if (enteredButton == map2Button) // enters map 2
			{
				AudioEngine::GetInstance().PlayEvent("menu_accept");

				if (enableLoadEffect)
					nextScene = game->map2Info.sceneName;
				else
					game->SetCurrentScene(game->map2Info.sceneName, true);
			}
			else if (enteredButton == map3Button) // enters map 3
			{
				AudioEngine::GetInstance().PlayEvent("menu_accept");

				if (enableLoadEffect)
					nextScene = game->map3Info.sceneName;
				else
					game->SetCurrentScene(game->map3Info.sceneName, true);
			}

			// loading screen should be shown.
			if (enableLoadEffect && nextScene != "")
			{
				loading = true;
				// loadingText->SetVisible(true);
				// loadingText->SetText(loadingText->GetText());

				// layers.push_back(loadLayer.GetPostLayer());
				// PostLayer::Sptr layer = std::make_shared<PostLayer>(POST_VS, "res/shaders/post/invert.fs.glsl");
				layers.push_back(loadLayer); // adds in the load layer 
				useFrameBuffers = true;

				loadingText->SetVisible(true);
			}
		}
	}
	else if(enableLoadEffect == true && loading == true && nextScene != "")
	{
		// switching scenes
		game->SetCurrentScene(nextScene, true);
		return;
	}
	else
	{
	}

	//// Sound (for SOME REASON, the music only plays when the screen is moving. Removing this stops the music entirely.)
	// not needed? (I don't think it worked properly anyway)
	// if (!AudioEngine::GetInstance().isEventPlaying("bgm_01"))
	// {
	// 	AudioEngine::GetInstance().StopEvent("bgm_01");
	// 	AudioEngine::GetInstance().PlayEvent("bgm_01");
	// }

	// AudioEngine::GetInstance().PlayEvent("bgm_01");
	// AudioEngine::GetInstance().SetEventPosition("bgm_01", glm::vec3(0.0F, 0.0F, 0.0F));
	// AudioEngine::GetInstance().SetListenerPosition(glm:f:vec3(0.0F, 0.0F, 0.0F));

	// button has been hit
	// if (enteredButton == entryButton && mousePressed)
	// {
	// 	game->SetCurrentScene(game->map1Info.sceneName, true);
	// }

}
