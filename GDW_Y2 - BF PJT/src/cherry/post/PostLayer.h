
// used for post-processing
#pragma once

#define POST_VS ("res/shaders/post/post.vs.glsl")
#define POST_FS ("res/shaders/post/post.fs.glsl")

#include "FrameBuffer.h"
#include "..\Shader.h"
#include "..\Mesh.h"

namespace cherry
{
	// post-processing layer
	class PostLayer
	{
	public:
		// the vertex shader and fragement shader
		PostLayer() = default;

		PostLayer(const std::string vs, const std::string fs);

		// adds a post processing layer
		// returns 'true' if successful
		void AddLayer(const std::string vs, const std::string fs);

		virtual void OnWindowResize(uint32_t width, uint32_t height);

		// renders the post layer
		virtual void PostRender();

	protected:
		// FrameBuffer::Sptr myMainFrameBuffer; // no longer needed

		Mesh::Sptr myFullscreenQuad;
		
		// post processing pass
		struct PostPass {
			Shader::Sptr Shader;
			FrameBuffer::Sptr Output;
		};

		std::vector<PostPass> myPasses;
	};

}