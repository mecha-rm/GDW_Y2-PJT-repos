#include "MorphAnimation.h"
#include "..\objects\Object.h"


// MORPH ANIMATION
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
	pose = Mesh::ConvertToMorphVertexArray(object->GetVertices(), object->GetVerticesTotal());

	const Vertex* verts = object->GetVertices(); // the vertices of the object
	MorphAnimationFrame * currFrame = (MorphAnimationFrame*)(GetCurrentFrame()); // the current frame
	unsigned int valsTtl = object->GetVerticesTotal(); // total amount of values

	

	// gets all the values
	for (int i = 0; i < valsTtl; i++)
	{
		// position
		// pose[i].Position0 = verts[i].Position;
		pose[i].Position1 = currFrame->GetVertices()[i];

		// colour
		// pose[i].Color = verts[i].Color;

		// normals
		// pose[i].Normal0 = verts[i].Normal;
		pose[i].Normal1 = currFrame->GetNormals()[i];

		// uv
		// pose[i].UV = verts[i].UV;
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

	Animation::Update(deltaTime);
	
	if (isPlaying() == false)
		return;

	// totals up time passed
	t += 0.1 * deltaTime;
	
	if (t > 1.0F)
		t = 0;

	object->GetMesh()->Morph(GeneratePose(), ((MorphAnimationFrame*)(GetCurrentFrame()))->GetValueAmount());
	object->GetMaterial()->GetShader()->SetUniform("a_T", t);
}


///////////////////////////////
// MORPH ANIMATION FRAME
cherry::MorphAnimationFrame::MorphAnimationFrame(glm::vec3* vertices, glm::vec3* normals, unsigned int valNum, float units)
	: AnimationFrame(units), vertices(vertices), normals(normals), valNum(valNum)
{
}

// gets the vertices from a regular vertex
cherry::MorphAnimationFrame::MorphAnimationFrame(cherry::Vertex* pose, unsigned int vertsTotal, float units)
	:MorphAnimationFrame(Mesh::ConvertToMorphVertexArray(pose, vertsTotal), vertsTotal, units)
{
}

// recieves vertices and vertices total
cherry::MorphAnimationFrame::MorphAnimationFrame(cherry::MorphVertex * pose, unsigned int vertsTotal, float units)
	: AnimationFrame(units)
{
	// setting the values
	valNum = vertsTotal;
	vertices = new glm::vec3[vertsTotal];
	normals = new glm::vec3[vertsTotal];

	// gets the values
	for (int i = 0; i < vertsTotal; i++)
	{
		vertices[i] = pose->Position0;
		normals[i] = pose->Normal0;
	}
}

// takes the vertices from an obj file.
cherry::MorphAnimationFrame::MorphAnimationFrame(std::string filePath, float units) : AnimationFrame(units)
{
	// creates an object that reads the file
	Object obj(filePath);

	// stores the vertices from the obj file as morph vertices
	const Vertex* tempVerts = obj.GetVertices();
	// MorphVertex* tempVerts;
	valNum = obj.GetVerticesTotal();

	// gets the vertices as morph target vertices
	// tempVerts = Mesh::ConvertToMorphVertexArray(obj.GetVertices(), obj.GetVerticesTotal());

	vertices = new glm::vec3[valNum];
	normals = new glm::vec3[valNum];

// gets the values from the morph vertices
	for (int i = 0; i < valNum; i++)
	{
		vertices[i] = tempVerts[i].Position;
		normals[i] = tempVerts[i].Normal;
	}
}

// gets the vertices
glm::vec3* cherry::MorphAnimationFrame::GetVertices() const { return vertices; }

// gets the normals
glm::vec3* cherry::MorphAnimationFrame::GetNormals() const { return normals; }

// returns the value amount.
unsigned int cherry::MorphAnimationFrame::GetValueAmount() const { return valNum; }



// gets the current value of 't' (or 'u')
// float cherry::MorphAnimationFrame::GetTime() const { return t / getDelayUnits(); }

//// updates the morph target
//void cherry::MorphAnimationFrame::Update(float deltaTime)
//{
//	// totals up time passed
//	t += deltaTime;
//
//	
//
//}