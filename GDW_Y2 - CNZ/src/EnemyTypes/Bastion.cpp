#include "Bastion.h"
#include "..\cherry\physics\PhysicsBody.h"

// emissive properties
// const glm::vec3 cnz::Bastion::EMISSIVE_COLOR = glm::vec3(0.1F, 0.8F, 0.1F);
const glm::vec3 cnz::Bastion::EMISSIVE_COLOR = glm::vec3(0.415686275F, 0.949019608F, 0.443137255F);
const float cnz::Bastion::EMISSIVE_POWER = 1.0F;

// TODO: replace with proper file path.
// constructor
cnz::Bastion::Bastion(std::string scene)
	: Enemy("res/objects/enemies/Bastion.obj", scene, true)
{
	type = cnz::bastion;
	description = "Bastion";
	SetPoints(3);
	SetSpeedMultiplier(13.0F);

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// scene and material
cnz::Bastion::Bastion(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Bastion.obj", scene, mat)
{
	type = cnz::bastion;
	description = "Bastion";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// copying from a pointer
cnz::Bastion::Bastion(cnz::Bastion* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	type = cnz::bastion;
	description = "Bastion";

	// play the walk animation if its available
	if (aniWalk.animation != nullptr)
	{
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}
}

// copy constructor
cnz::Bastion::Bastion(const cnz::Bastion& emy) : cnz::Enemy(emy)
{
	type = cnz::bastion;
	description = "Bastion";

	// play the walk animation if its available
	if (aniWalk.animation != nullptr)
	{
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}
}

// loads all animations
void cnz::Bastion::LoadAnimations()
{
	// walk animation
	{
		cherry::MorphAnimation* walk = new cherry::MorphAnimation();
		AddAnimation(walk);

		// there is no zeroth obj for some reason
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000000.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000001.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000002.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000003.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000004.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000005.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000006.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000007.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000008.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000009.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000010.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000011.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000012.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000013.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000014.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000015.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000016.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000017.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000018.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000019.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000020.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000021.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000022.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000023.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000024.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000025.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000026.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000027.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk Forwards/Bastion_WF_000028.obj", 0.04172f));

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

		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Hit/Bastion_Hit_000001.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Hit/Bastion_Hit_000004.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Hit/Bastion_Hit_000008.obj", 0.054f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Hit/Bastion_Hit_000012.obj", 0.054f));

		attack->SetInfiniteLoop(true);
		aniAttack.animation = attack;
		aniAttack.index = GetAnimationCount() - 1;
	}
}

// attack function
void cnz::Bastion::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

// update function
void cnz::Bastion::Update(float dt)
{
	Enemy::Update(dt);
}

