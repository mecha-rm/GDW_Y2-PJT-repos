// Animation (Source) - used to run animations
#include "Animation.h"
#include "..\utils\Utils.h"
#include "..\objects\Object.h"

// constructor
cherry::Animation::Animation() {}

// copy constructor
cherry::Animation::Animation(const cherry::Animation& ani)
{
	// descriptors
	name = ani.name;
	description = ani.description;
	tag = ani.tag;

	// deletes frames for this object
	DeleteAllFrames();
	
	// copying frames
	for (AnimationFrame* f : ani.frames)
	{
		switch (ani.GetId()) // id of animation
		{
		case 1: // morph targets
			frames.push_back(new MorphAnimationFrame(*((MorphAnimationFrame*)f)));
			break;
		case 3: // image animation
			frames.push_back(new ImageAnimationFrame(*((ImageAnimationFrame*)f)));
			break;
		}
	}

	// the amount of loops for the animation
	loopsTotal = ani.loopsTotal;

	// tells the animation to loop infinitely.
	infiniteLoop = ani.infiniteLoop;

	// if in reverse mode
	reverse = ani.reverse;

	// same id
	id = ani.id;

	// same object
	object = ani.object;
}

// constructor
cherry::Animation::Animation(int id) : id(id) {}

// deletes all frames.
cherry::Animation::~Animation()
{
	// deletes each animation.
	for (AnimationFrame* frame : frames)
		delete frame;

	// clears out the animations.
	frames.clear();
}

// gets the ID for the animation
int cherry::Animation::GetId() const { return id; }

// gets the object the animation is attachted to
cherry::Object* cherry::Animation::GetObject() const { return object; }

// sets the object this animation is attachted to
void cherry::Animation::SetObject(cherry::Object* obj) { object = obj; }

// gets the name of the animation
std::string cherry::Animation::GetName() const { return name; }

// sets the name
void cherry::Animation::SetName(std::string newName) { name = newName; }

// gets the description of the animation.
std::string cherry::Animation::GetDescription() const { return description; }

// sets the description of the animation
void cherry::Animation::SetDescription(std::string desc) { description = desc; }

// gets the tag for this animation
std::string cherry::Animation::GetTag() const { return tag; }

// sets the tag
void cherry::Animation::SetTag(std::string tag) { this->tag = tag; }

// gets the given frame of animation
cherry::AnimationFrame* cherry::Animation::GetFrame(unsigned int index) const
{
	// out of range
	if (index >= frames.size())
	{
		return nullptr;
	}
	else
	{
		return frames.at(index);
	}
}

// provides a frame based on the tag.
cherry::AnimationFrame* cherry::Animation::GetFrame(std::string tag) const
{
	// looks for the frame based on the provided tag.
	for (int i = 0; i < frames.size(); i++)
	{
		if (frames[i]->GetTag() == tag)
			return frames[i];
	}

	return nullptr;
}

// gets hte current animation frame.
cherry::AnimationFrame* cherry::Animation::GetCurrentFrame() const { return frames.at(currentFrame); }

// returns the index of the current frame.
int cherry::Animation::GetCurrentFrameIndex() const { return (signed)currentFrame; }


// adds a frame to the list
bool cherry::Animation::AddFrame(AnimationFrame* frame) { return util::addToVector(frames, frame); }

// removes the animation frame
// bool cherry::Animation::RemoveFrame(AnimationFrame* frame) { return util::removeFromVector(frames, frame); }

// deletes the frame if it's part of the list
// bool cherry::Animation::DeleteFrame(AnimationFrame* frame)
// {
// 	bool removed = RemoveFrame(frame);
// 
// 	// if removed successfully
// 	if (removed)
// 		delete frame;
// 
// 	return removed;
// }

// clears all frames
// void cherry::Animation::ClearAllFrames() { frames.clear(); }

// deletes all frames
void cherry::Animation::DeleteAllFrames()
{
	for (AnimationFrame* frame : frames)
		delete frame;

	frames.clear();
}

// gets the total amount of frames for the animation
unsigned int cherry::Animation::GetFrameCount() const { return frames.size(); }

// gets the amount of loops for the animation.
unsigned int cherry::Animation::GetLoopsTotal() const { return loopsTotal; }

// total amount of loops
void cherry::Animation::SetLoopsTotal(unsigned int loopAmnt) { loopsTotal = loopAmnt; }

// checks to see if the animation infinitely loops
bool cherry::Animation::IsInfiniteLoop() const { return infiniteLoop; }

// sets whehter to loop the animation infinitely.
void cherry::Animation::SetInfiniteLoop(bool infiLoop) { infiniteLoop = infiLoop; }

// infinite loop toggle
void cherry::Animation::SetInfiniteLoop() { infiniteLoop = !infiniteLoop; }

// gets the amount of finished loops for the animation
unsigned int cherry::Animation::GetFinishedLoops() const { return finishedLoops; }


// returns 'true' if the animation is playing.
bool cherry::Animation::isPlaying() const { return play; }

// plays the game
void cherry::Animation::Play() { play = true; }

// pauses the game
void cherry::Animation::Pause() { play = false; }

// stops the animation, returning it back to its beginning.
void cherry::Animation::Stop()
{
	play = false; // sets play to false
	currentFrame = 0; // returning to the starting frame
	elapsedTime = 0; // restarting the animation.
	finishedLoops = 0; // no finished loops, since the animation has been stopped.
}

// returns 'true' if the animation is playing in reverse.
bool cherry::Animation::IsReversed() const { return reverse; }

// toggles reverse
void cherry::Animation::SetReverse() { reverse = !reverse; }

// sets whether the animation should play in reverse
void cherry::Animation::SetReverse(bool rvs) { reverse = rvs; }


// returns how long hte frame has been on screen.
float cherry::Animation::GetElapsedTime() const { return elapsedTime; }

// time update
void cherry::Animation::Update(float deltaTime)
{
	// TODO: implement reverse
	// if the animation shouldn't play, the update is skipped.
	if (play == false)
		return;

	// increments the time for the animation.
	elapsedTime += deltaTime;

	// if the amount of time passed has exceeded the amount of delay units for the frames.
	if (elapsedTime >= frames.at(currentFrame)->GetDelayUnits())
	{
		// moves onto the next frame
		currentFrame++;
		elapsedTime = 0;

		// reached the end of the animation
		if (currentFrame >= frames.size())
		{
			currentFrame = 0;
			finishedLoops++;

			// if the animation has finished looping, and shouldn't infinitely loop.
			if (infiniteLoop == false && finishedLoops >= loopsTotal)
			{
				Stop(); // stopping the animation
			}

		}
	}

	// updates the frame
	/*if (play && currentFrame < frames.size())
		frames.at(currentFrame)->Update(deltaTime);*/
}


////////////////////////////////////////////////
// Animation Frame
cherry::AnimationFrame::AnimationFrame(float units)
{
	delayUnits = (units < 0) ? 0 : units;
}

// copy constructor
cherry::AnimationFrame::AnimationFrame(const AnimationFrame& frame)
{
	delayUnits = frame.delayUnits;
	tag = frame.tag;
}

// gets the delay until moving onto the next frame.
float cherry::AnimationFrame::GetDelayUnits() const { return delayUnits; }

// sets the amount of delay units
void cherry::AnimationFrame::setDelayUnits(float units)
{
	// if negative, then the frame only used for one render cycle.
	delayUnits = (units < 0) ? 0 : units;
}

// gets the tag
std::string cherry::AnimationFrame::GetTag() const { return tag; }

// sets the tag
void cherry::AnimationFrame::SetTag(std::string newTag) { tag = newTag; }

// update for an animation frame.
//void cherry::AnimationFrame::Update(float deltaTime)
//{
//}
