// used for convolution kernels
// see https://en.wikipedia.org/wiki/Kernel_(image_processing) for kernel types
// https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
// https://www.geeksforgeeks.org/multidimensional-arrays-c-cpp/
#pragma once

#include "PostLayer.h"

// identity matrix
#define KERNEL_IDENTITY glm::mat3(0, 0, 0, 0, 1, 0, 0, 0, 0);

// edge detection matrices
#define KERNEL_EDGE_1 glm::mat3(1, 0, -1, 0, 0, 0, -1, 0, 1);
#define KERNEL_EDGE_2 glm::mat3(0, 1, 0, 1, -4, 1, 0, 1, 0);
#define KERNEL_EDGE_3 glm::mat3(-1, -1, -1, -1, 8, -1, -1, -1, -1);

#define KERNEL_SHARPEN glm::mat3(0, -1, 0, -1, 5, -1, 0, -1, 0);

// box blur (1/9 for all components)
#define KERNEL_BOX_BLUR glm::mat3(0.11111111111F)

// gaussian blur (3 x 3)
#define KERNEL_GAUSSIAN_BLUR glm::mat3(0.0625F, 0.125F, 0.0625F, 0.125F, 0.25F, 0.125F, 0.0625F, 0.125F, 0.625F)

namespace cherry
{
	// Standard Kernel
	class KernelLayer
	{
	public:
		// creates shader and buffer
		KernelLayer();

		// uses provided shader and buffer
		KernelLayer(Shader::Sptr a_Shader, FrameBuffer::Sptr a_FrameBuffer);

		// gets the post layer
		cherry::PostLayer::Sptr GetPostLayer() const;

		// to string
		virtual std::string ToString() const = 0;

	private:

		// post layer
		cherry::PostLayer::Sptr layer = nullptr;

	protected:
		// vertex shader and fragement shader (uses default buffer)
		KernelLayer(std::string vs, std::string fs);
		
		// vertex shader, fragement shader, and frame buffer
		KernelLayer(std::string vs, std::string fs, FrameBuffer::Sptr buffer);

		// components
		cherry::Shader::Sptr shader = nullptr;
		cherry::FrameBuffer::Sptr frameBuffer = nullptr;
	};

	// KERNEL 3
	class Kernel3Layer : public KernelLayer
	{
	public:
		// initializes the kernel3 layer with a kernel identity matrix
		Kernel3Layer();

		// creates a kernel layer with the provided mat3
		Kernel3Layer(glm::mat3 kernel);

		// gets the matrix
		glm::mat3 GetMatrix() const;

		// sets the matrix
		void SetMatrix(glm::mat3 kernel);

		// to string
		std::string ToString() const override;

	private:
		glm::mat3 kernel3;

	protected:

	};

	// KERNEL 5
	class Kernel5Layer : public KernelLayer
	{
	public:
		// initializes the kernel5 layer with an empty 5x5 array
		Kernel5Layer();

		// creates a kernel layer.
		// there is no mat5, so a 5x5 matrix is sent instead
		Kernel5Layer(float m[5][5]);

		// setting individual values
		Kernel5Layer(
			float r00, float r01, float r02, float r03, float r04,
			float r10, float r11, float r12, float r13, float r14,
			float r20, float r21, float r22, float r23, float r24,
			float r30, float r31, float r32, float r33, float r34,
			float r40, float r41, float r42, float r43, float r44
		);

		// gets a dynamic array representing the matrix
		// do note that changing this dynamic array does not change the values in this object.
		float ** GetMatrix() const;

		// sets the matrix
		void SetMatrix(float m[5][5]);

		// gets the value; if the index is out of bounds, a 0 is returned.
		float GetValue(int row, int col) const;

		// sets the value; if the index is out of bounds, nothing happens.
		void SetValue(int row, int col, float value);

		// to string
		std::string ToString() const override;

	private:
		// since there's no mat5, a 5x5 matrix is used instead.
		float kernel5[5][5];

	protected:

	};

}
