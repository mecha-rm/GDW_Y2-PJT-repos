#pragma once
#include <vector>
#include <string>

namespace cherry
{
	// AnimationManager
	// vector of animations
	class AnimationFrame;
	class Object;

	class Animation
	{
	public:
		// animation constructor
		Animation();
		// Animation(bool bones);

		// destructor
		~Animation();

		// gets the object the animation applies to.
		cherry::Object* GetObject() const; // gets the object the animation applies to.

		// sets the object the animation applies to.
		// set to 'nullptr' to remove the object.
		void SetObject(cherry::Object * obj);

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

		// adds a frame to the list. If 'true' is passed, then the frame was added successfully.
		// the same frame cannot be put into the list twice.
		bool AddFrame(AnimationFrame * frame);

		// removes the provided frame
		bool RemoveFrame(AnimationFrame* frame);

		// gets the frame count for the animation
		unsigned int GetFrameCount() const;

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

		// updates the animation
		void Update(float deltaTime);
		

	private:
		// sets the object the animation is attached to.
		cherry::Object* object = nullptr;

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
	};

	// a frame of animation
	class AnimationFrame
	{
	public:
		// creates the animation frame. The amount of 'units' determines how long (in milliseconds) the frame stays on for.
		AnimationFrame(float units = 0);

		// length of time in milliseconds the frame lasts for.
		float getDelayUnits() const;

		// sets in milliseconds how long the frame lasts for.
		void setDelayUnits(float units);

		// gets the tag
		std::string GetTag() const;

		// sets the tag 
		void SetTag(std::string newTag);


		// update for an animation frame; functionality varies based on type.
		virtual void Update(float deltaTime);

	private:

		float delayUnits = 0; // the delay units (in milliseconds) between this frame and the following frame.

		std::string tag = "";
	
	protected:
	};
}