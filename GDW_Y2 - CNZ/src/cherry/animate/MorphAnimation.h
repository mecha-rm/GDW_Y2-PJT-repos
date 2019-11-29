// Morph Animation (Header) -used for morph targets
#pragma once
#include "Animation.h"
#include "..\Mesh.h"

namespace cherry
{
	typedef class MorphAnimation : public Animation
	{
	public:
		MorphAnimation();

		// the object being used for the morph target animation.
		MorphAnimation(Object* obj);
		
		// creates the object and changes its mesh type.
		void SetObject(cherry::Object* obj);

		// generates a mesh for the current pose, with position1 and normal1 being the targets.
		MorphVertex* GeneratePose() const;

		// updates the morphing animation
		void Update(float deltaTime);

	private:
		// the value of 't'
		float t = 0;

	protected:

	};

	// class for morph target animation
	typedef class MorphAnimationFrame : public AnimationFrame
	{
	public:

		// creates a morph animation frame with the vertices and normals seperated.
		// this takes position 0.
		MorphAnimationFrame(glm::vec3 * vertices, glm::vec3 * normals, unsigned int valNum, float units = 1.0F);

		// receives the verts to lerp to for this given frame, with units determining how long the lerp process takes from the last frame.
		MorphAnimationFrame(cherry::Vertex* pose, unsigned int vertsTotal, float units = 1.0F);

		// receives the verts to lerp to for this given frame
		// units determines the length of the lerp between the previous frame; default 1 second.
		// this takes position1 and normal1 from the provided pose values
		MorphAnimationFrame(cherry::MorphVertex * pose, unsigned int vertsTotal, float units = 1.0F);

		// the name of the file for the morph target animation frame; only takes .obj files for now.
		// units determines the length of the lerp between the previous frame; default 1 second.
		MorphAnimationFrame(std::string filePath, float units = 1.0F);

		// gets the vertices for this frame.
		glm::vec3 * GetVertices() const;

		// gets the normals for this frame
		glm::vec3* GetNormals() const;

		// gets the value amount, which is the same for vertices and 
		unsigned int GetValueAmount() const;

		// gets the current time value for the morph target. The interpolation of the vertices happens in the shader.
		// this gets the current value of 't' (or 'u')
		// float GetTime() const;


		// updates the morph target
		// void Update(float deltaTime);

	protected:
	
	private:
		// the pose for this frame
		// MorphVertex* pose;

		// array of vertices
		glm::vec3 * vertices;

		// array of normals
		glm::vec3* normals;

		// total amount of vertices
		unsigned int valNum;

		// the value of 't'
		// float t = 0;

		// TODO: have LERP, SLERP, and other lerp types
		// int lerpType;


	} MorphFrame;
}