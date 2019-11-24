#pragma once
#include "Animation.h"
#include "..\Mesh.h"

namespace cherry
{
	// class for morph target animation
	typedef class MorphAnimationFrame : public AnimationFrame
	{
	public:
		// receives the verts to lerp to for this given frame
		// units determines the length of the lerp between the previous frame; default 1 second.
		MorphAnimationFrame(cherry::Vertex * verts, unsigned int vertsTotal, float units = 1.0F);

		// the name of the file for the morph target animation frame; only takes .obj files for now.
		// units determines the length of the lerp between the previous frame; default 1 second.
		MorphAnimationFrame(std::string filePath, float units = 1.0F);

		// gets the current time value for the morph target. The interpolation of the vertices happens in the shader.
		// this gets the current value of 't' (or 'u')
		float GetTime() const;

		// updates the morph target
		void Update(float deltaTime);

	protected:
	
	private:
		// array of vertices
		cherry::Vertex* verticies;

		// total amount of vertices
		unsigned int verticiesTotal;

		// the value of 't'
		float t = 0;

		// TODO: have LERP, SLERP, and other lerp types
		// int lerpType;


	} MorphFrame;
}