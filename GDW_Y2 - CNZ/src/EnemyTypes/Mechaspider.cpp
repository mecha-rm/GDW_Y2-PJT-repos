#include "Mechaspider.h"
#include "..\cherry\physics\PhysicsBody.h"

// emissive properties
// const glm::vec3 cnz::Mechaspider::EMISSIVE_COLOR = glm::vec3(0.41F, 0.431F, 0.9F);
const glm::vec3 cnz::Mechaspider::EMISSIVE_COLOR = glm::vec3(0.835294118F, 0.560784314F, 0.992156863F);
const float cnz::Mechaspider::EMISSIVE_POWER = 1.0F;

// constructor
cnz::Mechaspider::Mechaspider(std::string scene)
	:Enemy("res/objects/enemies/spider.obj", scene, true)
{
	type = cnz::mechaspider;
	description = "Mechaspider";
	SetPoints(3);
	SetSpeedMultiplier(12.5F);

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// constructor
cnz::Mechaspider::Mechaspider(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/spider.obj", scene, mat)
{
	type = cnz::mechaspider;
	description = "Mechaspider";
	
	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// copy and change scene
cnz::Mechaspider::Mechaspider(cnz::Mechaspider* obj, std::string scene) : Enemy(obj, scene)
{
	type = cnz::mechaspider;
	description = "Mechaspider";
}

// copy constructor
cnz::Mechaspider::Mechaspider(const cnz::Mechaspider& enemy) : Enemy(enemy)
{
	type = cnz::mechaspider;
	description = "Mechaspider";
}

// loads animations
void cnz::Mechaspider::LoadAnimations()
{
	// Walk Animation
	{
		cherry::MorphAnimation* walk = new cherry::MorphAnimation();
		AddAnimation(walk);

		// there is no zeroth obj for some reason
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000000.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000001.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000002.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000003.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000004.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000005.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000006.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000007.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000008.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000009.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000010.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000011.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000012.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000013.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000014.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000015.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000016.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000017.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000018.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000019.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000020.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000021.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000022.obj", 0.04f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000023.obj", 0.04f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Walk/Spider_Walk_000024.obj", 0.04f));

		walk->SetInfiniteLoop(true);
		aniWalk.animation = walk;
		aniWalk.index = GetAnimationCount() - 1;
	}


	// Attack Animation 
	{
		cherry::MorphAnimation* attack = new cherry::MorphAnimation();
		AddAnimation(attack);

		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Burrow/Spider_Plant_000001.obj", 0.025f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Burrow/Spider_Plant_000005.obj", 0.025f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Burrow/Spider_Plant_000009.obj", 0.025f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider Mine/Burrow/Spider_Plant_000013.obj", 0.025f));

		attack->SetInfiniteLoop(true);
		aniAttack.animation = attack;
		aniAttack.index = GetAnimationCount() - 1;
	}
}


void cnz::Mechaspider::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Mechaspider::Update(float dt)
{
	Enemy::Update(dt);
}