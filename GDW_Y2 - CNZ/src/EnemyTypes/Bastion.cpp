#include "Bastion.h"

// TODO: replace with proper file path.
// constructor
cnz::Bastion::Bastion(std::string scene)
	: Enemy("res/objects/weapons/shield.obj", scene, true)
{
	description = "Bastion";

	LoadAnimations();
}

// scene and material
cnz::Bastion::Bastion(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/weapons/shield.obj", scene, mat)
{
	description = "Bastion";

	LoadAnimations();
}

// copying from a pointer
cnz::Bastion::Bastion(cnz::Bastion* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Bastion";
	
	// getting references to the animations
	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// copy constructor
cnz::Bastion::Bastion(const cnz::Bastion& emy) : cnz::Enemy(emy)
{
	description = "Bastion";

	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// loads all animations
void cnz::Bastion::LoadAnimations()
{
	walk = new cherry::MorphAnimation();
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000000.obj", 0.04172f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000001.obj", 0.04172f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000002.obj", 0.04172f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000003.obj", 0.04172f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000004.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000005.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000006.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000007.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000008.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000009.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000010.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000011.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000012.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000013.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000014.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000015.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000016.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000017.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000018.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000019.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000020.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000021.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000022.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000023.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000024.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000025.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000026.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000027.obj", 0.04172f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Bastion/Walk_Forwards/Bastion_WF_000028.obj", 0.04172f));

	this->AddAnimation(walk);
}


void cnz::Bastion::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Bastion::Update(float dt)
{
	Enemy::Update(dt);
}

