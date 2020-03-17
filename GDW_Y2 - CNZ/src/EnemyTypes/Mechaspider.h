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

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

		cherry::MorphAnimation walk;

	private:

	protected:
		// loads all the animations
		void LoadAnimations() override;
	};
}

