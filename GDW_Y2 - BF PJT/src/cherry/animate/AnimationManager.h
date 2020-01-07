// AnimationManager - will be used to manage animations
#pragma once
// #include "Animation.h"
#include "MorphAnimation.h"
#include "ImageAnimation.h"
#include <vector>

namespace cherry
{
	class AnimationManager
	{
	public:
		// animation manager constructor
		AnimationManager();

		// destructor
		~AnimationManager();

		// adds the animation, and makes it the current one
		bool AddAnimation(cherry::Animation * newAni, bool current = false);

		// removes an animation at a given index
		bool RemoveAnimation(unsigned int index);

		// gets the total amount of animations.
		unsigned int GetAnimationsTotal() const;

		// gets the animation at the requested index. If there is no animation at hte current index, then a nullptr is returned.
		cherry::Animation * GetAnimation(unsigned int index);

		// gets the current animation
		// if there is no current animation, a nullptr is returned.
		cherry::Animation * GetCurrentAnimation();

		// sets the index for the animation.
		// if this index is out of range, then a nullptr is returned.
		void SetCurrentAnimation(unsigned int index);

	private:
		std::vector<cherry::Animation *> animations;

		int currentAnimation = 0;
	protected:
	};
}
