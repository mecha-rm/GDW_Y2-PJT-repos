#include "../Enemies.h"
#pragma once

//Marauder class
namespace cnz {
	class Marauder : public cnz::Enemies
	{
	public:
		Marauder(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) {};
	};
}

