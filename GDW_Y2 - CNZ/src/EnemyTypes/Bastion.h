#pragma once
#include "Enemy.h"


//Bastion class
namespace cnz {
	class Bastion : public cnz::Enemy
	{
	public:
		// constructor
		Bastion(std::string scene);

		// scene and material
		Bastion(std::string scene, cherry::Material::Sptr mat);

		Bastion(const cnz::Bastion&);

		Bastion(cnz::Enemy* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

		cherry::MorphAnimation walk;
	};
}

