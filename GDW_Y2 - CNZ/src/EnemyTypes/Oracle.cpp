#include "Oracle.h"

// constructor
cnz::Oracle::Oracle(std::string scene)
	: Enemy("res/objects/enemies/Enemy_Spear.obj", scene, true)
{
	description = "Oracle";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000000.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000001.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000002.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000003.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000004.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000005.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000006.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000007.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000008.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000009.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000010.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000011.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000012.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000013.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000014.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000015.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000016.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000017.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000018.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000019.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000020.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000021.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000022.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000023.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000024.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000025.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000026.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000027.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000028.obj", 0.04172f));

	this->AddAnimation(&walk);
}

// constructor
cnz::Oracle::Oracle(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Enemy_Spear.obj", scene, mat)
{
	description = "Oracle";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000000.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000001.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000002.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000003.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000004.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000005.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000006.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000007.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000008.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000009.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000010.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000011.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000012.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000013.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000014.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000015.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000016.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000017.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000018.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000019.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000020.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000021.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000022.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000023.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000024.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000025.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000026.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000027.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000028.obj", 0.04172f));

	this->AddAnimation(&walk);
}

// copy constructor
cnz::Oracle::Oracle(const cnz::Oracle& enemy) : Enemy(enemy)
{
	description = "Oracle";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000000.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000001.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000002.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000003.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000004.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000005.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000006.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000007.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000008.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000009.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000010.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000011.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000012.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000013.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000014.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000015.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000016.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000017.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000018.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000019.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000020.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000021.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000022.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000023.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000024.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000025.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000026.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000027.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000028.obj", 0.04172f));

	this->AddAnimation(&walk);
}

cnz::Oracle::Oracle(cnz::Enemy* obj, std::string scene) : cnz::Enemy(obj, scene) 
{ 
	description = "Oracle"; 
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000000.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000001.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000002.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000003.obj", 0.04172f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000004.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000005.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000006.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000007.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000008.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000009.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000010.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000011.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000012.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000013.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000014.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000015.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000016.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000017.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000018.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000019.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000020.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000021.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000022.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000023.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000024.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000025.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000026.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000027.obj", 0.04172f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Oracle/Oracle_Walk_Forward/Oracle_WF_000028.obj", 0.04172f));

	this->AddAnimation(&walk);
}

void cnz::Oracle::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Oracle::Update(float dt)
{
	Enemy::Update(dt);
}
