// calculates interpolation
#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "Matrix.h"
#include "Vector.h"

namespace util
{
	namespace math
	{
		// the constant matrix for bezier curves
		static const util::math::Mat4 M_BEZIER;

		// the constant matrix for catmull-rom curves. This has already had the (1/2) scalar applied.
		static const util::math::Mat4 M_CATMULL_ROM;

		// the matrix for catmull-rom, which has not had the (1/2) scalar applied.
		static const util::math::Mat4 M_CATMULL_ROM_DBL; 

		// lerps between two values. If (t) is greater than 1, then it is set to 1. And if it's less than 0, it is set to 0.
		inline float lerp(float a, float b, float t);

		// linear interpolation (vec2). If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		inline util::math::Vec2 lerp(const util::math::Vec2& v0, const util::math::Vec2& v1, float t);

		// linear interpolation (vec3). If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		inline util::math::Vec3 lerp(const util::math::Vec3& v0, const util::math::Vec3& v1, float t);

		// linear interpolation (vec4). If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		inline util::math::Vec4 lerp(const util::math::Vec4& v0, const util::math::Vec4& v1, float t);




		// calculates the inverse of the lerp equation by finding where vt is between v0 and v1 on a given axis.
		// 'axis' determines what axis is used (x or y). x is considered the default case, even if an unusable symbol is passed in axis.
		inline float inverseLerp(const util::math::Vec2 & v0, const const util::math::Vec2 & v1, const util::math::Vec2 & vt, char axis = 'x');

		// calculates the inverse of the lerp equation by finding where v2 is between vt and v1 on a given axis.
		//  'axis' determines what axis is used (x, y, or z). x is considered the default case, even if an unusable symbol is passed in axis.
		// '0' is returned if v0 and v1 are the same on a given axis. 0 is also returned if vt is not between v0 and v1.
		inline float inverseLerp(const util::math::Vec3 & v0, const const util::math::Vec3 & v1, const util::math::Vec3 & vt, char axis = 'x');

		// calculates the inverse of the lerp equation by finding where v2 is between vt and v1 on a given axis.
		//  'axis' determines what axis is used (x, y, z, or w). x is considered the default case, even if an unusable symbol is passed in axis.
		// '0' is returned if v0 and v1 are the same on a given axis. 0 is also returned if vt is not between v0 and v1.
		inline float inverseLerp(const util::math::Vec4& v0, const const util::math::Vec4& v1, const util::math::Vec4& vt, char axis = 'x');

		// finds the inverse of the LERP equation on the x-axis (vec2)
		inline float inverseLerpX(const util::math::Vec2 v0, const const util::math::Vec2 v1, const util::math::Vec2 vt);

		// finds the inverse of the LERP equation on the x-axis (vec3)
		inline float inverseLerpX(const util::math::Vec3 v0, const const util::math::Vec3 v1, const util::math::Vec3 vt);

		// finds the inverse of the LERP equation on the x-axis (vec4)
		inline float inverseLerpX(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt);

		// finds the inverse of the LERP equation on the y-axis (vec2)
		inline float inverseLerpY(const util::math::Vec2 v0, const const util::math::Vec2 v1, const util::math::Vec2 vt);

		// finds the inverse of the LERP equation on the y-axis (vec3)
		inline float inverseLerpY(const util::math::Vec3 v0, const const util::math::Vec3 v1, const util::math::Vec3 vt);

		// finds the inverse of the LERP equation on the y-axis (vec4)
		inline float inverseLerpY(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt);

		// finds the inverse of the LERP equation on the z-axis
		inline float inverseLerpZ(const util::math::Vec3 v0, const const util::math::Vec3 v1, const util::math::Vec3 vt);

		// finds the inverse of the LERP equation on the z-axis (vec4)
		inline float inverseLerpZ(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt);

		// finds the inverse of the LERP equation on the z-axis (vec4)
		inline float inverseLerpW(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt);


		// bilinear interpolation. If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		util::math::Vec2 bilerp(const util::math::Vec2& v0, const util::math::Vec2& v1, float t);

		// calculates a point on a spline.
		// if 'bezier' is true, then it assumes a bezier spline is used. If 'bezier' is false, then it's assumed to be a catmull-rom spline.
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec2 pointOnCurve(const util::math::Vec2 p0, const util::math::Vec2 p1, const util::math::Vec2 p2, const util::math::Vec2 p3, float u, bool catmullRom);

		// calculates a point on a spline.
		// if 'bezier' is true, then it assumes a bezier spline is used. If 'bezier' is false, then it's assumed to be a catmull-rom spline.
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec3 pointOnCurve(const util::math::Vec3 p0, const util::math::Vec3 p1, const util::math::Vec3 p2, const util::math::Vec3 p3, float u, bool catmullRom);

		// calculates a point on a bezier curve. P1 and P2 are the anchours for P0 and P3 respectively.
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec2 bezier(const util::math::Vec2 p0, const util::math::Vec2 p1, const util::math::Vec2 p2, const util::math::Vec2 p3, float u);

		// calculates a point on a bezier curve. P1 and P2 are the anchours for P0 and P3 respectively.
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec3 bezier(const util::math::Vec3 p0, const util::math::Vec3 p1, const util::math::Vec3 p2, const util::math::Vec3 p3, float u);

		// calculates a point on a catmull-rom curve.
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec2 catmullRom(const util::math::Vec2 p0, const util::math::Vec2 p1, const util::math::Vec2 p2, const util::math::Vec2 p3, float u);

		// calculates a point on a catmull-rom curve.
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec3 catmullRom(const util::math::Vec3 p0, const util::math::Vec3 p1, const util::math::Vec3 p2, const util::math::Vec3 p3, float u);

		// calculates a point on a bezier curve using de-Castlejau algorithm (vec2).
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec2 deCastlejau(const util::math::Vec2 p1, const util::math::Vec2 t1, const util::math::Vec2 t2, const util::math::Vec3 p2, float u);

		// calculates a point on a bezier curve using de-Castlejau algorithm (vec3).
		// if (u) is less than 0, it's set at 0. If (u) is greater than 1, it's set to 1.
		util::math::Vec3 deCastlejau(const util::math::Vec3 p1, const util::math::Vec3 t1, const util::math::Vec3 t2, const util::math::Vec3 p2, float u);

		//template<typename T>
		//inline T lerp(const T& v0, const T& v1, const float& t)
		//{
		//	return ((1.0F - t) * v0 + t * v1);
		//}


		//// Inverse Lerp: determines how much a value is between two other values
		//// finds 't' by checking where 'v' is between v0 and v1.
		//template<typename T>
		//inline float inverseLerp(const T& v, const T& v0, const T& v1)
		//{
		//	return (v.x - v0.x) / (v1.x - v0.x);
		//}

	}
}
#endif // !INTERPOLATION_H