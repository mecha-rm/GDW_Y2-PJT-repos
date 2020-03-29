// Morph Animation (Source) -used for morph targets
#include "MorphAnimation.h"
#include "..\objects\Object.h"


// MORPH ANIMATION
// set as animation 1 in the list
cherry::MorphAnimation::MorphAnimation() : Animation(1) {}

cherry::MorphAnimation::MorphAnimation(const MorphAnimation& ani)
	: Animation(ani)
{
	t = ani.t;
}


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
	MorphVertex* pose = nullptr;
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

	// checks for vertex counts being equal
	if (valsTtl != currFrame->GetVertexCount() || valsTtl != nextFrame->GetVertexCount())
	{
		throw std::runtime_error("Error. Vertex count inequality between frames.");
	}

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
	// gets the new pose to be generated.
	MorphVertex* morphVerts = nullptr;
	
	// if no animation is playing, nothing happens.
	if (isPlaying() == false)
		return;

	// totals up time passed
	// gets how long the frame has been on screen, divided by how long the frame will stay on screen.
	t = (GetElapsedTime() + deltaTime) / GetCurrentFrame()->GetDelayUnits();
	
	if (t > 1.0F)
		t = 1.0F;

	// getting the pose
	// TODO: optimize so that a pose isn't generated every frame.
	// it needs to be saved so that the pose gets deleted afterwards.
	morphVerts = GeneratePose();
	
	object->GetMesh()->Morph(morphVerts, ((MorphAnimationFrame*)(GetCurrentFrame()))->GetVertexCount());
	object->GetMaterial()->GetShader()->SetUniform("a_T", t);
	
	// deleting the vertices
	delete[] morphVerts;

	// switches the frame if at the end of the animation.
	Animation::Update(deltaTime);
}

// to string
std::string cherry::MorphAnimation::ToString() const
{
	return std::string(
		"MorphAnimation: " + GetName() + " | " +
		"Description: " + GetDescription() + " | " +
		"Tag: " + GetTag()
	);
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
cherry::MorphAnimationFrame::MorphAnimationFrame(std::string filePath, float units) : AnimationFrame(units), filePath(filePath)
{
	// loads the pose for the frame
	LoadPose();
}

// morph animation frame
cherry::MorphAnimationFrame::MorphAnimationFrame(const MorphAnimationFrame& frame)
	: AnimationFrame(frame)
{
	// copy file path
	filePath = frame.filePath;

	// copying vertex count
	verticesTotal = frame.verticesTotal;

	// copies the pose
	pose = new Vertex[verticesTotal];
	memcpy(pose, frame.pose, sizeof(Vertex) * frame.verticesTotal);
}

// destructor.
cherry::MorphAnimationFrame::~MorphAnimationFrame()
{
	delete[] pose; // deleting the pose.
}

// returns the pose
const cherry::Vertex* const cherry::MorphAnimationFrame::GetPose() const { return pose; }

// returns the value amount.
unsigned int cherry::MorphAnimationFrame::GetVertexCount() const { return verticesTotal; }

// loads in the vertices from a file. This is based on the loadObject code in the Object class.
// it was added in because using the one in the Object class took too long.
void cherry::MorphAnimationFrame::LoadPose()
{
	std::ifstream file; // file
	std::string line = ""; // the current line of the file.

	std::vector<float> tempVecFlt; // a temporary float vector. Used to save the results of a parsing operation.
	std::vector<uint32_t>tempVecUint; // temporary vector for uin32_t data. Saves information from parsing operation.

	// vertex indices
	std::vector<Vertex> vertVec; // a vector of vertices; gets all vertices from the file before putting them in the array.
	std::vector<uint32_t> vertIndices; // a vector of indices; gets all indices from the file before putting them into the array.

	// textures
	std::vector<glm::vec2>vtVec; // temporary vector for vertex vector coordinates; saves values, but doesn't actually get used
	std::vector<unsigned int> textIndices; // a vector of texture indices.

	// normals
	std::vector<glm::vec3>vnVec; // temporary vector for vertex normals; saves values, but doesn't actually get used
	std::vector<unsigned int> normIndices; // vector of vertex normal indices

	file.open(filePath, std::ios::in); // opens file

	// if the file is closed.
	if (!file)
	{
		throw std::runtime_error("File access failure.");
		file.close();
		return;
	}

	// while there are still lines to receive from the file.
	while (std::getline(file, line))
	{
		if (line.length() == 0) // if there was nothing on the line, then it is skipped.
			continue;

		// ignores object name (o), comments (#), and mtl file (mtllib)
		// vertex
		if (line.substr(0, line.find_first_of(" ")) == "v")
		{
			/*
			 * Versions:
			 *** (x, y, z) (version used by Blender)
			 *** (x, y, z, r, g, b)
			 *** (x, y, z, w)
			 *** (x, y, z, w, r, g, b)
			*/
			tempVecFlt = Object::parseStringForTemplate<float>(line); // gets the values from the line

			// checks what version was used.
			switch (tempVecFlt.size())
			{
			case 3: // (x, y, z)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 4: // (x, y, z, w) (n/a) ('w' value is ignored); currently same as case 3.
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 6: // (x, y, z, r, g, b)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[3], tempVecFlt[4], tempVecFlt[5], 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 7: // (x, y, z, w, r, g, b) (n/a) ('w' value is ignored); currently the same as case 6.
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[4], tempVecFlt[5], tempVecFlt[6], 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;
			}
		}
		else if (line.substr(0, line.find_first_of(" ")) == "vt") // Texture UV (u, v); not used for anything
		{
			tempVecFlt = Object::parseStringForTemplate<float>(line); // gets values

			vtVec.push_back(glm::vec2(tempVecFlt[0], tempVecFlt[1])); // saves values
		}
		else if (line.substr(0, line.find_first_of(" ")) == "vn") // Vertex Normals (x, y, z); not used at this stage
		{
			tempVecFlt = Object::parseStringForTemplate<float>(line); // gets the values from the line

			vnVec.push_back(glm::vec3(tempVecFlt[0], tempVecFlt[1], tempVecFlt[2])); // stores them
		}
		// indices
		else if (line.substr(0, line.find_first_of(" ")) == "f")
		{
			// passes the line and replaces all '/' with ' ' so that the string parser can work.
			// format: (face/texture/normal) (shortened to (v1/vt/vn).
			tempVecUint = Object::parseStringForTemplate<uint32_t>(util::replaceSubstring(line, "/", " "));

			// We only need every 1st value in a set, which this loop accounts for.
			for (int i = 0; i < tempVecUint.size(); i += 3)
			{
				// vertex indice/vertex texture indice/vertex normal indice
				// v1/vt1/vn1
				vertIndices.push_back(tempVecUint[i]);
				textIndices.push_back(tempVecUint[i + 1]);
				normIndices.push_back(tempVecUint[i + 2]);
			}

		}
	}

	// file finished
	file.close();

	// vertex array
	{
		// deletes the current pose
		delete[] pose;

		verticesTotal = vertIndices.size(); // gets the total amount of vertices, which is currenty based on the total amount of indices.
		pose = new Vertex[verticesTotal]; // making the dynamic array of vertices

		// puts the vertices into the dynamic vertex buffer array.
		for (int i = 0; i < vertIndices.size(); i++)
			pose[i] = vertVec[vertIndices[i] - 1];		
	}

	// calculating the normals
	{
		// vertex normal Indices and vertex Indices are the same size
		// calculates how many times a given normal is used.
		for (int i = 0; i < normIndices.size(); i++)
		{
			// adding the normal to the vertex
			pose[i].Normal = vnVec.at(normIndices[i] - 1);
		}
	}

	// calculating the UVs
	{
		// vertex normal Indices and vertex Indices are the same size
		// calculates how many times a given normal is used.
		for (int i = 0; i < textIndices.size(); i++)
		{
			// adding the uvs to the designated vertices
			pose[i].UV = vtVec.at(textIndices.at(i) - 1);
		}
	}

	
	return;
}


// to string
std::string cherry::MorphAnimationFrame::ToString() const
{
	return std::string(
		"MorphAnimationFrame - File Path:" + filePath + " | Vertex Count: " + std::to_string(verticesTotal)
	);
}




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