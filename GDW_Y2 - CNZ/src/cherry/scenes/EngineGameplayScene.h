// the default scene the engine loads into
#pragma once

#include "GameplayScene.h"
#include "..\post/LookUpTable.h"
#include "..\post/DepthOfFieldLayer.h"
#include "..\post/BloomLayer.h"
#include "..\post/KernelLayer.h"
#include <queue>

namespace cherry
{
	// the scene the engine uses.
	class EngineGameplayScene : public cherry::GameplayScene
	{
	public:
		// constructor
		EngineGameplayScene(std::string sceneName);

		// loading the scene
		void OnOpen() override;

		// closing the scene
		void OnClose() override;

		// generates a new instance of the engine gameplay scene.
		virtual Scene* GenerateNewInstance() const override;

		// these functions get called by the game class by default, but they can be overwritten.
		void MouseButtonPressed(GLFWwindow* window, int button) override;

		// called when a mouse button is being held.
		void MouseButtonHeld(GLFWwindow* window, int button) override;

		// called when a mouse button has been pressed
		void MouseButtonReleased(GLFWwindow* window, int button) override;


		// called when a key has been pressed
		void KeyPressed(GLFWwindow* window, int key) override;

		// called when a key is being held down
		void KeyHeld(GLFWwindow* window, int key) override;

		// called when a key has been released
		void KeyReleased(GLFWwindow* window, int key) override;

		// ImGUI draw function.
		void DrawGui(float deltaTime) override;

		// update loop.
		void Update(float deltaTime) override;

		std::string nextScene = "";

	private:
		// translation and rotation direction
		glm::vec3 t_Dir = glm::vec3(0, 0, 0);
		glm::vec3 r_Dir = glm::vec3(0, 0, 0);

		// translation and rotation speeds
		float t_Inc = 22.50F;
		float r_Inc = 55.0F;

		// hitbox
		int hitBoxIndex = -1;

		// the left, middle, and right mouse buttons
		bool mbLeft = false, mbMiddle = false, mbRight = false;

		// the default game materials
		Material::Sptr matStatic = nullptr; // the static material
		Material::Sptr matDynamic = nullptr; // the dynamic material

		// defaults
		SamplerDesc description; // texture description 
		TextureSampler::Sptr sampler; // texture sampler

		// audio variables
		const bool ENABLE_AUDIO = true; // enables sound
		std::string bgmName = ""; // the name of the bgm
		glm::vec3 audioNear = glm::vec3(0.0F, 0.0F, 0.0F); // near positio for audio (loudest)
		glm::vec3 audioFar = glm::vec3(0.0F, 0.0F, 15.0F); // far position for audio (quietest/off)
		float audioT = 1.0F; // interpolates between audio near and audio far (starts at max volume)
		float audioT_inc = 0.5F; // incrementer for audio
		int volumeChange = 0; // -1 for lower, +1 for higher

		// temporary (to be removed after review 3)
		std::queue<PostLayer::Sptr> layers1;
		std::queue<PostLayer::Sptr> layers2;
		std::queue<PostLayer::Sptr> layers3;
		std::queue<PostLayer::Sptr> layers4;
		std::queue<PostLayer::Sptr> layers5;
		std::queue<PostLayer::Sptr> layers6;
		std::queue<PostLayer::Sptr> layers7;
		std::queue<PostLayer::Sptr> layers8;

		
		// PostLayer::Sptr layer1 = nullptr;
		// PostLayer::Sptr layer2 = nullptr;
		// PostLayer::Sptr layer3 = nullptr;
		// PostLayer::Sptr layer4 = nullptr;
		// PostLayer::Sptr layer5 = nullptr;
		// PostLayer::Sptr layer6 = nullptr;
		// PostLayer::Sptr layer7 = nullptr;

		// kernel layers
		Kernel3Layer edgeDetect1;
		Kernel3Layer edgeDetect2;
		Kernel3Layer edgeDetect3;
		Kernel3Layer sharpen;
		
		// the bloom layer options.
		BloomLayer bloomBox;
		BloomLayer bloomGau;
		BloomLayer bloomRad;

		// BloomLayer bloomLayer;

		// the depth of field layer
		DepthOfFieldLayer dofLayer;

		// the lookup table.
		LookUpTable warmTable;
		LookUpTable coolTable;

		// uses layers
		const bool USE_LAYERS = true;

		// tells the engine to write profiles
		const bool PROFILE = false;

		// if 'true', then this scene is being reopened. If false, the scene has not been open once.
		bool reopened = false;

	protected:
	};
}
