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

		// copy constructor. ReplaceAnimations serves the same purpose.
		AnimationManager(const AnimationManager&);

		// destructor
		~AnimationManager();

		// sets the object for all animations in the list.
		void SetObject(Object* object);

		// adds the animation, and makes it the current one
		bool AddAnimation(cherry::Animation * newAni, bool current = false);

		// removes an animation at a given index
		bool RemoveAnimation(unsigned int index);

		// clears out all aniamtions, but does not delete them from memory
		void ClearAllAnimations();

		// deletes all animations
		void DeleteAllAnimations();

		// replaces the animations in this object with copies of the provided manager.
		// remember to set the new object for the animations
		void ReplaceAnimations(const AnimationManager& manager);

		// gets the total amount of animations.
		unsigned int GetAnimationsTotal() const;

		// gets the animation at the requested index. If there is no animation at hte current index, then a nullptr is returned.
		cherry::Animation * GetAnimation(unsigned int index);

		// gets the current animation
		// if there is no current animation, a nullptr is returned.
		cherry::Animation * GetCurrentAnimation();

		// sets the current animation via a pointer to said animation.
		// if said animation isn't found, then the animation is left as what it currently is.
		// if the animation is not in the list, but addAnimation is true, then the animation is added to the list and set as current.
		void SetCurrentAnimation(cherry::Animation* newAni, const bool addAnimation = false);

		// sets the index for the animation.
		// if this index is out of range, then a nullptr is returned.
		void SetCurrentAnimation(unsigned int index);

	private:
		std::vector<cherry::Animation *> animations;

		int currentAnimation = 0;
	protected:
	};
}
