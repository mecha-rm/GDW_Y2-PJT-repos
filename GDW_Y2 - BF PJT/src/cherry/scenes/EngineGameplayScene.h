// the default scene the engine loads into
#pragma once

#include "GameplayScene.h"
#include "..\post/LookUpTable.h"
#include "..\post/DepthOfFieldLayer.h"
#include "..\post/BloomLayer.h"

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

		// temporary (to be removed after review 3)
		PostLayer::Sptr layer1 = nullptr;
		PostLayer::Sptr layer2 = nullptr;
		PostLayer::Sptr layer3 = nullptr;
		PostLayer::Sptr layer4 = nullptr;
		PostLayer::Sptr layer5 = nullptr;
		PostLayer::Sptr layer6 = nullptr;
		PostLayer::Sptr layer7 = nullptr;

		// the depth of field layer
		DepthOfFieldLayer dofLayer;

		// the lookup table.
		LookUpTable table;
		
		// the bloom layer.
		BloomLayer bloomLayer;

		bool useLayers = true;

	protected:
	};
}
