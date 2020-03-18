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

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

		cherry::MorphAnimation* walk;

	private:
		cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
		float arrowST = 0;

	protected:
		void LoadAnimations() override;
	};
}

