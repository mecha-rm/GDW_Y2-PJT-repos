#include "FrameBuffer.h"
#include <toolkit/Logging.h>

// constructor
cherry::FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint8_t numSamples)
{
	myWidth = width;
	myHeight = height;
	
	LOG_ASSERT(myWidth > 0, "Width must be greater than zero!");
	LOG_ASSERT(myHeight > 0, "Height must be greater than zero!");

	int maxSamples = 0;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	
	myNumSamples = glm::clamp((int)numSamples, 1, maxSamples);
	isValid = false;

	// creates the frame buffers
	glCreateFramebuffers(1, &myRendererID);

	if (myNumSamples > 1) {
		myUnsampledFrameBuffer = std::make_shared<FrameBuffer>(width, height, 1);
	}
}

// destructor
cherry::FrameBuffer::~FrameBuffer()
{
	LOG_INFO("Deleting frame buffer with ID: {}", myRendererID);
	glDeleteFramebuffers(1, &myRendererID);
}

// returns the width of the buffer.
uint32_t cherry::FrameBuffer::GetWidth() const { return myWidth; }

// returns the height of the buffer.
uint32_t cherry::FrameBuffer::GetHeight() const { return myHeight; }

// gets the size of the buffer
glm::u32vec2 cherry::FrameBuffer::GetSize() const { return { myWidth, myHeight }; }

// gets once of the frame attachments
cherry::Texture2D::Sptr cherry::FrameBuffer::GetAttachment(RenderTargetAttachment attachment) {
	if (myNumSamples > 1) {
		return myUnsampledFrameBuffer->GetAttachment(attachment);
	}
	else {
		if (myLayers.find(attachment) != myLayers.end()) {
			if (!myLayers[attachment].IsRenderBuffer) {
				return std::dynamic_pointer_cast<Texture2D>(myLayers[attachment].Resource);
			}
			else
				return nullptr;
		}
		else
			return nullptr;
	}
}
 
void cherry::FrameBuffer::Resize(uint32_t newWidth, uint32_t newHeight) {
	LOG_ASSERT(newWidth > 0, "Width must be greater than zero!");
	LOG_ASSERT(newHeight > 0, "Height must be greater than zero!");

	if (newWidth != myWidth || newHeight != myHeight) {
		myWidth = newWidth;
		myHeight = newHeight;

		for (auto& kvp : myLayers) {
			AddAttachment(kvp.second.Description);
		}
		Validate();
	}

	if (myNumSamples > 1) {
		myUnsampledFrameBuffer->Resize(newWidth, newHeight);
	}
}

void cherry::FrameBuffer::AddAttachment(const RenderBufferDesc& desc)
{
	// Remove any existing render buffers bound to that slot
	if (myLayers.find(desc.Attachment) != myLayers.end()) {
		LOG_WARN("A target is already bound to slot, deleting existing target");
		if (myLayers[desc.Attachment].IsRenderBuffer)
			glDeleteRenderbuffers(1, &myLayers[desc.Attachment].RendererID);
		else
			myLayers[desc.Attachment].Resource = nullptr;
	}
	// If this is a new attachment, and it's a color, we need to update our DrawBuffers
	else if (desc.Attachment >= RenderTargetAttachment::Color0 && desc.Attachment <= RenderTargetAttachment::Color7) {  // NEW
		myDrawBuffers.push_back(desc.Attachment);
		// when drawing, we're rendering into one of these buffers.
		glNamedFramebufferDrawBuffers(myRendererID, myDrawBuffers.size(), reinterpret_cast<const GLenum*>(myDrawBuffers.data()));
		// static_assert(false); // Note how we now store information on what buffers we will read/draw to
	}

	RenderBuffer& buffer = myLayers[desc.Attachment];
	buffer.Description = desc;
	buffer.IsRenderBuffer = !desc.ShaderReadable;

	// Handling for when we can use renderbuffers instead of textures
	if (buffer.IsRenderBuffer) {
		glCreateRenderbuffers(1, &buffer.RendererID);

		// Enable multisampling on the buffer if required
		if (myNumSamples > 1)
			glNamedRenderbufferStorageMultisample(buffer.RendererID, myNumSamples, *desc.Format, myWidth, myHeight);
		else
			glNamedRenderbufferStorage(buffer.RendererID, *desc.Format, myWidth, myHeight);

		// Attach the renderbuffer to our RenderTarget
		glNamedFramebufferRenderbuffer(myRendererID, *desc.Attachment, GL_RENDERBUFFER, buffer.RendererID);
	}
	// We are going to use a texture as a backing resource
	else {
		// Create a descriptor for the image
		Texture2DDescription imageDesc = Texture2DDescription();
		imageDesc.Width = myWidth;
		imageDesc.Height = myHeight;
		imageDesc.Sampler.WrapS = imageDesc.Sampler.WrapT = WrapMode::ClampToEdge;
		imageDesc.Sampler.MinFilter = MinFilter::Linear;
		imageDesc.Format = (InternalFormat)desc.Format;
		imageDesc.NumSamples = myNumSamples;
		imageDesc.MipLevels = 1; // setting the mip levels

		// Create the image, and store it's info in our buffer tag
		Texture2D::Sptr image = std::make_shared<Texture2D>(imageDesc);
		buffer.Resource = image;
		buffer.RendererID = image->GetRenderID();

		glNamedFramebufferTexture(myRendererID, *desc.Attachment, image->GetRenderID(), 0);

		if (myNumSamples > 1) {
			myUnsampledFrameBuffer->AddAttachment(desc);
		}
	}
	isValid = false;
}

bool cherry::FrameBuffer::Validate()
{
	if (myNumSamples > 1) {
		myUnsampledFrameBuffer->Validate();
	}

	GLenum result = glCheckNamedFramebufferStatus(myRendererID, GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE) {
		switch (result) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			LOG_ERROR("Rendertarget failed to validate. One of the attachment points is framebuffer incomplete."); break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			LOG_ERROR("Rendertarget failed to validate. There are no attachments!"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			LOG_ERROR("Rendertarget failed to validate. Draw buffer is incomplete."); break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			LOG_ERROR("Rendertarget failed to validate. Read buffer is incomplete."); break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			LOG_ERROR("Rendertarget failed to validate. Check the formats of the attached targets"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			LOG_ERROR("Rendertarget failed to validate. Check the multisampling parameters on all attached targets"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			LOG_ERROR("Rendertarget failed to validate for unknown reason!"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_VIEW_TARGETS_OVR:
			LOG_ERROR("Rendertarget failed to validate. Multiview targets issue!"); break;
		default: LOG_ERROR("Rendertarget failed to validate for unknown reason!");
		}
		// GL_FRAMEBUFFER_INCOMPLETE
		isValid = false;
		return false;
	}
	else {
		isValid = true;
		return true;
	}
}

void cherry::FrameBuffer::Bind(uint32_t slot) {
	GetAttachment(RenderTargetAttachment::Color0)->Bind(slot);
}

void cherry::FrameBuffer::Bind(uint32_t slot, RenderTargetAttachment attachment) {
	GetAttachment(attachment)->Bind(slot);
}

void cherry::FrameBuffer::Bind(RenderTargetBinding bindMode) const {
	myBinding = bindMode;
	glBindFramebuffer((GLenum)bindMode, myRendererID);
}

void cherry::FrameBuffer::UnBind() const {
	if (myBinding != RenderTargetBinding::None) {
		if (myNumSamples > 1) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, myRendererID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, myUnsampledFrameBuffer->myRendererID);
			Blit({ 0, 0, myWidth, myHeight }, { 0, 0, myWidth, myHeight }, BufferFlags::All,  MagFilter::Nearest);
			// TODO: Properly blit all our buffers into the destination
			// where to copy from, and where to copy to.
			for (auto& kvp : myLayers) {
				if (IsColorAttachment(kvp.first)) {
					glReadBuffer(*kvp.first);
					glDrawBuffer(*kvp.first);
					Blit({ 0, 0, myWidth, myHeight }, { 0, 0, myWidth, myHeight }, BufferFlags::Color,
						MagFilter::Linear);
				}
			}

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
		glBindFramebuffer((GLenum)myBinding, 0);
		myBinding = RenderTargetBinding::None;
	}
}

void cherry::FrameBuffer::Blit(const glm::ivec4& srcBounds, const glm::ivec4& dstBounds, cherry::BufferFlags flags, cherry::MagFilter filterMode) {
	glBlitFramebuffer(
		srcBounds.x, srcBounds.y, srcBounds.z, srcBounds.w,
		dstBounds.x, dstBounds.y, dstBounds.z, dstBounds.w,
		*flags, *filterMode);
}

cherry::FrameBuffer::Sptr cherry::FrameBuffer::Clone() const {
	auto result = std::make_shared<FrameBuffer>(myWidth, myHeight, myNumSamples);

	for (auto& kvp : myLayers) {
		result->AddAttachment(kvp.second.Description);
	}
	result->Validate();
	return result;
}

void cherry::FrameBuffer::SetDebugName(const std::string& value) {
	glObjectLabel(GL_FRAMEBUFFER, myRendererID, -1, value.c_str());
	// Pass the name down the call chain
	IGraphicsResource::SetDebugName(value);

	for (const auto& attachment : myLayers)
	{
		char name[128];
		sprintf_s(name, 128, "%s_%s", value.c_str(), (~attachment.first).c_str());
		if (attachment.second.IsRenderBuffer) {
			glObjectLabel(GL_RENDERBUFFER, attachment.second.RendererID, -1, name);
		}
		else {
			glObjectLabel(GL_TEXTURE, attachment.second.RendererID, -1, name);
		}
	}
}