#pragma once
#include "cherry/objects/Object.h"
#include "cherry/PhysicsBody.h"

class Projectile : public cherry::Object
{
public:
	Projectile(Projectile* arrow);

	Projectile(std::string modelFile) : Projectile(modelFile, cherry::Vec3()) {}

	// sets the Enemies file, and position
	Projectile(std::string modelFile, cherry::Vec3 pos) : Object(modelFile)
	{
		position = pos;
		// TODO: replace with a more accurate hitbox
		//AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
	}

	void SetDirVec(cherry::Vec3 startPos, cherry::Vec3 endPos);
	void SetDirVec(cherry::Vec3 dirVec);

	cherry::Vec3 GetDirectionVec() { return arrowDirVec; }

	void SetWhichGroup(int i) { whichGroup = i; }
	void SetWhichEnemy(int i) { whichEnemy = i; }
	int GetWhichGroup() { return whichGroup; }
	int GetWhichEnemy() { return whichEnemy; }
	
	bool active = false;
	// set draw pbody
	bool SetDrawPBody(bool draw);

	// get draw pbody
	bool GetDrawPBody();

	// get pbody size
	cherry::Vec3 GetPBodySize();

	// get pbody width, height and depth.
	float GetPBodyWidth();
	float GetPBodyHeight();
	float GetPBodyDepth();

private:
	cherry::Vec3 pBodySize = cherry::Vec3(2, 2, 4);
	bool drawPBody = false;

	cherry::Object* arrow;
	cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
	int whichGroup = -1;
	int whichEnemy = -1;
	
};

