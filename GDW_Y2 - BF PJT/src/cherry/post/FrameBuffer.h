/*
 * Team: Bonus Fruit:
 * Description: frame buffer for post processing effects
 * References:
	* Intermediate Computer Graphics
		* florp::graphics::IGraphicsResource
		* FrameBuffer
	* https://learnopengl.com/Advanced-OpenGL/Framebuffers
	* https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
	* https://learnopengl.com/In-Practice/2D-Game/Postprocessing
	* https://www.khronos.org/opengl/wiki/Buffer_Object
	* https://www.khronos.org/opengl/wiki/Buffer_Texture
	* https://www.khronos.org/opengl/wiki/Renderbuffer_Object
 */

#pragma once

#include <toolkit/EnumToString.h>
#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <unordered_map>

// #include "..\TextureSampler.h" // included in texture 2d
#include "..\IGraphicsResource.h"
#include "..\textures/Texture2D.h"


namespace cherry
{
	// colour formats
	ENUM(RenderTargetAttachment, uint32_t,
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Color4 = GL_COLOR_ATTACHMENT4,
		Color5 = GL_COLOR_ATTACHMENT5,
		Color6 = GL_COLOR_ATTACHMENT6,
		Color7 = GL_COLOR_ATTACHMENT7,
		Depth = GL_DEPTH_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT
	);

	// checks for colour target attachment
	constexpr bool IsColorAttachment(RenderTargetAttachment attachment) {
		return attachment >= RenderTargetAttachment::Color0 && attachment <= RenderTargetAttachment::Color7;
	}

	// this is a more restricted version of what is in texture 2D.
	// extras are marked with an EX prefix.
	ENUM(RenderTargetType, uint32_t,
		Color32 = GL_RGBA8,
		Color24 = GL_RGB8,
		Color16 = GL_RG8,
		Color8 = GL_R8,
		DepthStencil = GL_DEPTH24_STENCIL8,
		Depth16 = GL_DEPTH_COMPONENT16,
		Depth24 = GL_DEPTH_COMPONENT24,
		Depth32 = GL_DEPTH_COMPONENT32,
		Stencil4 = GL_STENCIL_INDEX4,
		Stencil8 = GL_STENCIL_INDEX8,
		Stencil16 = GL_STENCIL_INDEX16,

		// EX_RED = GL_RED,
		// EX_GREEN = GL_GREEN,
		// EX_BLUE = GL_BLUE,
		// EX_Color32F = GL_RGBA32F,
		// EX_Color16F = GL_RGBA16F,
		Default = 0
	);

	ENUM_FLAGS(RenderTargetBinding, GLenum,
		None = 0,
		Draw = GL_DRAW_FRAMEBUFFER,
		Write = GL_DRAW_FRAMEBUFFER,
		Read = GL_READ_FRAMEBUFFER,
		Both = GL_FRAMEBUFFER
	);

	ENUM_FLAGS(BufferFlags, GLenum,
		None = 0,
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT,
		All = Color | Depth | Stencil
	);

	// if set to true it uses the render buffer.
	struct RenderBufferDesc {
		// If this is set to true, we will generate an OpenGL texture instead of a render buffer
		
		// if 'true', then the post-processing effect is made using a render buffer. If false, then it's made as a texture.
		// when not sampling from the produced image, a render buffer is preferred.
		// see https://www.khronos.org/opengl/wiki/Renderbuffer_Object for more details.
		bool ShaderReadable;

		// size of the renderable
		uint32_t Width = 0;
		uint32_t Height = 0;

		RenderTargetType Format = RenderTargetType::Color32;

		// the buffer attachment
		RenderTargetAttachment Attachment;

	};

	// the frame buffer
	class FrameBuffer : public cherry::ITexture
	{
	public:
		GraphicsClass(FrameBuffer);

		// shared pointer
		// typedef std::shared_ptr<FrameBuffer> Sptr;

		// constructor
		FrameBuffer(uint32_t width, uint32_t height, uint8_t numSamples = 1);

		// destructor 
		~FrameBuffer();

		// gets the width
		uint32_t GetWidth() const;

		// gets the height
		uint32_t GetHeight() const;

		// gets the size of the buffer, which is (width, height)
		glm::u32vec2 GetSize() const;

		// gets the image attachment.
		cherry::Texture2D::Sptr GetAttachment(RenderTargetAttachment attachment);

		/*
		 * Resizes this frame buffer to the new dimensions. Note that this will destroy any data currently stored within it,
		 * and invalidate any texture handles that have been retrieved from this frame buffer
		 * @param width The new width of the frame buffer (must be non-zero)
		 * @param height The new height of the frame buffer (must be non-zero)
		*/
		void Resize(uint32_t newWidth, uint32_t newHeight);

		/*
		 * Adds a new attachment to this frame buffer, with the given description. If a layer it already attached to the binding point
		 * given by the description, it will be destroyed
		 * @param desc The descriptor of the buffer that we want to add
		 */
		void AddAttachment(const RenderBufferDesc& desc);

		/*
		 * Validates this FrameBuffer, and returns whether it is in a valid state or not
		 * @returns True if the FrameBuffer is ready for drawing, false if otherwise
		 */
		bool Validate();

		// virtual void Bind(uint32_t slot) override;
		// binding the frame buffer
		virtual void Bind(uint32_t slot);

		// binding a specific attachment
		virtual void Bind(uint32_t slot, RenderTargetAttachment attachment);

		/*
		 * Binds this frame buffer for usage as either a reading buffer, writing buffer, or both
		 * @param bindMode The slot to bind to (default is Draw/Write)
		 */
		void Bind(RenderTargetBinding bindMode = RenderTargetBinding::Draw) const;
		/*
		 * Unbinds this frame buffer from the slot that it is bound to. Note that you should always call UnBind before calling
		 * Bind on another frame buffer with the same parameters
		 */
		void UnBind() const;

		/*
			Blits (copies) the contents of the read framebuffer into the draw framebuffer
			@param srcBounds Region in the source framebuffer to copy from (left, top, right, bottom)
			@param dstBounds Region in the target framebuffer to copy to   (left, top, right, bottom)
			@param buffers   The buffers to copy
		*/
		static void Blit(
			const glm::ivec4& srcBounds, const glm::ivec4& dstBounds,
			BufferFlags flags = BufferFlags::All, cherry::MagFilter filterMode = cherry::MagFilter::Linear);

		/*
		 * Creates a clone of this frame buffer, with the same size and attachments
		 */
		Sptr Clone() const;

		/*
		 * Overrides SetDebug name, so that we can send the name into OpenGL
		 * @param value The new debug name for this object
		 */
		virtual void SetDebugName(const std::string& value) override;

	private:
		// frame buffer dimensions
		uint32_t myWidth, myHeight;

		// The number of samples to use if multi-sampling is enabled.
		uint8_t  myNumSamples;

		// Whether or not this frame buffer is in a valid state
		bool isValid;

		// The current attachment points that this FrameBuffer is bound to
		mutable RenderTargetBinding myBinding;

		// We will store a pointer to another FBO if this one is multisampled
		FrameBuffer::Sptr myUnsampledFrameBuffer;

		// Stores our attachment information for a given render buffer attachment point
		struct RenderBuffer {
			GLuint  RendererID;

			IGraphicsResource::Sptr Resource;
			
			bool IsRenderBuffer;
			RenderBufferDesc Description;

			RenderBuffer() = default;
		};
		// Stores our buffers per attachment point
		std::unordered_map<RenderTargetAttachment, RenderBuffer> myLayers;
		std::vector<RenderTargetAttachment> myDrawBuffers;  // NEW

	protected:
	};
}