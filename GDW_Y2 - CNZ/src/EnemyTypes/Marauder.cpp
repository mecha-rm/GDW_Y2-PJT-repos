#include "Marauder.h"
#include "..\cherry\physics\PhysicsBody.h"

// emissive properties
// const glm::vec3 cnz::Marauder::EMISSIVE_COLOR = glm::vec3(0.9F, 0.1231F, 0.5F);
const glm::vec3 cnz::Marauder::EMISSIVE_COLOR = glm::vec3(0.105882353F, 0.164705882F, 1.0F);
const float cnz::Marauder::EMISSIVE_POWER = 1.0F;

// constructor
cnz::Marauder::Marauder(std::string scene) 
	: Enemy("res/objects/enemies/Marauder.obj", scene, true)
{
	type = cnz::marauder;
	description = "Marauder";
	SetPoints(1);
	SetSpeedMultiplier(10.0F);

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// constructor
cnz::Marauder::Marauder(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Marauder.obj", scene, mat)
{
	type = cnz::marauder;
	description = "Marauder";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// copy constructor (from a pointer)
cnz::Marauder::Marauder(cnz::Marauder* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	type = cnz::marauder;
	description = "Marauder";

	// play the walk animation if its available
	if (aniWalk.animation != nullptr)
	{
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}
}

// copy constructor
cnz::Marauder::Marauder(const cnz::Marauder& enemy) : Enemy(enemy)
{
	type = cnz::marauder;
	description = "Marauder";

	// play the walk animation if its available
	if (aniWalk.animation != nullptr)
	{
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}
}

// loads all animations
void cnz::Marauder::LoadAnimations()
{
	// Walk Animation
	{
		cherry::MorphAnimation* walk = new cherry::MorphAnimation();
		AddAnimation(walk);

		// there is no zeroth obj for some reason
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000000.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000001.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000002.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000003.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000004.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000005.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000006.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000007.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000008.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000009.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000010.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000011.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000012.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000013.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000014.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000015.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000016.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000017.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000018.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000019.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000020.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000021.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000022.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000023.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000024.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000025.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000026.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000027.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000028.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000029.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000030.obj", 0.04181f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000031.obj", 0.04181f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000032.obj", 0.04181f));

		walk->SetInfiniteLoop(true);
		aniWalk.animation = walk;
		aniWalk.index = GetAnimationCount() - 1;

		// play the walk animation
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}

	// Attack Animation
	{
		cherry::MorphAnimation* attack = new cherry::MorphAnimation();
		AddAnimation(attack);

		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Attack/Mar_Attack_000001.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Attack/Mar_Attack_000009.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Attack/Mar_Attack_000018.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Attack/Mar_Attack_000027.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Attack/Mar_Attack_000036.obj", 0.054f));

		attack->SetInfiniteLoop(true);
		aniAttack.animation = attack;
		aniAttack.index = GetAnimationCount() - 1;
	}
}

// attack function
void cnz::Marauder::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

// update function
void cnz::Marauder::Update(float dt)
{
	Enemy::Update(dt);
}