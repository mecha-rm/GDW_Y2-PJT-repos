// struct to streamline loading animation. It is not used.
#pragma once
#include <cherry\animate\MorphAnimation.h>

// animation loader
namespace cnz
{
	struct AnimStruct { // easier to batch add frames with a morphanim this way.
		int numFrames;
		std::string basePath;
		float animTime;
		cherry::MorphAnimation* anim;
	};

	// loads animation
	cnz::AnimStruct& LoadAnimation(AnimStruct& anim);
}