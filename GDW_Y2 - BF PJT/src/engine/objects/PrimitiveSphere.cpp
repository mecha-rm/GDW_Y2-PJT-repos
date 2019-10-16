#include "PrimitiveSphere.h"
#include "..\utils\math\Rotation.h"

PrimitiveSphere::PrimitiveSphere(float radius, unsigned int segRows, unsigned int segCols) : Primitive()
{
	// SETUP ANGLES

	// (2) + segRows * segCols
	// Two is added to account for the vertices at the top and bottom of the sphere respectively.
	
	unsigned int index = 1; // the index of the vector
	float segWidth = radius / (float)segCols; // the width of each segment (should be radius / segCols)
	float segHeight = radius / (float)segRows; // the height of each segment (should be radius / segHeight)

	// Position and Color (Default Values)
	// fills each column, then moves to the following row
	vertices = new Vertex[2 * segRows * segCols];

	vertices[0] = { {0.0F, 0.5F, 0.0F}, {1.0F, 1.0F, 1.0F, 1.0F} }; // top vertex
	vertices[2 * segRows * segCols - 1] = { {0.0F, -0.5F, 0.0F}, {1.0F, 1.0F, 1.0F, 1.0F} }; // bottom vertex

	int edge = 1;
	// bool loopBack = false;



	//for (int row = 0; row < segRows; row++) // goes through each row
	//{
	//	for (int col = 0; col < segCols / 2; col++) // goes through each column
	//	{
	//		// starts with vertices at the top of the sphere.

	//		// vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, 0.0F}, {1.0F, 1.0F, 1.0F, 1.0F} };
	//		// vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, 0.0F}, {1.0F, 1.0F, 1.0F, 1.0F} };

	//		vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, -0.5F + segWidth * edge}, {1.0F, 1.0F, 1.0F, 1.0F} };

	//		if(edge != 0 && edge != round((float)col / 2.0F))
	//			vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, -0.5F + segWidth * edge}, {1.0F, 1.0F, 1.0F, 1.0F} };

	//		index++;
	//		edge++;
	//	}
	//}

	


	// rings are verticle portions (i.e. rows)
	// segments are horizontal (i.e. columns)

	
}
