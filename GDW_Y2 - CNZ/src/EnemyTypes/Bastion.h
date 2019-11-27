#include "../Enemies.h"
#pragma once

//Bastion class
namespace cnz {
	class Bastion : public cnz::Enemies
	{
	public:
		Bastion(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) {};
	};
}

