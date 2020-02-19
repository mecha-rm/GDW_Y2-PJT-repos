// Utils Cherry - various macro utilities for the program.
#pragma once

#include <memory>
#include <string>

// this is a file taken from the Computer Graphics framework, where it was simply titled "Utils.h".
// this had to be changed due to another file already being called "Utils.h" (see src/cherry/utils/Utils.h).
// this file remains unchanged from what we had in the framework at the time.

// the value of TypeName gets replaced with whatever gets brought in.
// prevents copies of certain game sceneLists
#define NoCopy(TypeName) \
	TypeName(const TypeName& other) = delete; \
	TypeName& operator =(const TypeName& other) = delete;

#define NoMove(TypeName) \
	TypeName(const TypeName&& other) = delete; \
	TypeName& operator =(const TypeName&& other) = delete;

#define GraphicsClass(TypeName)\
	typedef std::shared_ptr<TypeName> Sptr;\
	NoMove(TypeName);\
	NoCopy(TypeName); \
	std::string DebugName;

///// SHADERS //////
// REGULAR
// macros for file paths for commonly used shaders
#define STATIC_VS ("res/shaders/lighting.vs.glsl")
#define STATIC_FS ("res/shaders/blinn-phong.fs.glsl")

#define DYNAMIC_VS ("res/shaders/lighting-morph.vs.glsl")
#define DYNAMIC_FS ("res/shaders/blinn-phong-morph.fs.glsl")

// POST
#define POST_VS ("res/shaders/post/post.vs.glsl")
#define POST_FS ("res/shaders/post/post.fs.glsl")
