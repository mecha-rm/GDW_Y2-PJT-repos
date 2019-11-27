#pragma once
#include "cherry/objects/Object.h"

class Projectile : public cherry::Object
{
public:
	Projectile(cherry::Object* arrow);

private:
	cherry::Object* arrow;
};

