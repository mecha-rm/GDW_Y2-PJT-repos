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
		~Path() = default; // TODO: make nodes pointers

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

		// returns 'true' if using speed control
		bool GetSpeedControl() const;

		// sets whether to used speed control or not.
		void SetSpeedControl(bool spdCtrl);


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

		// removes all nodes from the path
		void RemoveAllNodes();

		// edits a node by changing its position.
		void EditNode(unsigned int index, Vec3 newPos);

		// edits a node's position
		void EditNode(unsigned int index, float x, float y, float z);

		// gets the total amount of nodes for this path.
		unsigned int GetNodeCount() const;


		// sets the starting position of the object on the path.
		void SetStartingPoint(Vec3 startPos);

		// gets the incrementer
		float GetIncrementer() const;

		// sets the incremente used for the path.
		// if a negative value is passed, it is converted to its absolute value. Use Reverse() if you wnat to reverse the path.
		void SetIncrementer(float inc);

		// returns if the path is open (true) or closed (closed)
		// if the path is closed, then the end point interpolates to the starting point.
		// the path is closed by default.
		bool IsOpenPath() const;

		// sets whether the paht is open (true) or closed (false). If the path is closed, then the entity loops aorund the whole path.
		// if the path is open, then the entity snaps to the start of the path once it ends.
		void SetOpenPath(bool open);

		// returns 'true' if the path is closed, false otherwise.
		bool IsClosedPath() const;

		// set to 'true' will close the path, false will have the path stay open.
		// if the path is open, then the entity snaps to the start of the path once it ends.
		void SetClosedPath(bool closed);

		// returns 'true' if the current path has been finished. False otherwise.
		// NOTE: if the path is closed (i.e. the entity loops from start to end), this will always return false.
		bool AtEndOfPath() const;

		// returns if the pathway is reversed.
		bool IsReversed() const;

		// if 'true' is passed, the path is travelled backwards
		void SetReverse(bool rvs);

		// reverses the direction on the path.
		void Reverse();

		// resets the path, putting the entity back at its beginning.
		// the starting position will be set to the first node, and the ending position will be set to the second node.
		// to remove all the nodes, use RemoveAllNodes().
		void Reset();

		// updates the object's placement on the path, returning the new position.
		// the vector returned is the new position for the object along the path.
		// do note that you cannot use this function if there are less than 2 nodes.
		cherry::Vec3 Run(float deltaTime);


	private:

		// calculates the distances along the curve from the current index
		void CalculateDistances(int fromIndex = 0);

		
		short int mode = 0; // interpolation mode
		
		static const unsigned int MODES_TOTAL; // total amount of modes
		
		bool speedControl = false; // used for speed control
		
		bool openPath = false; // sets whether the path is open or closed.

		// the nodes on the path
		std::vector<cherry::Vec3> nodes;
		int index = 0; // index in 'nodes'

		cherry::Vec3 p0; // starting position
		cherry::Vec3 p1; // ending position

		float u_inc = 0.1; // the incrementer for the time value.
		float u = 0; // the percentage of the line.
	
		float direc = 1; // 1 is for travelling forward, -1 is travelling backwards
		

		// calculates the distances for the curve
		// (pairwise distance, distance on curve)
		std::vector<cherry::Vec2> distances;

	protected:

		
	};

}