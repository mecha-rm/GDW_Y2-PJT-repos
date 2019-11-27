#include "Path.h"
#include "..\utils\math\Interpolation.h"

// creates a path with a starting position of (0, 0, 0)
cherry::Path::Path() : p0(0, 0, 0) {}

// sets the start position
cherry::Path::Path(cherry::Vec3 startPos) : p0(startPos) {}

// gets the interpolation mode
short int cherry::Path::GetInterpolationMode() const { return mode; }

// sets the interpolation mode
void cherry::Path::SetInterpolationMode(short int mode)
{
	this->mode = (mode < 0) ? 0 : (mode >= MODES_TOTAL) ? MODES_TOTAL - 1 : mode;
}

// adds a node to the path
void cherry::Path::AddNode(Vec3 node) { nodes.push_back(node); }

// adds a node at the requested index.
void cherry::Path::AddNode(Vec3 node, unsigned int index)
{
	if (index >= nodes.size())
		nodes.insert(nodes.begin() + index, node);
	else
		nodes.push_back(node);
}

// adds a node
void cherry::Path::AddNode(float x, float y, float z) { AddNode(Vec3(x, y, z)); }

// adds a node at the provided index
void cherry::Path::AddNode(float x, float y, float z, unsigned int index) { AddNode(Vec3(x, y, z), index); }

// removes a node at the provided index.
// if the index is out of range, no node is removed.
void cherry::Path::RemoveNode(unsigned int index)
{
	if (index >= nodes.size())
		return;

	nodes.erase(nodes.begin() + index);
}

// set the starting point
void cherry::Path::SetStartingPoint(Vec3 startPos) { p0 = startPos; }

// gets the incrementer
float cherry::Path::GetIncrementer() const { return u; }

// sets the incrementer
void cherry::Path::SetIncrementer(float inc) { u_inc = abs(inc); }

// if true, the pathway travel is reversed.
bool cherry::Path::IsReversed() const { return (direc < 0 ? true : false); }

// sets if the pathway should be reversed.
void cherry::Path::SetReverse(bool rvs) 
{ 
	direc = (rvs) ? 1 : -1; 
	u = 1.0F - u; // reverses placement
}

// reverses the direction on the path
void cherry::Path::Reverse() { direc = (direc >= 0.0F) ? -1.0F : 1.0F; }

// updates the object's placement on the path, returning the new position.
cherry::Vec3 cherry::Path::Run(float deltaTime)
{
	cherry::Vec3 p2 = p0; // the result of the calcualtion

	// doesn't run if there aren't enough nodes.
	if (nodes.size() < 2)
		return p0;

	// gets the end position
	p1 = nodes.at(index);

	// incrementer for 'u' (or 't') values
	u += u_inc * deltaTime * direc;

	// bounds checking
	u = (u > 1.0F) ? 1.0F : (u < 0.0F) ? 0.0F : u;
	// interpolation modes
	switch (mode)
	{
	case 0: // lerp 
	default:
		p2 = util::math::lerp(p0.v, p1.v, u);
		break;

	case 1: // spline
		// the indexes for pt0, pt2, and pt3
		// pt0 is for the control point before the start.
		// pt1 is the start point. The value of p0 is used instead.
		// pt2 is the point on the line.
		// pt3 is for the control point after the end.
		int pt0 = 0, pt1 = 0, pt2 = 0, pt3 = 0;

		// spline values
		// p0 = index - 2, p1 = index - 1, p2 = index, p3 = index + 1
		// TODO: take out pt1 and pt2
		pt0 = index - 2; //  index - 2 (or nodes.size() - 1)
		pt1 = index - 1;
		pt2 = index;
		pt3 = index + 1; // index + 1 

		// if the index is out of range
		if (pt0 < 0)
		{
			// offsetting the values
			pt0 = nodes.size() + pt0;

			// if still less than 0, ind0 is set to 0.
			if (pt0 < 0)
				pt0 = 0;
		}

		// index out of range for pt1
		if (pt1 < 0)
		{
			// offsetting the values
			pt1 = nodes.size() + pt1;

			// if still less than 0, ind0 is set to 0.
			if (pt1 < 0)
				pt1 = 0;

		}

		// if p3 is greater than the amount of nodes, it's set to 0.
		if (pt3 >= nodes.size())
			pt3 = 0;

		if (direc >= 0) // forward
		{
			p2 = util::math::catmullRom(
				nodes.at(pt0).v, p0.v, p1.v, nodes.at(pt3).v, u);
		}
		else if (direc < 0) // backward
		{
			p2 = util::math::catmullRom(
				nodes.at(pt3).v, p1.v, p0.v, nodes.at(pt0).v, u);
		}


		break;
	}

	// if the endpoint has been reached
	if (p2 == p1)
	{
		// if going forward
		if (direc >= 0.0F)
		{
			index++;

			if (index >= nodes.size()) // bounds
				index = 0;
		}
		else // going backwards
		{
			index--;

			if (index < 0) // bounds
				index = nodes.size() - 1;
		}

		// new values
		p0 = p1;
		p1 = nodes.at(index);
		u = 0;
	}
	
	return p2;
}
