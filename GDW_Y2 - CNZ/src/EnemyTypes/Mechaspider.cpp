#include "Mechaspider.h"

// constructor
cnz::Mechaspider::Mechaspider(std::string scene)
	:Enemy("res/objects/enemies/Spider.obj", scene, true)
{
	description = "Mechaspider";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000000.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000001.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000002.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000003.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000004.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000005.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000006.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000007.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000008.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000009.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000010.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000011.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000012.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000013.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000014.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000015.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000016.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000017.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000018.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000019.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000020.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000021.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000022.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000023.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000024.obj", 0.04f));

	this->AddAnimation(&walk);
}

// constructor
cnz::Mechaspider::Mechaspider(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Spider.obj", scene, mat)
{
	description = "Mechaspider";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000000.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000001.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000002.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000003.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000004.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000005.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000006.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000007.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000008.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000009.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000010.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000011.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000012.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000013.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000014.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000015.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000016.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000017.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000018.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000019.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000020.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000021.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000022.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000023.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000024.obj", 0.04f));

	this->AddAnimation(&walk);
}

// copy constructor
cnz::Mechaspider::Mechaspider(const cnz::Mechaspider& enemy) : Enemy(enemy)
{
	description = "Mechaspider";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000000.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000001.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000002.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000003.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000004.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000005.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000006.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000007.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000008.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000009.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000010.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000011.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000012.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000013.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000014.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000015.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000016.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000017.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000018.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000019.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000020.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000021.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000022.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000023.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000024.obj", 0.04f));

	this->AddAnimation(&walk);
}

cnz::Mechaspider::Mechaspider(cnz::Enemy* obj, std::string scene) : Enemy(obj, scene)
{
	description = "Mechaspider";
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000000.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000001.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000002.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000003.obj", 0.04f));
	walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000004.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000005.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000006.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000007.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000008.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000009.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000010.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000011.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000012.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000013.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000014.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000015.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000016.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000017.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000018.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000019.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000020.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000021.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000022.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000023.obj", 0.04f));
	//walk.AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/enemy/Spider_Mine/Walk/Spider_Walk_000024.obj", 0.04f));

	this->AddAnimation(&walk);
}

void cnz::Mechaspider::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Mechaspider::Update(float dt)
{
	Enemy::Update(dt);
}
