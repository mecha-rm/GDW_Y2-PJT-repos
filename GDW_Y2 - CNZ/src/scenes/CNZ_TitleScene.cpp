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

	glm::ivec2 myWindowSize = Game::GetRunningGame()->GetWindowSize();

	std::string sceneName = GetName();
	std::string buttonImage = "res/images/button_beta_sml.png";
	std::string textFnt = FONT_ARIAL;
	Vec4 textClr = Vec4(1.0F, 1.0F, 1.0F, 1.0F);

	glm::vec2 startPos{ 0.9, 0.65F };
	glm::vec2 offset{ -0.18F, 0.0F };

	glm::vec3 textLocalPos{ 0.0F, -3.0F, 2.0F };

	// sound
	//AudioEngine::GetInstance().Init();
	AudioEngine::GetInstance().LoadBank("Master");

	AudioEngine::GetInstance().LoadEvent("Music");
	AudioEngine::GetInstance().LoadEvent("menu accept");
	AudioEngine::GetInstance().LoadEvent("menu click");

	// title screen
	{
		Image * image = new Image("res/images/codename_zero_logo_small.png", sceneName, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(0.5F, 0.2F);

		cherry::Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		image->SetScale(0.8F);

		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb); // collision box

		entryButton = new Button();
		entryButton->object = image;

		AddButton(entryButton, false);
	}

	// ranking button
	{
		Image* image = new Image("res/images/ranking_button.png", sceneName, false, false);
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
		Image* image = new Image("res/images/map1_button.png", sceneName, false, false);
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
		Image* image = new Image("res/images/map2_button.png", sceneName, false, false);
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
		Image* image = new Image("res/images/map3_button.png", sceneName, false, false);
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
		image->AddAnimation(imgAnime, false);
		image->SetVisible(true);
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

	cherry::MenuScene::OnClose();
}

// update loop
void cnz::CNZ_TitleScene::Update(float deltaTime)
{
	MenuScene::Update(deltaTime);

	using namespace cherry;

	CNZ_Game* const game = (CNZ_Game*)Game::GetRunningGame();

	// if the load effect isn't enabled, or if the load effect is enabled and the loading screen isn't active. 
	if (!enableLoadEffect || (enableLoadEffect && loading == false))
	{
		// a button has been entered and the mouse has been pressed.
		if (enteredButton != nullptr && mousePressed)
		{
			if (!AudioEngine::GetInstance().isEventPlaying("menu click")) {
				AudioEngine::GetInstance().PlayEvent("menu click");
			}

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
				if (enableLoadEffect)
					nextScene = game->rankingSceneName;
				else
					game->SetCurrentScene(game->rankingSceneName, true);

			}
			else if (enteredButton == map1Button) // enters map 1
			{
				if (enableLoadEffect)
					nextScene = game->map1Info.sceneName;
				else
					game->SetCurrentScene(game->map1Info.sceneName, true);
			}
			else if (enteredButton == map2Button) // enters map 2
			{
				if (enableLoadEffect)
					nextScene = game->map2Info.sceneName;
				else
					game->SetCurrentScene(game->map2Info.sceneName, true);
			}
			else if (enteredButton == map3Button) // enters map 3
			{
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
	else if(enableLoadEffect && loading == true && nextScene != "")
	{
		// switching scenes
		game->SetCurrentScene(nextScene, true);
		return;
	}
	else
	{
	}

	//// Sound
	if (!AudioEngine::GetInstance().isEventPlaying("Music")) {
		AudioEngine::GetInstance().PlayEvent("Music");
	}

	// button has been hit
	// if (enteredButton == entryButton && mousePressed)
	// {
	// 	game->SetCurrentScene(game->map1Info.sceneName, true);
	// }

}
