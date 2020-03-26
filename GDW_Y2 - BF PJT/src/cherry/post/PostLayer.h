
// used for post-processing
#pragma once


#include "FrameBuffer.h"
#include "..\Shader.h"
#include "..\Mesh.h"
#include "..\Camera.h"
#include "..\UtilsCRY.h"

namespace cherry
{
	// post-processing layer
	class PostLayer
	{
	public:
		GraphicsClass(PostLayer);

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

		// removes a layer from the post pass list. The shader and frame buffer must match for this to happen.
		// the list can contain multiple instances of the shader and buffer, so only the earliest instance will be removed.
		// since these are shared points, these will not be deleted until all references are gone.
		void RemoveLayer(Shader::Sptr& shader, FrameBuffer::Sptr& buffer);

		// removes a layer form the list based on the index.
		// if the index is out of bounds, nothing happens.
		void RemoveLayer(unsigned int index);

		// returns the shader at the providedi ndex.
		const cherry::Shader::Sptr& GetPassShader(unsigned int index);

		// returns the buffer at the provided index.
		const cherry::FrameBuffer::Sptr& GetPassBuffer(unsigned int index);



		// called when the window is being resized.
		virtual void OnWindowResize(uint32_t width, uint32_t height);

		// renders the post layer
		virtual void PostRender(const cherry::Camera::Sptr& camera);

		// returns a pass shader based on the provided index. Nullptr is returned if the index is out of bounds.
		const cherry::Shader::Sptr& GetShader(unsigned int index) const;

		// returns a pass shader based on the provided index. Nullptr is returned if the index is out of bounds.
		const cherry::FrameBuffer::Sptr& GetFrameBuffer(unsigned int index) const;

		// gets the shader form the last pass.
		const cherry::Shader::Sptr& GetLastPassShader() const;

		// gets the last pas from the buffer.
		const FrameBuffer::Sptr& GetLastPassBuffer() const;
		 
		// the initial frame buffer for this layer's passes
		// if left as a nullptr, it is set to the scene registry's mainbuffer
		FrameBuffer::Sptr initialBuffer = nullptr;

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