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

		// faux copy constructor
		Bastion(cnz::Bastion* obj, std::string scene);

		// copy constructor
		Bastion(const cnz::Bastion&);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

	private:
		// loads all aniamtions
		void LoadAnimations() override;

		// emissive colour and power
		static const glm::vec3 EMISSIVE_COLOR;
		static const float EMISSIVE_POWER;

	protected:
	};
}

