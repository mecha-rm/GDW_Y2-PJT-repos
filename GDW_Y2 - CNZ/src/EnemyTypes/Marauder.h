#pragma once
#include "Enemy.h"

//Marauder class
namespace cnz {
	class Marauder : public cnz::Enemy
	{
	public:
		// constructor
		Marauder(std::string scene);

		// constructor
		Marauder(std::string scene, cherry::Material::Sptr mat);

		// faux copy constructor.
		Marauder(cnz::Marauder* obj, std::string scene);

		// copy constructor.
		Marauder(const cnz::Marauder&);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

	private:
		// emissive colour and power
		static const glm::vec3 EMISSIVE_COLOR;
		static const float EMISSIVE_POWER;


	protected:
		// loads all animations
		void LoadAnimations() override;
	};
}

