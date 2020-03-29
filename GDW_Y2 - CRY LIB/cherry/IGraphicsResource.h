#pragma once

#include "UtilsCRY.h"


namespace cherry
{
	// TODO: expand on comments, and seperat einto .h and .cpp
	// TODO: move to its own file
	// taken from florp::IGraphicsResource in the ICG_2 engine
	class IGraphicsResource {
	public:
		GraphicsClass(IGraphicsResource);

		virtual ~IGraphicsResource() = default;

		/*
			Gets the debugging name associated with this graphics resource
			@returns The debug name of the resource
		*/
		const std::string& GetDebugName() const { return myDebugName; }
		/*
			Sets the debugging name associated with this graphics resource
			@param value The new name for the resource
		*/
		virtual void SetDebugName(const std::string& value) { myDebugName = value; }

		/*
			Gets the renderer ID of this resource. This is the ID that the underlying
			renderer will assign to the resource upon creation (for instance, this will
			be a handle that is populated by a glCreate* command)
			@returns A 32 bit handle for this resource
		*/
		uint32_t GetRenderID() const { return myRendererID; }

	protected:
		IGraphicsResource() = default;

		std::string myDebugName;
		uint32_t myRendererID;
	};
}