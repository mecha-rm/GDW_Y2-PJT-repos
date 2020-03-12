#include "KernelLayer.h"

#include "..\Game.h"
#include "..\utils/math/Matrix.h"

// KERNEL
cherry::KernelLayer::KernelLayer()
{
	// gets the window size
	glm::ivec2 windowSize = Game::GetRunningGame()->GetWindowSize();

	// shader
	shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_LIGHTING_FS);

	// frame buffer
	frameBuffer = std::make_shared<FrameBuffer>(windowSize.x, windowSize.y);

	// buffer color
	RenderBufferDesc bufferColor = RenderBufferDesc();
	bufferColor.ShaderReadable = true;
	bufferColor.Attachment = RenderTargetAttachment::Color0;
	bufferColor.Format = RenderTargetType::Color24; // loads with RGB

	// buffer depth
	RenderBufferDesc bufferDepth = RenderBufferDesc();
	bufferDepth.ShaderReadable = true;
	bufferDepth.Attachment = RenderTargetAttachment::Depth;
	bufferDepth.Format = RenderTargetType::Depth24;

	// frame buffer
	frameBuffer->AddAttachment(bufferColor);
	frameBuffer->AddAttachment(bufferDepth);

	// makes the layer
	layer = std::make_shared<PostLayer>(shader, frameBuffer);
}

// uses shader and buffer
cherry::KernelLayer::KernelLayer(Shader::Sptr a_Shader, FrameBuffer::Sptr a_FrameBuffer)
{
	shader = a_Shader;
	frameBuffer = a_FrameBuffer;
	layer = std::make_shared<PostLayer>(shader, frameBuffer);
}

// vertex shader, fragment shader, and default buffer
cherry::KernelLayer::KernelLayer(std::string vs, std::string fs)
{
	// gets the window size
	glm::ivec2 windowSize = Game::GetRunningGame()->GetWindowSize();

	// shader
	shader = std::make_shared<Shader>();
	shader->Load(vs.c_str(), fs.c_str());

	// frame buffer
	frameBuffer = std::make_shared<FrameBuffer>(windowSize.x, windowSize.y);

	// buffer color
	RenderBufferDesc bufferColor = RenderBufferDesc();
	bufferColor.ShaderReadable = true;
	bufferColor.Attachment = RenderTargetAttachment::Color0;
	bufferColor.Format = RenderTargetType::Color24; // loads with RGB

	// buffer depth
	RenderBufferDesc bufferDepth = RenderBufferDesc();
	bufferDepth.ShaderReadable = true;
	bufferDepth.Attachment = RenderTargetAttachment::Depth;
	bufferDepth.Format = RenderTargetType::Depth24;

	// frame buffer
	frameBuffer->AddAttachment(bufferColor);
	frameBuffer->AddAttachment(bufferDepth);

	layer = std::make_shared<PostLayer>(shader, frameBuffer);
}


// vertex shader, fragment shader, and frame buffer
cherry::KernelLayer::KernelLayer(std::string vs, std::string fs, FrameBuffer::Sptr buffer)
{
	// shader
	shader = std::make_shared<Shader>();
	shader->Load(vs.c_str(), fs.c_str());

	frameBuffer = buffer;
	layer = std::make_shared<PostLayer>(shader, frameBuffer);
}


// gets the post layer
cherry::PostLayer::Sptr cherry::KernelLayer::GetPostLayer() const { return layer; }


/// KERNEL 3 ///
// constructor
cherry::Kernel3Layer::Kernel3Layer()
{
	// identity matrix
	kernel3 = glm::mat3(
		0, 0, 0,
		0, 1, 0,
		0, 0, 0
	);

	shader->SetUniform("a_Kernel", kernel3);
}


cherry::Kernel3Layer::Kernel3Layer(glm::mat3& kernel)
	: KernelLayer(), kernel3(kernel)
{
}

// gets the matrix
glm::mat3 cherry::Kernel3Layer::GetMatrix() const { return kernel3; }

// sets the matrix
void cherry::Kernel3Layer::SetMatrix(glm::mat3& kernel)
{
	kernel3 = kernel;
	shader->SetUniform("a_Kernel", kernel3);
}

// to string function
std::string cherry::Kernel3Layer::ToString() const
{
	// this has a tostring function already
	util::math::Mat3 temp(
		kernel3[0][0], kernel3[0][1], kernel3[0][2],
		kernel3[1][0], kernel3[1][1], kernel3[1][2],
		kernel3[2][0], kernel3[2][1], kernel3[2][2]
	);

	return temp.toString();
}

/// KERNEL 5 ///
// constructor
cherry::Kernel5Layer::Kernel5Layer() : KernelLayer(POST_VS, POST_KERNEL5_FS)
{
	// matrix is already set to (0, 0)
	kernel5[2][2] = 1.0F;

	shader->SetUniform("a_Kernel[2][2]", kernel5[2][2]);
}


// receives a 5x5 matrix since there is no mat5
cherry::Kernel5Layer::Kernel5Layer(float m[5][5])
	: KernelLayer(POST_VS, POST_KERNEL5_FS)
{
	// sets the matrix
	SetMatrix(m);
}

// kernel layer
cherry::Kernel5Layer::Kernel5Layer(
	float r00, float r01, float r02, float r03, float r04, 
	float r10, float r11, float r12, float r13, float r14, 
	float r20, float r21, float r22, float r23, float r24, 
	float r30, float r31, float r32, float r33, float r34, 
	float r40, float r41, float r42, float r43, float r44)
	: KernelLayer(POST_VS, POST_KERNEL5_FS)
{
	float m[5][5]{
		{r00, r10, r20, r30, r40},
		{r01, r11, r21, r31, r41},
		{r02, r12, r22, r32, r42},
		{r00, r10, r23, r33, r43},
		{r00, r10, r24, r34, r44}
	};

	// sets the matrix
	SetMatrix(m);
}

// gets the matrix
float ** cherry::Kernel5Layer::GetMatrix() const 
{ 
	// creating the matrix
	float** m = new float*[5];

	// column 0
	m[0] = new float[5]{
		kernel5[0][0], kernel5[1][0], kernel5[2][0], kernel5[3][0], kernel5[4][0]
	}; 
	
	// column 1
	m[1] = new float[5]{
		kernel5[0][1], kernel5[1][1], kernel5[2][1], kernel5[3][1], kernel5[4][1]
	};

	// column 2
	m[2] = new float[5]{
		kernel5[0][2], kernel5[1][2], kernel5[2][2], kernel5[3][2], kernel5[4][2]
	};

	// column 3
	m[3] = new float[5]{
		kernel5[0][3], kernel5[1][3], kernel5[2][3], kernel5[3][3], kernel5[4][3]
	};

	// column 4
	m[4] = new float[5]{
		kernel5[0][4], kernel5[1][4], kernel5[2][4], kernel5[3][4], kernel5[4][4]
	};

	return m;
}

// sets the matrix
void cherry::Kernel5Layer::SetMatrix(float m[5][5])
{
	// size_t tst = sizeof(m); // returns 8
	// tst = sizeof(m[0]); // returns 20
	// tst = sizeof(m) * sizeof(m[0]); // 160

	// int r = sizeof m / sizeof m[0];
	// int c = sizeof m[0] / sizeof(float);
	//sizeof(arr[0]) returns the sizeof(data_type) * amount of values
	//sizeof(arr) returns

	// copying the data
	memcpy(kernel5, m, sizeof(float) * 5 * 5);

	// row 0
	shader->SetUniform("a_Kernel[0][0]", kernel5[0][0]);
	shader->SetUniform("a_Kernel[0][1]", kernel5[0][1]);
	shader->SetUniform("a_Kernel[0][2]", kernel5[0][2]);
	shader->SetUniform("a_Kernel[0][3]", kernel5[0][3]);
	shader->SetUniform("a_Kernel[0][4]", kernel5[0][4]);

	// row 1
	shader->SetUniform("a_Kernel[1][0]", kernel5[1][0]);
	shader->SetUniform("a_Kernel[1][1]", kernel5[1][1]);
	shader->SetUniform("a_Kernel[1][2]", kernel5[1][2]);
	shader->SetUniform("a_Kernel[1][3]", kernel5[1][3]);
	shader->SetUniform("a_Kernel[1][4]", kernel5[1][4]);

	// row 2
	shader->SetUniform("a_Kernel[2][0]", kernel5[2][0]);
	shader->SetUniform("a_Kernel[2][1]", kernel5[2][1]);
	shader->SetUniform("a_Kernel[2][2]", kernel5[2][2]);
	shader->SetUniform("a_Kernel[2][3]", kernel5[2][3]);
	shader->SetUniform("a_Kernel[2][4]", kernel5[2][4]);

	// row 3
	shader->SetUniform("a_Kernel[3][0]", kernel5[3][0]);
	shader->SetUniform("a_Kernel[3][1]", kernel5[3][1]);
	shader->SetUniform("a_Kernel[3][2]", kernel5[3][2]);
	shader->SetUniform("a_Kernel[3][3]", kernel5[3][3]);
	shader->SetUniform("a_Kernel[3][4]", kernel5[3][4]);

	// row 4
	shader->SetUniform("a_Kernel[4][0]", kernel5[4][0]);
	shader->SetUniform("a_Kernel[4][1]", kernel5[4][1]);
	shader->SetUniform("a_Kernel[4][2]", kernel5[4][2]);
	shader->SetUniform("a_Kernel[4][3]", kernel5[4][3]);
	shader->SetUniform("a_Kernel[4][4]", kernel5[4][4]);
}

// gets the value
float cherry::Kernel5Layer::GetValue(int row, int col) const
{
	if (row < 0 || row >= 5 || col < 0 || col >= 5)
		return 0;

	return kernel5[row][col];
}

// sets the value
void cherry::Kernel5Layer::SetValue(int row, int col, float value)
{
	if (row < 0 || row >= 5 || col < 0 || col >= 5)
		return;

	kernel5[row][col] = value;
}

// to string function
std::string cherry::Kernel5Layer::ToString() const
{
	std::string str = "";

	for (int i = 0; i < 5; i++)
	{
		std::string row = "(" +
			std::to_string(kernel5[i][0]) + ", " +
			std::to_string(kernel5[i][1]) + ", " +
			std::to_string(kernel5[i][2]) + ", " +
			std::to_string(kernel5[i][3]) + ", " +
			std::to_string(kernel5[i][4]) + ")";

		str += row + "\n";
	}

	// returns the string
	return str;
}
