#pragma once
#include "Enemy.h"

//Oracle class
namespace cnz {
	class Oracle : public cnz::Enemy
	{
	public:
		// constructor
		Oracle(std::string scene);

		// constructor
		Oracle(std::string scene, cherry::Material::Sptr mat);

		// copy constuctor
		Oracle(const cnz::Oracle&);

		// faux copy constructor
		Oracle(cnz::Oracle* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

		cherry::MorphAnimation* walk;
		
	private:

	protected:
		void LoadAnimations() override;
	};
}

