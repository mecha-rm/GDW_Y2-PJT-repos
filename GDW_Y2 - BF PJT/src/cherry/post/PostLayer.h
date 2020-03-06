
// used for post-processing
#pragma once

#define POST_VS ("res/shaders/post/post.vs.glsl")
#define POST_FS ("res/shaders/post/post.fs.glsl")

#include "FrameBuffer.h"
#include "..\Shader.h"
#include "..\Mesh.h"
#include "..\Camera.h"

namespace cherry
{
	// post-processing layer
	class PostLayer
	{
	public:
		// the vertex shader and fragement shader
		PostLayer();

		// creates a post processing layer, and adds in the first pass by calling AddLayer(string, string)
		PostLayer(const std::string vs, const std::string fs);

		// creates a post processing layer with a provided shader and frame buffer.
		// it is recommended that the frame buffer have a colour and depth attachment.
		// rendering the scene with depth may fail otherwise.
		PostLayer(Shader::Sptr& shader, FrameBuffer::Sptr& output);

		// adds a post processing layer with a default frame buffer.
		void AddLayer(const std::string vs, const std::string fs);

		// adds a layer using the provided shader and frame buffer.
		// colour and depth attachment are suggested for the frame buffer if it is the first pass.
		void AddLayer(Shader::Sptr& shader, FrameBuffer::Sptr& output);

		virtual void OnWindowResize(uint32_t width, uint32_t height);

		// renders the post layer
		virtual void PostRender(const cherry::Camera::Sptr& camera);

	protected:
		// FrameBuffer::Sptr myMainFrameBuffer; // no longer needed

		Mesh::Sptr myFullscreenQuad;
		
		// post processing pass
		struct PostPass {
			Shader::Sptr Shader;
			FrameBuffer::Sptr Output;
		};

		// TODO: maybe make this a queue for algorithms GDW requirements?
		std::vector<PostPass> myPasses;
	};

}