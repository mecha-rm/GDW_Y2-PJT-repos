#pragma once
#include "Enemy.h"

//Mechaspider class
namespace cnz {
	class Mechaspider : public cnz::Enemy
	{
	public:
		// constructor
		Mechaspider(std::string scene);

		// constructor
		Mechaspider(std::string scene, cherry::Material::Sptr mat);

		// copies the enemy and sets the scene.
		Mechaspider(cnz::Mechaspider* obj, std::string scene);

		// copy constructor
		Mechaspider(const cnz::Mechaspider&);

		// attack
		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		// update
		void Update(float dt);

		cherry::MorphAnimation* walk;

	private:
		// emissive colour and power
		static const glm::vec3 EMISSIVE_COLOR;
		static const float EMISSIVE_POWER;

	protected:
		// loads all the animations
		void LoadAnimations() override;
	};
}

