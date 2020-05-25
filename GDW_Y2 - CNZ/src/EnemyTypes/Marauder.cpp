#include "Marauder.h"

// emissive properties
// const glm::vec3 cnz::Marauder::EMISSIVE_COLOR = glm::vec3(0.9F, 0.1231F, 0.5F);
const glm::vec3 cnz::Marauder::EMISSIVE_COLOR = glm::vec3(0.105882353F, 0.164705882F, 1.0F);

const float cnz::Marauder::EMISSIVE_POWER = 1.0F;

// constructor
cnz::Marauder::Marauder(std::string scene) 
	: Enemy("res/objects/enemies/Marauder.obj", scene, true)
{
	description = "Marauder";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	LoadAnimations();
}

cnz::Marauder::Marauder(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Marauder.obj", scene, mat)
{
	description = "Marauder";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	LoadAnimations();
}


cnz::Marauder::Marauder(cnz::Marauder* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Marauder";
	
	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// copy constructor
cnz::Marauder::Marauder(const cnz::Marauder& enemy) : Enemy(enemy)
{
	description = "Marauder";

	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// loads all animations
void cnz::Marauder::LoadAnimations()
{
	walk = new cherry::MorphAnimation();
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

	// moved
	// AddAnimation(walk);
}


void cnz::Marauder::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Marauder::Update(float dt)
{
	Enemy::Update(dt);
}