#pragma once
#include "..\VectorCRY.h"
#include <vector>

namespace cherry
{
	class Object;

	class Path
	{
	public:
		// creates a path with a starting position of (0, 0, 0)
		Path();

		// sets the start position
		Path(cherry::Vec3 startPos);
		
		// returns the interpolation mode
		// [0]: lerp
		// [1]: spline
		short int GetInterpolationMode() const;

		// returns the interpolation mode.
		// [0]: lerp
		// [1]: spline
		void SetInterpolationMode(short int mode);

		// adds a node to the path.
		void AddNode(Vec3 node);

		// adds a node at the provided index. 
		// If the index is greater than the size of the vector by more than 1, it is added to the end of the list. 
		void AddNode(Vec3 node, unsigned int index);

		// adds a node to the path.
		void AddNode(float x, float y, float z);

		// adds a node at the provided index. 
		// If the index is greater than the size of the vector by more than 1, it is added to the end of the list. 
		void AddNode(float x, float y, float z, unsigned int index);

		// removes a node at the provided index.
		// if the index is out of range, no node is removed.
		void RemoveNode(unsigned int index);

		// sets the starting position of the object on the path.
		void SetStartingPoint(Vec3 startPos);

		// gets the incrementer
		float GetIncrementer() const;

		// sets the incremente used for the path.
		// if a negative value is passed, it is converted to its absolute value. Use Reverse() if you wnat to reverse the path.
		void SetIncrementer(float inc);



		// returns if the pathway is reversed.
		bool IsReversed() const;

		// if 'true' is passed, the path is travelled backwards
		void SetReverse(bool rvs);

		// reverses the direction on the path.
		void Reverse();

		// updates the object's placement on the path, returning the new position.
		// the vector returned is the new position for the object along the path.
		// do note that you cannot use this function if there are less than 2 nodes.
		cherry::Vec3 Run(float deltaTime);

		//TODO: add more LERP modes

	private:
		
		short int mode = 0;
		const unsigned int MODES_TOTAL = 2;
		
		// the nodes on the path
		std::vector<cherry::Vec3> nodes;
		int index = 0; // index in 'nodes'

		Vec3 p0; // starting position

		Vec3 p1; // ending position

		float u_inc = 0.1; // the incrementer for the time value.
		float u = 0; // the percentage of the line.
	
		float direc = 1; // 1 is for travelling forward, -1 is travelling backwards

	protected:

		
	};

}