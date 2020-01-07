// Morph Animation (Source) -used for morph targets
#include "MorphAnimation.h"
#include "..\objects\Object.h"


// MORPH ANIMATION
// set as animation 1 in the list
cherry::MorphAnimation::MorphAnimation() : Animation(1) {}


// constructor
cherry::MorphAnimation::MorphAnimation(Object* obj)
	: Animation(1)
{
	SetObject(obj); // sets the object
}

// creates the object and generates a mesh type.
void cherry::MorphAnimation::SetObject(cherry::Object* obj)
{
	Animation::SetObject(obj);

	// TODO: add in checker for mesh type

}

// generates the pose and returns it.
cherry::MorphVertex* cherry::MorphAnimation::GeneratePose() const
{
	// the pose
	MorphVertex* pose = new MorphVertex[object->GetVerticesTotal()];
	pose = Mesh::ConvertToMorphVertexArray(object->GetVertices(), object->GetVerticesTotal()); // copies the object

	// const Vertex* verts = object->GetVertices(); // the vertices of the object
	int nextFrameIndex = GetCurrentFrameIndex() + 1; // gets the index of the previous frame.

	MorphAnimationFrame* currFrame = (MorphAnimationFrame*)(GetCurrentFrame()); // the current frame
	MorphAnimationFrame* nextFrame = nullptr; // the next frame

	const Vertex* currPose; // pose for the current frame
	const Vertex* nextPose; // pose for the next frame

	unsigned int valsTtl = object->GetVerticesTotal(); // total amount of values

	if (nextFrameIndex >= GetFrameCount()) // if its the starting frame, then it morphs to the next frame
		nextFrameIndex = 0;
	
	nextFrame = (MorphAnimationFrame*)(GetFrame(nextFrameIndex)); // gets the next frame
	
	// getting the poses.
	currPose = currFrame->GetPose();
	nextPose = nextFrame->GetPose();

	// gets all the values
	for (int i = 0; i < valsTtl; i++)
	{
		int ind1 = i - 1, ind2 = i; // the indexes

		// if the index is out of range, then it lops around
		if (ind1 < 0)
			ind1 = valsTtl - ind1;

		// position
		// pose[i].Position = verts[i].Position;
		pose[i].Position1 = currPose[i].Position;
		pose[i].Position2 = nextPose[i].Position;

		// colour
		// pose[i].Color = verts[i].Color;
		pose[i].Color1 = currPose[i].Color;
		pose[i].Color2 = nextPose[i].Color;

		// normals
		// pose[i].Normal = verts[i].Normal;
		pose[i].Normal1 = currPose[i].Normal;
		pose[i].Normal2 = nextPose[i].Normal;

		// uvs
		// pose[i].UV = verts[i].UV;
		pose[i].UV1 = currPose[i].UV;
		pose[i].UV2 = nextPose[i].UV;
	}

	return pose;
}

// updating hte morphing animation.
void cherry::MorphAnimation::Update(float deltaTime)
{
	// GLuint shader_program;
	// GLuint loc = glGetUniformLocation(shader_program, "time");
	// // sends the time value to the shader in the GPU.
	// glUniform1f(loc, t);
	// // time += 0.5;

	// TODO: fix so that the morphing happens from the second animation
	MorphVertex* morphVerts = nullptr;
	
	if (isPlaying() == false)
		return;

	// totals up time passed
	// gets how long the frame has been on screen, divided by how long the frame will stay on screen.
	t = (GetElapsedTime() + deltaTime) / GetCurrentFrame()->GetDelayUnits();
	
	if (t > 1.0F)
		t = 1.0F;

	// getting the frame of animation
	// TODO: optimize so that a pose isn't generated every frame.
	morphVerts = GeneratePose();
	
	object->GetMesh()->Morph(GeneratePose(), ((MorphAnimationFrame*)(GetCurrentFrame()))->GetValueAmount());
	object->GetMaterial()->GetShader()->SetUniform("a_T", t);
	
	delete[] morphVerts;

	// switches the frame if at the end of the animation.
	Animation::Update(deltaTime);
}


///////////////////////////////
// MORPH ANIMATION FRAME
cherry::MorphAnimationFrame::MorphAnimationFrame(glm::vec3* vertices, glm::vec4 * colors, glm::vec3* normals,
	glm::vec2* uvs, unsigned int valNum, float units)
	: AnimationFrame(units), verticesTotal(valNum)
{
	// the pose, and arrays to the individual values.
	pose = new Vertex[valNum];

	for (int i = 0; i < valNum; i++) // getting all the values.
	{
		pose[i].Position = vertices[i];
		pose[i].Color = colors[i];
		pose[i].Normal = normals[i];
		pose[i].UV = uvs[i];
	}

	// deleting data, since it has been successfully copied.
	delete[] vertices;
	delete[] colors;
	delete[] normals;
	delete[] uvs;
}

// gets the vertices from a regular vertex
cherry::MorphAnimationFrame::MorphAnimationFrame(cherry::Vertex* pose, unsigned int vertsTotal, float units)
	:MorphAnimationFrame(Mesh::ConvertToMorphVertexArray(pose, vertsTotal), vertsTotal, units)
{
}

// recieves vertices and vertices total
cherry::MorphAnimationFrame::MorphAnimationFrame(cherry::MorphVertex * newPose, unsigned int vertsTotal, float units)
	: AnimationFrame(units)
{
	// setting the values
	verticesTotal = vertsTotal;
	pose = new Vertex[vertsTotal];

	// gets the values
	for (int i = 0; i < vertsTotal; i++)
	{
		pose[i].Position = newPose->Position;
		pose[i].Color = newPose->Color;
		pose[i].Normal = newPose->Normal;
		pose[i].UV = newPose->UV;
	}
}

// takes the vertices from an obj file.
cherry::MorphAnimationFrame::MorphAnimationFrame(std::string filePath, float units) : AnimationFrame(units)
{
	// creates an object that reads the file
	Object obj(filePath);
	// Object* obj = new Object(filePath); // TODO: make value variable

	// stores the vertices from the obj file as morph vertices
	const Vertex* tempVerts = obj.GetVertices();
	// MorphVertex* tempVerts;
	verticesTotal = obj.GetVerticesTotal();

	// gets the vertices as morph target vertices
	// tempVerts = Mesh::ConvertToMorphVertexArray(obj.GetVertices(), obj.GetVerticesTotal());

	pose = new Vertex[verticesTotal];

	// gets the values from the morph vertices 
	// TODO: maybe memcpy is more efficient?
	for (int i = 0; i < verticesTotal; i++)
	{
		pose[i].Position = tempVerts[i].Position;
		pose[i].Color = tempVerts[i].Color;
		pose[i].Normal = tempVerts[i].Normal;
		pose[i].UV = tempVerts[i].UV;
	}
}

// destructor.
cherry::MorphAnimationFrame::~MorphAnimationFrame()
{
	delete[] pose; // deleting the pose.
}

// returns the pose
const cherry::Vertex* const cherry::MorphAnimationFrame::GetPose() const { return pose; }

// returns the value amount.
unsigned int cherry::MorphAnimationFrame::GetValueAmount() const { return verticesTotal; }



// gets the current value of 't' (or 'u')
// float cherry::MorphAnimationFrame::GetTime() const { return t / GetDelayUnits(); }

//// updates the morph target
//void cherry::MorphAnimationFrame::Update(float deltaTime)
//{
//	// totals up time passed
//	t += deltaTime;
//
//	
//
//}