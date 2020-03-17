#include "Marauder.h"

// constructor
cnz::Marauder::Marauder(std::string scene) 
	: Enemy("res/objects/enemies/Enemy_Sword.obj", scene, true)
{
	description = "Marauder";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000000.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000001.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000002.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000003.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000004.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000005.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000006.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000007.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000008.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000009.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000010.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000011.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000012.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000013.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000014.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000015.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000016.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000017.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000018.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000019.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000020.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000021.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000022.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000023.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000024.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000025.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000026.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000027.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000028.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000029.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000030.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000031.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000032.obj", 0.04181f));

	this->AddAnimation(&walk);
}

cnz::Marauder::Marauder(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Enemy_Sword.obj", scene, mat)
{
	description = "Marauder";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000000.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000001.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000002.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000003.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000004.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000005.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000006.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000007.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000008.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000009.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000010.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000011.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000012.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000013.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000014.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000015.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000016.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000017.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000018.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000019.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000020.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000021.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000022.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000023.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000024.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000025.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000026.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000027.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000028.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000029.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000030.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000031.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000032.obj", 0.04181f));

	this->AddAnimation(&walk);
}


cnz::Marauder::Marauder(cnz::Enemy* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Marauder";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000000.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000001.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000002.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000003.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000004.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000005.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000006.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000007.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000008.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000009.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000010.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000011.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000012.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000013.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000014.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000015.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000016.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000017.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000018.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000019.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000020.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000021.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000022.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000023.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000024.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000025.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000026.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000027.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000028.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000029.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000030.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000031.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000032.obj", 0.04181f));

	this->AddAnimation(&walk);
}

// copy constructor
cnz::Marauder::Marauder(const cnz::Marauder& enemy) : Enemy(enemy)
{
	description = "Marauder";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000000.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000001.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000002.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000003.obj", 0.04181f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000004.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000005.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000006.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000007.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000008.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000009.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000010.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000011.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000012.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000013.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000014.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000015.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000016.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000017.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000018.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000019.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000020.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000021.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000022.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000023.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000024.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000025.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000026.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000027.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000028.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000029.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000030.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000031.obj", 0.04181f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Marauder/Mar_Walk_Forward/Mar_WF_000032.obj", 0.04181f));

	this->AddAnimation(&walk);
}
void cnz::Marauder::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Marauder::Update(float dt)
{
	Enemy::Update(dt);
}
