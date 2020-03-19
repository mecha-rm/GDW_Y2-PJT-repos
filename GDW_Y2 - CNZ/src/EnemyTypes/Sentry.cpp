#include "Sentry.h"

// setting emissive values
const glm::vec3 cnz::Sentry::EMISSIVE_COLOR = glm::vec3(0.811F, 0.3231F, 0.421F);
const float cnz::Sentry::EMISSIVE_POWER = 1.2F;

// enemy
cnz::Sentry::Sentry(std::string scene)
	: Enemy("res/objects/enemies/Sentry.obj", scene, true)
{
	description = "Sentry";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);
	
	LoadAnimations();
}

// constructor
cnz::Sentry::Sentry(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Sentry.obj", scene, mat)
{
	description = "Sentry";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	LoadAnimations();
}

// copy constructor
cnz::Sentry::Sentry(const cnz::Sentry& sentry)
	: Enemy(sentry)
{
	description = "Sentry";

	// getting references to the animations
	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// copies the enemy and replaces its scene.
cnz::Sentry::Sentry(const cnz::Sentry* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Sentry";

	// getting references to the animations
	cherry::AnimationManager& ani = GetAnimationManager();

	walk = (cherry::MorphAnimation*)ani.GetAnimation(0);
}

// load aniamtions
void cnz::Sentry::LoadAnimations()
{
	walk = new cherry::MorphAnimation();
	// walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000000.obj", 0.04034f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000001.obj", 0.04034f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000002.obj", 0.04034f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000003.obj", 0.04034f));
	walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000004.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000005.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000006.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000007.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000008.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000009.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000010.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000011.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000012.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000013.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000014.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000015.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000016.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000017.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000018.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000019.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000020.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000021.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000022.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000023.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000024.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000025.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000026.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000027.obj", 0.04034f));
	//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000028.obj", 0.04034f));

	this->AddAnimation(walk);
}

void cnz::Sentry::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
	arrowDirVec = aimPos - startPos;

	float temp = arrowDirVec.GetX() * arrowDirVec.GetX() + arrowDirVec.GetZ() * arrowDirVec.GetZ() + arrowDirVec.GetZ() * arrowDirVec.GetZ();
	arrowDirVec.SetX(arrowDirVec.GetX() / temp);
	arrowDirVec.SetY(arrowDirVec.GetY() / temp);
	arrowDirVec.SetZ(arrowDirVec.GetZ() / temp);

	arrow->SetPosition(startPos);
	arrow->SetRotation(cherry::Vec3(0, 0, 0), true);
	attacking = true;
}

void cnz::Sentry::Update(float dt)
{
	Enemy::Update(dt);
}

