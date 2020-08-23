#pragma once
#include "Enemy.h"

//Sentry class
namespace cnz {
	class Sentry : public cnz::Enemy
	{
	public:
		// constructor
		Sentry(std::string scene);

		// constructor
		Sentry(std::string scene, cherry::Material::Sptr mat);

		// copy constructor.
		Sentry(const cnz::Sentry&);

		// copies the enemy and sets its scene.
		Sentry(const cnz::Sentry* obj, std::string scene);

		// attack
		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		// update
		void Update(float dt);

	private:
		// emissive colour and power
		static const glm::vec3 EMISSIVE_COLOR;
		static const float EMISSIVE_POWER;

		// arrow direction vector.
		cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
		float arrowST = 0;

	protected:
		// loads animations
		void LoadAnimations() override;
	};
}

