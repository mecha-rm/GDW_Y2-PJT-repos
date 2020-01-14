// Animation (Header) - used to run animations
// this currently only has morph target animation
#pragma once
#include <vector>
#include <string>
#include "..\Mesh.h"

namespace cherry
{
	// TODO: add AnimationManager
	// forward declarations
	class AnimationFrame;
	class Object;

	// TODO: make abstract class?
	class Animation
	{
	public:
		// animation constructor
		Animation();
		// Animation(bool bones);

		// destructor
		virtual ~Animation();
		
		// animation object
		// 0 = generic/none
		// 1 = morph targets
		// 2 = forward kinematics
		// 3 = texture animation
		int GetId() const;

		// gets the object the animation applies to.
		cherry::Object* GetObject() const; // gets the object the animation applies to.

		// sets the object the animation applies to.
		// set to 'nullptr' to remove the object.
		virtual void SetObject(cherry::Object * obj);

		// gets the name of the animation.
		std::string GetName() const;

		// sets the name of the animation as a cosnt char
		void SetName(std::string newName);

		// gets the description of the animation.
		std::string GetDescription() const;

		// sets the description
		void SetDescription(std::string desc);


		// gets a frame based on an index. Returns nullptr if the index is out of range.
		AnimationFrame * GetFrame(unsigned int index) const;
		
		// gets a frame based on its tag.
		AnimationFrame * GetFrame(std::string tag) const;
		
		// TODO: add GetPreviousFrame and GetNextFrame()?

		// gets the current animation frame.
		AnimationFrame* GetCurrentFrame() const;

		// returns the index of the current frame
		int GetCurrentFrameIndex() const;


		// adds a frame to the list. If 'true' is passed, then the frame was added successfully.
		// the same frame cannot be put into the list twice.
		// TODO: check to make sure it's the right kind of frame.
		bool AddFrame(AnimationFrame * frame);

		// removes the provided frame
		bool RemoveFrame(AnimationFrame* frame);

		// gets the frame count for the animation
		unsigned int GetFrameCount() const;

		// sets the frame the animation should start on.
		// void SetStartingFrame(unsigned int index);

		// gets how many loops are set for the animation.
		unsigned int GetLoopsTotal() const;

		// sets the amount of loops an animation has.
		void SetLoopsTotal(unsigned int loopAmnt);

		// gets whether the animation loops infinitely or not.
		bool IsInfiniteLoop() const;

		// sets whether an animation loops infinitely or not.
		void SetInfiniteLoop(bool infiLoop);

		// toggles the infinite loop on/off.
		void SetInfiniteLoop();

		// returns how many loops have been completed. This only increments if the animation automatically plays more than once.
		unsigned int GetFinishedLoops() const;


		// returns 'true' if the animation is playing.
		// if 'false' is returned, the animation is either paused or stopped.
		bool isPlaying() const;

		// plays the animation
		void Play();

		// pauses the animation; pressing play will continue the animation from where it was paused.
		void Pause();

		// stops the animation; hitting play will make it start over from the beginning.
		void Stop();

		// returns 'true' if the animation is playing in reverse.
		bool IsReversed() const;

		// toggles 'reverse' on and off for the animation.
		void SetReverse();

		// set whether the animation should play in reverse.
		void SetReverse(bool rvs);


		// returns how long this frame has been on screen.
		float GetElapsedTime() const;

		// updates the animation
		virtual void Update(float deltaTime);
		

	private:

		// the name of the animation
		std::string name = "";

		// the description of the anismation
		std::string description = "";
		
		// list of frames
		std::vector<AnimationFrame *> frames;

		// increment for the animation
		float elapsedTime = 0;

		// the index of the current frame
		unsigned int currentFrame = 0;

		// the amount of loops for the animation
		unsigned int loopsTotal = 0;

		// tells the animation to loop infinitely.
		bool infiniteLoop = false;

		// the amount of loops that have been finished for the animation.
		unsigned int finishedLoops = 0;

		// used to play the animation
		bool play = false;
		
		// plays the animation in reverse
		bool reverse = false; 

	protected:
		// for inherited classes to specify their derived type.
		// 0: generic
		// 1: morph target
		// 3: image animation
		Animation(int id);

		int id = 0;

		// sets the object the animation is attached to.
		cherry::Object* object = nullptr;
	};

	// a frame of animation
	class AnimationFrame
	{
	public:
		// creates the animation frame. The amount of 'units' determines how long (in milliseconds) the frame stays on for.
		// a pose can also be set for the animation if you want.
		AnimationFrame(float units = 0);

		// length of time in milliseconds the frame lasts for.
		float GetDelayUnits() const;

		// sets in milliseconds how long the frame lasts for.
		void setDelayUnits(float units);

		// gets the tag
		std::string GetTag() const;

		// sets the tag 
		void SetTag(std::string newTag);


		// update for an animation frame; functionality varies based on type.
		// virtual void Update(float deltaTime);

	private:

		// the pose for the given frame.
		Mesh::Sptr pose;

		float delayUnits = 0; // the delay units (in milliseconds) between this frame and the following frame.

		std::string tag = ""; // the tag for the animation
	
	protected:
	};
}