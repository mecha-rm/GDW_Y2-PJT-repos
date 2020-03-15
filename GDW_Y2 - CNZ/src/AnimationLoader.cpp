#include "AnimationLoader.h"

cnz::AnimStruct& cnz::LoadAnimation(AnimStruct& anim)
{
	std::stringstream curPath;
	float curFrameTime;

	curFrameTime = anim.animTime / anim.numFrames;

	for (int frame = 0; frame < anim.numFrames; frame++) {
		curPath = std::stringstream();
		if (frame < 10) {
			curPath << anim.basePath << 0 << frame << ".obj";
		}
		else {
			curPath << anim.basePath << frame << ".obj";
		}
		anim.anim->AddFrame(new cherry::MorphAnimationFrame(curPath.str(), curFrameTime));
	}

	return anim;

	// std::stringstream curPath;
	// float curFrameTime;
	// for (int i = 0; i < animList.size(); i++) {
	// 	curFrameTime = animList[i]->animTime / animList[i]->numFrames;
	// 	for (int frame = 0; frame < animList[i]->numFrames; frame++) {
	// 		curPath = std::stringstream();
	// 		if (frame < 10) {
	// 			curPath << animList[i]->basePath << 0 << frame << ".obj";
	// 		}
	// 		else {
	// 			curPath << animList[i]->basePath << frame << ".obj";
	// 		}
	// 		animList[i]->anim->AddFrame(new cherry::MorphAnimationFrame(curPath.str(), curFrameTime));
	// 	}
	// }
}