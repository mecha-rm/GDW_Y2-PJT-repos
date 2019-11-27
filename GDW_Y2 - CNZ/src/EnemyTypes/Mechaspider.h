#include "../Enemies.h"
#pragma once

//Mechaspider class
namespace cnz {
	class Mechaspider : public cnz::Enemies
	{
	public:
		Mechaspider(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) {};
	};
}

