#include "Bastion.h"

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

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

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

	LoadAnimations();
}

// copying from a pointer
cnz::Bastion::Bastion(cnz::Bastion* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	type = cnz::bastion;
	description = "Bastion";
	
	// getting references to the animations
	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// copy constructor
cnz::Bastion::Bastion(const cnz::Bastion& emy) : cnz::Enemy(emy)
{
	type = cnz::bastion;
	description = "Bastion";

	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// loads all animations
void cnz::Bastion::LoadAnimations()
{
	walk = new cherry::MorphAnimation();
	this->AddAnimation(walk);

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

	// moved
	// this->AddAnimation(walk);
}


void cnz::Bastion::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Bastion::Update(float dt)
{
	Enemy::Update(dt);
}

