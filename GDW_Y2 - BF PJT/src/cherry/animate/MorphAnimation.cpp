#include "MorphAnimation.h"
#include "..\objects\Object.h"

// recieves vertices and vertices total
cherry::MorphAnimationFrame::MorphAnimationFrame(cherry::Vertex* verts, unsigned int vertsTotal, float units) 
	: AnimationFrame(units), verticies(verts), verticiesTotal(verticiesTotal)
{
}

// takes the vertices from an obj file.
cherry::MorphAnimationFrame::MorphAnimationFrame(std::string filePath, float units) : AnimationFrame(units)
{
	// creates an object that reads the file
	Object obj(filePath);

	// copies the data from the object
	memcpy(verticies, obj.GetVertices(), sizeof(Vertex) * obj.GetVerticesTotal());

	// gets the total amount of vertices
	verticiesTotal = obj.GetVerticesTotal();
}

// gets the current value of 't' (or 'u')
float cherry::MorphAnimationFrame::GetTime() const { return t / getDelayUnits(); }

// updates the morph target
void cherry::MorphAnimationFrame::Update(float deltaTime)
{
	

	// totals up time passed
	t += deltaTime;


}
