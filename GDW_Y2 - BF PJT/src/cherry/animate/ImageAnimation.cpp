#include "ImageAnimation.h"

// #include "..\objects\Object.h"
#include "..\objects\Image.h"

#include <fstream>

// TEXTURE ANIMATION
cherry::ImageAnimation::ImageAnimation() : Animation(3) { }

// copying the image animation
cherry::ImageAnimation::ImageAnimation(const ImageAnimation& ani)
	: Animation(ani)
{
	// copies the current index.
	currFrameIndex = ani.currFrameIndex;

	const int VERT_COUNT = ani.object->GetVerticesTotal();

	// copies the poses.
	for (int i = 0; i < ani.poses.size(); i++)
	{
		// creates a new pose.
		Pose newPose;

		// copies the pose.
		newPose.pose = new Vertex[VERT_COUNT];
		memcpy(newPose.pose, ani.poses[i].pose, sizeof(Vertex) * VERT_COUNT);

		// copies the frame.
		newPose.f0 = new ImageAnimationFrame(*ani.poses[i].f0);

		// adds the new pose.
		poses.push_back(newPose);
	}
}

// destructor
cherry::ImageAnimation::~ImageAnimation()
{
	// deletes all poses; the frames get deleted by the parent destructor.
	for (int i = 0; i < poses.size(); i++)
	{
		delete[] poses[i].pose;
	}

	poses.clear();
}

// adds a frame and saves a pose.
bool cherry::ImageAnimation::AddFrame(AnimationFrame* frame)
{
	// return Animation::AddFrame(frame);
	
	// adds frame
	bool added = Animation::AddFrame(frame);

	// the object has not been added yet.
	if (object == nullptr || !added)
		throw std::runtime_error("The object has not been set for this animation. Attach the object before adding frames");

	// gets the object as an image.
	Image* image = (Image*)object;

	ImageAnimationFrame* f0 = (ImageAnimationFrame*)frame;

	// vertices total and the new frame
	const int VERTS_TOTAL = image->GetVerticesTotal();
	Vertex* newVerts = new Vertex[VERTS_TOTAL]; // allocating space for the new set of vertices.

	//// copying the other values.
	//for (int i = 0; i < VERTS_TOTAL; i++)
	//{
	//	newVerts[i].Position = image->GetVertices()[i].Position;
	//	newVerts[i].Color = image->GetVertices()[i].Color;
	//	newVerts[i].Normal = image->GetVertices()[i].Normal;
	//}

	memcpy(newVerts, image->GetVertices(), sizeof(Vertex) * VERTS_TOTAL);

	// the four vertices (guaranteed)
	newVerts[0].UV = f0->GetUVMinimum(); // bottom-left ~ default: (0, 0)
	newVerts[1].UV = glm::vec2(f0->GetUMaximum(), f0->GetVMinimum()); // bottom-right ~ default: (1, 0)

	newVerts[2].UV = glm::vec2(f0->GetUMinimum(), f0->GetVMaximum()); // top-left ~ default: (0, 1)
	newVerts[3].UV = f0->GetUVMaxium(); // top-right ~ default: (1, 1)

	// extra four vertices (only present if front is duplicated on the back)
	if (image->HasDuplicatedFront())
	{

		newVerts[4].UV = newVerts[1].UV; // bottom right (1, 0)
		newVerts[5].UV = newVerts[0].UV; // bottom left (0, 0)

		newVerts[6].UV = newVerts[3].UV; // top right (1, 1)
		newVerts[7].UV = newVerts[2].UV; // top left (0 1)
	}

	Pose newPose;
	newPose.f0 = f0;
	newPose.pose = newVerts;

	// saves the pose
	poses.push_back(newPose);
	
}

// update loop
void cherry::ImageAnimation::Update(float deltaTime)
{
	if (isPlaying() == false || object == nullptr)
		return;

	// Image* image = (Image*)object;
	// cherry::ImageAnimationFrame* currFrame = (ImageAnimationFrame*)GetCurrentFrame();
	// cherry::ImageAnimationFrame* nextFrame = nullptr;
	// 
	// Vertex* newVerts = nullptr; // vertices
	// float vertsTotal = 0;
	float t = 0; // current time
	// 
	// // checks to see what frame to get next.
	// if (GetCurrentFrameIndex() + 1 >= GetFrameCount())
	// 	nextFrame = (ImageAnimationFrame*)GetFrame(0);	
	// else
	// 	nextFrame = (ImageAnimationFrame*)GetFrame(GetCurrentFrameIndex() + 1);
	// 
	// // checks for null values.
	// if (currFrame == nullptr || nextFrame == nullptr)
	// 	return;
	// 
	// // Image img2(nextFrame->GetFilePath(), "", image->IsDoubleSided(), image->HasDuplicatedFront()); // the second  iamge
	// // const Vertex* verts2 = img2.GetVertices();
	// 																									  // Image* img; // the third image
	// // totals up time passed
	// // gets how long the frame has been on screen, divided by how long the frame will stay on screen.
	t = (GetElapsedTime() + deltaTime) / GetCurrentFrame()->GetDelayUnits();

	// if (t >= 1.0F) // time
	if (true) // time
	{
		// t = 1.0F; // capping

		// old
		// // vertices total, and 
		// vertsTotal = image->GetVerticesTotal();
		// newVerts = new Vertex[vertsTotal]; // allocating space for the new set of vertices.
		// 
		// // copying the other values.
		// for (int i = 0; i < vertsTotal; i++)
		// {
		// 	newVerts[i].Position = image->GetVertices()[i].Position;
		// 	newVerts[i].Color = image->GetVertices()[i].Color;
		// 	newVerts[i].Normal = image->GetVertices()[i].Normal;
		// }
		// 
		// // the four vertices (guaranteed)
		// newVerts[0].UV = nextFrame->GetUVMinimum(); // bottom-left ~ default: (0, 0)
		// newVerts[1].UV = glm::vec2(nextFrame->GetUMaximum(), nextFrame->GetVMinimum()); // bottom-right ~ default: (1, 0)
		// 
		// newVerts[2].UV = glm::vec2(nextFrame->GetUMinimum(), nextFrame->GetVMaximum()); // top-left ~ default: (0, 1)
		// newVerts[3].UV = nextFrame->GetUVMaxium(); // top-right ~ default: (1, 1)
		// 	
		// // extra four vertices (only present if front is duplicated on the back)
		// if (image->HasDuplicatedFront())
		// {
		// 
		// 	newVerts[4].UV = newVerts[1].UV; // bottom right (1, 0)
		// 	newVerts[5].UV = newVerts[0].UV; // bottom left (0, 0)
		// 
		// 	newVerts[6].UV = newVerts[3].UV; // top right (1, 1)
		// 	newVerts[7].UV = newVerts[2].UV; // top left (0 1)
		// }
		// 
		// // TODO: check for no image being used. 
		// // if the two images are not the same
		// if (currFrame->GetFilePath() != nextFrame->GetFilePath())
		// {
		// 	Texture2D::Sptr txtr = Texture2D::LoadFromFile(nextFrame->GetFilePath());
		// 	object->GetMaterial()->Set("s_Albedos[0]", txtr, image->GetTextureSampler());
		// 	object->GetMaterial()->Set("s_Albedos[1]", txtr, image->GetTextureSampler());
		// 	object->GetMaterial()->Set("s_Albedos[2]", txtr, image->GetTextureSampler());
		// }
		// 
		// // gets the uvs of the image.
		// object->GetMesh()->Morph(newVerts, vertsTotal); // morphs the vertices.
		// delete[] newVerts;

		// new
		if (currFrameIndex != GetCurrentFrameIndex())
		{
			// updates the current frame.
			currFrameIndex = GetCurrentFrameIndex();
		
			int vertsTotal = object->GetVerticesTotal();

			// TODO: this actually doesn't work, so make sure to fix this.
			ImageAnimationFrame* prevFrame;

			// gets the previous frame
			if (currFrameIndex - 1 < 0)
				prevFrame = (ImageAnimationFrame*)GetFrame(GetFrameCount() - 1);
			else
				prevFrame = (ImageAnimationFrame*)GetFrame(currFrameIndex - 1);

			// = (ImageAnimationFrame*);

			// gets the current frame and pose.
			ImageAnimationFrame* currFrame = (ImageAnimationFrame*)GetCurrentFrame();
			// Pose& nextFrame = poses.at(currIndex);
		
			// if (currFrame->GetFilePath() != nextFrame.f0->GetFilePath())
			if (currFrame->GetFilePath() != prevFrame->GetFilePath())
			{
				Image* image = (Image*)object;
				Texture2D::Sptr txtr = Texture2D::LoadFromFile(prevFrame->GetFilePath());

				object->GetMaterial()->Set("s_Albedos[0]", txtr, image->GetTextureSampler());
				object->GetMaterial()->Set("s_Albedos[1]", txtr, image->GetTextureSampler());
				object->GetMaterial()->Set("s_Albedos[2]", txtr, image->GetTextureSampler());
			}
			
			object->GetMesh()->Morph(poses.at(currFrameIndex).pose, vertsTotal); // morphs the vertices.
		}
	}

	// updates the animation.
	Animation::Update(deltaTime);
}

// toString for image animation
std::string cherry::ImageAnimation::ToString() const
{
	return std::string(
		"ImageAnimation: " + GetName() + " | " +
		"Description: " + GetDescription() + " | " +
		"Tag: " + GetTag()
	);
}

// TEXTURE ANIMATION FRAME
// constructor
cherry::ImageAnimationFrame::ImageAnimationFrame(std::string filePath, glm::vec2 uvMin, glm::vec2 uvMax, float units) :
	ImageAnimationFrame(filePath, cherry::Vec4(uvMin.x, uvMin.y, uvMax.x, uvMax.y))
{
}

cherry::ImageAnimationFrame::ImageAnimationFrame(std::string filePath, cherry::Vec2 uvMin, cherry::Vec2 uvMax, float units) :
	ImageAnimationFrame(filePath, cherry::Vec4(uvMin.v.x, uvMin.v.y, uvMax.v.x, uvMax.v.y))
{

}

cherry::ImageAnimationFrame::ImageAnimationFrame(std::string filePath, glm::vec4 uvs, float units) :
	ImageAnimationFrame(filePath, cherry::Vec4(uvs))
{
}

// uv coordinates as a vec4
cherry::ImageAnimationFrame::ImageAnimationFrame(std::string filePath, cherry::Vec4 uvs, float units)
	: AnimationFrame(units), image(filePath), uvCoords(uvs.v.x, uvs.v.y, uvs.v.z, uvs.v.w)
{
	std::ifstream file(filePath, std::ios::in); // opens the file

	// file access failure check.
	if (!file)
	{
		// #ifndef _DEBUG
		throw std::runtime_error("Error opening image file.");
		// #endif // !DEBUG
		file.close();
		return;
	}
	else // if file opening was successful, it is safe to read from.
	{
	}

	// closes the file since it was only opened for this check.
	file.close();
}

// copy constructor
cherry::ImageAnimationFrame::ImageAnimationFrame(const ImageAnimationFrame& frame)
	: AnimationFrame(frame)
{
	image = frame.image;
	uvCoords = frame.uvCoords;
}

// gets the file path of the image for this frame.
const std::string cherry::ImageAnimationFrame::GetFilePath() const { return image; }

// gets the minimum uv values.
glm::vec2 cherry::ImageAnimationFrame::GetUVMinimum() const { return glm::vec2(uvCoords.x, uvCoords.y); }

// gets the maximum uv values
glm::vec2 cherry::ImageAnimationFrame::GetUVMaxium() const { return glm::vec2(uvCoords.z, uvCoords.w); }

// gets the minimum u coordinate.
float cherry::ImageAnimationFrame::GetUMinimum() const { return uvCoords.x; }

// gets the maximum u-coordinate
float cherry::ImageAnimationFrame::GetUMaximum() const{ return uvCoords.z; }

// gets the minimum v-coordinate.
float cherry::ImageAnimationFrame::GetVMinimum() const { return uvCoords.y; }

// gets the maximum v-coordinate
float cherry::ImageAnimationFrame::GetVMaximum() const { return uvCoords.q; }

// to string function
std::string cherry::ImageAnimationFrame::ToString() const
{
	util::math::Vec4 uvs(uvCoords.x, uvCoords.y, uvCoords.z, uvCoords.w);

	return std::string(
		"Image: " + image + " | " +
		"UVs" + uvs.toString()
	);
}
