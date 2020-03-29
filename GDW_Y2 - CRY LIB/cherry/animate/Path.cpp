#include "Path.h"
#include "..\utils\math\Interpolation.h"

// total amount of modes
const unsigned int cherry::Path::MODES_TOTAL = 24;

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

// gets speed control
bool cherry::Path::GetSpeedControl() const { return speedControl; }

// sets speed control
void cherry::Path::SetSpeedControl(bool spdCtrl) { speedControl = spdCtrl; }

// adds a node to the path
void cherry::Path::AddNode(Vec3 node) 
{ 
	nodes.push_back(node); // adding the node 
	distances.push_back(Vec2()); // this will receive a proper value when distances are calculated.

	CalculateDistances(nodes.size() - 1); // calculates the distances
}

// adds a node at the requested index.
void cherry::Path::AddNode(Vec3 node, unsigned int index)
{
	if (index < nodes.size()) // adds at a given index
	{
		nodes.insert(nodes.begin() + index, node);
		distances.push_back(Vec2()); // this will receive a proper value when distances are calculated.

		CalculateDistances(index);
	}
	else // end of the list
	{
		nodes.push_back(node);
		distances.push_back(Vec2()); // this will receive a proper value when distances are calculated.

		CalculateDistances(nodes.size() - 1);
	}
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
	distances.erase(distances.begin() + index);

	CalculateDistances(index); // recalculates the distances past this point.
}

// clears all nodes
void cherry::Path::RemoveAllNodes()
{
	nodes.clear(); // clears out all nodes
	distances.clear(); // clears out all distances
}

// edits a node's position
void cherry::Path::EditNode(unsigned int index, Vec3 newPos) 
{
	// if the index is out of bounds, then nothing happens.
	if (index >= nodes.size())
		return;

	// changes the position and recalculates the distances.
	nodes[index] = newPos;
	CalculateDistances(index);

}

// edits an existing node
void cherry::Path::EditNode(unsigned int index, float x, float y, float z) { EditNode(index, Vec3(x, y, z)); }

// gets the amount of nodes
unsigned int cherry::Path::GetNodeCount() const { return nodes.size(); }

// set the starting point
void cherry::Path::SetStartingPoint(Vec3 startPos) { p0 = startPos; }

// gets the incrementer
float cherry::Path::GetIncrementer() const { return u; }

// sets the incrementer
void cherry::Path::SetIncrementer(float inc) { u_inc = abs(inc); }

// gets whether the path is open or closed
bool cherry::Path::IsOpenPath() const { return openPath; }

// sets if the path is open or closed.
void cherry::Path::SetOpenPath(bool open) { this->openPath = open; }

// checks if the path is closed (true), or open (false)
bool cherry::Path::IsClosedPath() const { return !IsOpenPath(); }

// sets if the path should be closed or open.
void cherry::Path::SetClosedPath(bool closed) { SetOpenPath(!closed); }

// returns 'ture' if the path has been finished
bool cherry::Path::AtEndOfPath() const { return (u >= 1.0F && index == 0); }

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

// resets all nodes
void cherry::Path::Reset()
{
	index = 0;
	u = 0;

	// changing the starting position
	if (nodes.size() != 0)
		p0 = nodes[0];
	else
		p0 = cherry::Vec3();
	
	// changing the ending position
	if (nodes.size() != 0)
		p1 = nodes[nodes.size() - 1];
	else
		p1 = cherry::Vec3();
}

// updates the object's placement on the path, returning the new position.
cherry::Vec3 cherry::Path::Run(float deltaTime)
{
	cherry::Vec3 p2 = p0; // the result of the calcualtion
	int pt0 = 0, pt1 = 0, pt2 = 0, pt3 = 0; // used for curves
	float uOrig = 0; // the 'u' value that ranges from 0 to 1. This is used for changing it back when using speed control.

	// doesn't run if there aren't enough nodes.
	if (nodes.size() < 2)
		return p0;

	// gets the end position
	p1 = nodes.at(index);

	// incrementer for 'u' (or 't') values
	u += u_inc * deltaTime * direc;

	// bounds checking
	u = (u > 1.0F) ? 1.0F : (u < 0.0F) ? 0.0F : u;
	uOrig = u;

	// recalculating the value of 'u' for speed control
	if (speedControl && distances.size() != 0)
	{
		float dist = 0.0F;

		if (openPath) // if its an open path, then the total distance used is in the final index
		{
			dist = u * distances[distances.size() - 1].v.y;
		}
		else // if it's a closed path, then the total distance used is saved at the starting index.
		{
			dist = u * distances[0].v.y;
		}

		// finds the point on the path the entity should be
		for (int i = 1; i < distances.size(); i++)
		{
			if (dist <= distances[i].v.y) // if the range the distance falls into has been found.
			{
				// (x_want - x_start) / (x_end - x_start)
				if(i == 1)
					u = dist / distances[i].v.y; // x_start is equal to 0
				else
					u = (dist - distances[i - 1].v.y) / (distances[i].v.y - distances[i - 1].v.y); // calculates the new 'u' value

				// getting the start and end points
				p0 = nodes[i - 1];
				p1 = nodes[i];
				break;

			}
			else if (i == distances.size() - 1) // on final path
			{
				// if there is a loop around to the start.
				// loops around to the start if it is a closed path
				if (!openPath)
				{
					u = (dist - distances[i].v.y) / (distances[0].v.y - distances[i].v.y);
					p0 = nodes[i];
					p1 = nodes[0];
				}

				if (u > 1.0F)
					u = 1.0F;
			}

		}
	}

	// interpolation modes
	switch (mode)
	{
	case 0: // lerp 
	default:
		p2 = util::math::lerp(p0.v, p1.v, u);
		break;

	case 1: // spline
		// the indexes for pt0, pt2, and pt3
		// pt0 is for the control point before the start point.
		// pt1 is the start point. The value of p0 is used instead.
		// pt2 is the point on the line.
		// pt3 is for the control point after the end.
		pt0 = 0;
		pt1 = 0;
		pt2 = 0;
		pt3 = 0;

		// spline values
		// p0 = index - 2, p1 = index - 1, p2 = index, p3 = index + 1
		// TODO: take out pt1 and pt2
		pt0 = index - 2; //  index - 2 (or nodes.size() - 1)
		pt1 = index - 1;
		pt2 = index;
		pt3 = index + 1; // index + 1 (or 0 if looping around to start)

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

		if (direc >= 0) // forward (uses catmull-rom)
		{
			p2 = util::math::catmullRom(
				nodes.at(pt0).v, p0.v, p1.v, nodes.at(pt3).v, u);
		}
		else if (direc < 0) // backward (uses catmull-rom)
		{
			p2 = util::math::catmullRom(
				nodes.at(pt3).v, p1.v, p0.v, nodes.at(pt0).v, u);
		}


		break;

		// Easing
	case 3: // Ease In 1
		p2 = util::math::easeIn1(p0.v, p1.v, u);
		break;
	case 4: // Ease In 2
		p2 = util::math::easeIn2(p0.v, p1.v, u);
		break;
	case 5: // Ease In 3
		p2 = util::math::easeIn3(p0.v, p1.v, u);
		break;

	case 6: // Ease Out 1
		p2 = util::math::easeOut1(p0.v, p1.v, u);
		break;
	case 7: // Ease Out 2
		p2 = util::math::easeOut2(p0.v, p1.v, u);
		break;
	case 8: // Ease Out 3
		p2 = util::math::easeOut3(p0.v, p1.v, u);

	case 9: // Ease In-Out 1
		p2 = util::math::easeInOut1(p0.v, p1.v, u);
		break;
	case 10: // Ease In-Out 2
		p2 = util::math::easeInOut2(p0.v, p1.v, u);
		break;
	case 11: // Ease In-Out 3
		p2 = util::math::easeInOut3(p0.v, p1.v, u);

	case 12: // Ease In-Circular
		p2 = util::math::easeInCircular(p0.v, p1.v, u);
		break;
	case 13: // Ease In-Out Circular
		p2 = util::math::easeOutCircular(p0.v, p1.v, u);
		break;
	case 14: // Ease In-Out Circular
		p2 = util::math::easeInOutCircular(p0.v, p1.v, u);
		break;

	case 15: // Ease In-Bounce 1 
		p2 = util::math::easeInBounce1(p0.v, p1.v, u);
		break;
	case 16: // Ease In-Bounce 2 
		p2 = util::math::easeInBounce2(p0.v, p1.v, u);
		break;
	case 17: // Ease In-Bounce 3
		p2 = util::math::easeInBounce3(p0.v, p1.v, u);
		break;

	case 18: // Ease Out Bounce 1 
		p2 = util::math::easeOutBounce1(p0.v, p1.v, u);
		break;
	case 19: // Ease Out Bounce 2 
		p2 = util::math::easeOutBounce2(p0.v, p1.v, u);
		break;
	case 20: // Ease Out Bounce 3
		p2 = util::math::easeOutBounce3(p0.v, p1.v, u);
		break;

	case 21: // Ease In-Out Bounce 1
		p2 = util::math::easeInOutBounce1(p0.v, p1.v, u);
		break;
	case 22: // Ease In-Out Bounce 2
		p2 = util::math::easeInOutBounce2(p0.v, p1.v, u);
		break;
	case 23: // Ease In-Out Bounce 3
		p2 = util::math::easeInOutBounce3(p0.v, p1.v, u);
		break;
	}

	u = uOrig;
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

		// if the path is open, rather than closed.
		if (openPath && index == 0)
			u = 1.0F; // u is set to '1' so that the object snaps back to the start of the path.
		
	}
	
	return p2;

}

// calculates the pairwise distance and distance along the curve;
// fromIndex determines from which index the values are calculated.
void cherry::Path::CalculateDistances(int fromIndex)
{
	// no nodes, and thus no distances.
	if (nodes.size() == 0)
	{
		distances.clear();
		return;
	}

	// makes sure the amount of distances aligns with the amount of nodes
	if(nodes.size() != distances.size())
		distances.resize(nodes.size());

	// if there is only a single node
	if (nodes.size() == 1)
	{
		distances.at(0) = Vec2();
		return;
	}

	// if the index is out of bounds, then the calculation is skipped over.
	if (fromIndex >= distances.size())
		return;

	// bounds checking
	if (fromIndex <= 1)
	{
		distances.at(0) = Vec2(); // starts off as (0, 0)
		fromIndex = 1; // starts at index 1 since it references the start of the curve
	}
	

	// recalculates distances from the current index
	for (int i = fromIndex; i < distances.size(); i++)
	{
		float pwDist = (nodes.at(i) - nodes.at(i - 1)).GetLength(); // pairwise distance
		float distOnCurve = distances.at(i - 1).v.y + pwDist; // distance on curve
		
		distances.at(i) = Vec2(pwDist, distOnCurve); // calculates the new distances
	}

	// calculates the distance at index (0) as if the path is a complete loop
	distances.at(0).v.x = (nodes.at(0) - nodes.at(nodes.size() - 1)).GetLength();
	distances.at(0).v.y = distances.at(distances.size() - 1).v.y + distances.at(0).v.x;
}
