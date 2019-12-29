#include "TextureAnimation.h"

// TEXTURE ANIMATION
cherry::TextureAnimation::TextureAnimation() : Animation(3) { }

// TEXTURE ANIMATION FRAME
// constructor
cherry::TextureAnimationFrame::TextureAnimationFrame(glm::vec2 uvMin, glm::vec2 uvMax) : 
	TextureAnimationFrame(cherry::Vec2(uvMin), cherry::Vec2(uvMax))
{
}

cherry::TextureAnimationFrame::TextureAnimationFrame(cherry::Vec2 uvMin, cherry::Vec2 uvMax) : 
	uvCoords(uvMin.v.x, uvMin.v.y, uvMax.v.x, uvMax.v.y)
{
}

cherry::TextureAnimationFrame::TextureAnimationFrame(glm::vec4 uvs) : uvCoords(uvs)
{
}

// uv coordinates as a vec4
cherry::TextureAnimationFrame::TextureAnimationFrame(cherry::Vec4 uvs)
	:uvCoords(uvs.v.x, uvs.v.y, uvs.v.z, uvs.v.w)
{
}

// gets the minimum u coordinate.
float cherry::TextureAnimationFrame::GetUMinimum() const { return uvCoords.x; }

// gets the maximum u-coordinate
float cherry::TextureAnimationFrame::GetUMaximum() const{ return uvCoords.z; }

// gets the minimum v-coordinate.
float cherry::TextureAnimationFrame::GetVMinimum() const { return uvCoords.y; }

// gets the maximum v-coordinate
float cherry::TextureAnimationFrame::GetVMaximum() const { return uvCoords.q; }
