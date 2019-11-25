#include "../Enemies.h"
#pragma once

//Sentry class
namespace cnz {
	class Sentry : public cnz::Enemies
	{
	public:
		Sentry(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) {};
	};
}

