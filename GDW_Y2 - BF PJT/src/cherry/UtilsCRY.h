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
#define MODEL_VS ("res/shaders/model.vs.glsl")
#define MODEL_FS ("res/shaders/model.fs.glsl")

#define STATIC_VS ("res/shaders/lighting.vs.glsl")
#define STATIC_FS ("res/shaders/blinn-phong.fs.glsl")

#define DYNAMIC_VS ("res/shaders/lighting-morph.vs.glsl")
#define DYNAMIC_FS ("res/shaders/blinn-phong-morph.fs.glsl")

// POST
#define POST_VS ("res/shaders/post/post.vs.glsl")

#define POST_FS ("res/shaders/post/post.fs.glsl")
#define POST_LIGHTING_FS "res/shaders/post/blinn-phong_post.fs.glsl"

#define POST_KERNEL3_FS  "res/shaders/post/kernel3.fs.glsl"
#define POST_KERNEL5_FS  "res/shaders/post/kernel5.fs.glsl"
#define POST_GAMMA_FS "res/shaders/post/gamma_correction.fs.glsl"
#define POST_CEL_FS "res/shaders/post/toon-cel.fs.glsl"
#define POST_LUT_FS "res/shaders/post/lut.fs.glsl"
#define POST_DOF_FS "res/shaders/post/depth_of_field.fs.glsl"

#define POST_BLOOM_HIGHLIGHT_FS "res/shaders/post/bloom_highlight.fs.glsl"
#define POST_BOX_BLUR_FS "res/shaders/post/box_blur.fs.glsl"
#define POST_GAUSSIAN_BLUR3_FS "res/shaders/post/gaussian_blur3.fs.glsl"
#define POST_GAUSSIAN_BLUR5_FS "res/shaders/post/gaussian_blur5.fs.glsl"

#define POST_MOTION_BLUR_FS "res/shaders/post/motion_blur.fs.glsl"

#define POST_RADIAL_BLUR_CIRCULAR_FS "res/shaders/post/radial_blur_circular.fs.glsl"
#define POST_RADIAL_BLUR_LINEAR_FS "res/shaders/post/radial_blur_linear.fs.glsl"
#define POST_RADIAL_BLUR_ZOOM_FS "res/shaders/post/radial_blur_zoom.fs.glsl"

#define POST_ACCUMULATE_ADD_FS "res/shaders/post/accumulate_additive.fs.glsl"
#define POST_ACCUMULATE_MIX_FS "res/shaders/post/accumulate_mix.fs.glsl"
#define POST_ACCUMULATE_AVE_FS "res/shaders/post/accumulate_average.fs.glsl"

#define POST_POINT_SHADOW_FS "res/shaders/post/point_shadow_post.fs.glsl"