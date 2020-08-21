#include "Sentry.h"
#include "..\cherry\physics\PhysicsBody.h"

// setting emissive values
// const glm::vec3 cnz::Sentry::EMISSIVE_COLOR = glm::vec3(0.811F, 0.3231F, 0.421F);
const glm::vec3 cnz::Sentry::EMISSIVE_COLOR = glm::vec3(0.960784314F, 0.980392157F, 0.352941176F);
const float cnz::Sentry::EMISSIVE_POWER = 1.0F;

// enemy
cnz::Sentry::Sentry(std::string scene)
	: Enemy("res/objects/enemies/Sentry.obj", scene, true)
{
	type = cnz::sentry;
	description = "Sentry";
	SetPoints(2);
	SetSpeedMultiplier(10.8F);

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving the primary physics body.

	LoadAnimations();
}

// constructor
cnz::Sentry::Sentry(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Sentry.obj", scene, mat)
{
	type = cnz::sentry;
	description = "Sentry";

	SetEmissiveColor(EMISSIVE_COLOR);
	SetEmissivePower(EMISSIVE_POWER);

	// physics body
	cherry::PhysicsBody* pb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 1), GetPBodySize());
	AddPhysicsBody(pb);
	primaryBody = pb; // saving as primary physics body

	LoadAnimations();
}

// copy constructor
cnz::Sentry::Sentry(const cnz::Sentry& sentry)
	: Enemy(sentry)
{
	type = cnz::sentry;
	description = "Sentry";
}

// copies the enemy and replaces its scene.
cnz::Sentry::Sentry(const cnz::Sentry* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	type = cnz::sentry;
	description = "Sentry";
}

// load aniamtions
void cnz::Sentry::LoadAnimations()
{
	{
		cherry::MorphAnimation* walk = new cherry::MorphAnimation();
		AddAnimation(walk);

		// there is no zeroth obj for some reason
		// walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000000.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000001.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000002.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000003.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000004.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000005.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000006.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000007.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000008.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000009.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000010.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000011.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000012.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000013.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000014.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000015.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000016.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000017.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000018.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000019.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000020.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000021.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000022.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000023.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000024.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000025.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000026.obj", 0.04034f));
		//walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000027.obj", 0.04034f));
		walk->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Walk Forward/Sentry_WF_000028.obj", 0.04034f));

		aniWalk.animation = walk;
		aniWalk.index = GetAnimationCount() - 1;
	}

	// Attack Animation
	{
		cherry::MorphAnimation* attack = new cherry::MorphAnimation();
		AddAnimation(attack);

		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Attack/Sentry_Attack_000001.obj", 0.075f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Attack/Sentry_Attack_000012.obj", 0.075f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Attack/Sentry_Attack_000024.obj", 0.075f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Attack/Sentry_Attack_000036.obj", 0.075f));
		attack->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Sentry/Attack/Sentry_Attack_000042.obj", 0.075f));

		aniAttack.animation = attack;
		aniAttack.index = GetAnimationCount() - 1;
	}
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

