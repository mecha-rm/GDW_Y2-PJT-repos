#include "../Enemies.h"
#pragma once

//Oracle class
namespace cnz {
	class Oracle : public cnz::Enemies
	{
	public:
		Oracle(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) {};
	};
}

