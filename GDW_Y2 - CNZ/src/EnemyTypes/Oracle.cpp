#include "Oracle.h"
#include "..\cherry\physics\PhysicsBody.h"

// const glm::vec3 cnz::Oracle::EMISSIVE_COLOR = glm::vec3(0.77F, 0.451F, 0.2F);
const glm::vec3 cnz::Oracle::EMISSIVE_COLOR = glm::vec3(0.8F, 0.6F, 1.0F);
const float cnz::Oracle::EMISSIVE_POWER = 1.0F;

// constructor
cnz::Oracle::Oracle(std::string scene)
	: Enemy("res/objects/enemies/Oracle.obj", scene, true)
{
	type = cnz::oracle;
	description = "Oracle";
	SetPoints(1);
	SetSpeedMultiplier(10.5F);

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// constructor
cnz::Oracle::Oracle(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Oracle.obj", scene, mat)
{
	type = cnz::oracle;
	description = "Oracle";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// copy constructor
cnz::Oracle::Oracle(const cnz::Oracle& enemy) : Enemy(enemy)
{
	type = cnz::oracle;
	description = "Oracle";

	// play the walk animation if its available
	if (aniWalk.animation != nullptr)
	{
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}
}

cnz::Oracle::Oracle(cnz::Oracle* obj, std::string scene) : cnz::Enemy(obj, scene) 
{ 
	type = cnz::oracle;
	description = "Oracle"; 

	// play the walk animation if its available
	if (aniWalk.animation != nullptr)
	{
		SetCurrentAnimationByIndex(aniWalk.index);
		aniWalk.animation->Play();
	}
}

void cnz::Oracle::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Oracle::Update(float dt)
{
	Enemy::Update(dt);
}

void cnz::Oracle::LoadAnimations()
{
	// Walk Animation
	{
		cherry::MorphAnimation* walk = new cherry::MorphAnimation();
		AddAnimation(walk);

		// there is no zeroth obj for some reason
		// walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000000.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000001.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000002.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000003.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000004.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000005.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000006.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000007.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000008.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000009.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000010.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000011.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000012.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000013.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000014.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000015.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000016.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000017.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000018.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000019.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000020.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000021.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000022.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000023.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000024.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000025.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000026.obj", 0.04172f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000027.obj", 0.04172f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000028.obj", 0.04172f));

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

		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Attack/Oracle_Attack_000001.obj", 0.0138f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Attack/Oracle_Attack_000008.obj", 0.0138f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Attack/Oracle_Attack_000016.obj", 0.0138f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Attack/Oracle_Attack_000024.obj", 0.0138f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Attack/Oracle_Attack_000032.obj", 0.0138f));

		attack->SetInfiniteLoop(true);
		aniAttack.animation = attack;
		aniAttack.index = GetAnimationCount() - 1;
	}
}
