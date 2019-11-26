#include "Path.h"
#include "..\utils\math\Interpolation.h"

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

// updates the object's placement on the path, returning the new position.
cherry::Vec3 cherry::Path::Run(float deltaTime)
{
	cherry::Vec3 p2; // the result of the calcualtion

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
		// the indexes for p0, p2, and p3
		// t0 is for the control point before the start.
		// ind1 is for the control point after the end.
		int t0 = 0, t1 = 0;

		// spline values
		// p0 = index - 2, p1 = index - 1, p2 = index, p3 = index + 1
		t0 = index - 2; //  index - 2 (or nodes.size() - 1)
		t1 = index + 1; // index + 1 

		// if the index is out of range
		if (t0 < 0)
		{
			// offsetting the values
			t0 = nodes.size() + t0;

			// if still less than 0, ind0 is set to 0.
			if (t0 < 0)
				t0 = 0;
			
		}

		// if p3 is greater than the amount of nodes, it's set to 0.
		if (t1 > nodes.size())
			t1 = 0;

		if (direc >= 0) // forward
		{
			p2 = util::math::catmullRom(
				nodes.at(t0).v, p0.v, p1.v, nodes.at(t1).v, u);
		}
		else if (direc < 0) // backward
		{
			p2 = util::math::catmullRom(
				nodes.at(t1).v, p1.v, p0.v, nodes.at(t0).v, u);
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

			if (index > nodes.size()) // bounds
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
	
}
