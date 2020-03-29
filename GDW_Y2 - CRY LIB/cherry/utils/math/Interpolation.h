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
		static const util::math::Mat4 M_BEZIER
		(
			-1, 3, -3, 1,
			3, -6, 3, 0,
			-3, 3, 0, 0,
			1, 0, 0, 0
		);

		// the constant matrix for catmull-rom curves. This has already had the (1/2) scalar applied.
		static const util::math::Mat4 M_CATMULL_ROM
		(
			0.5F * -1.0F, 0.5F * 3.0F, 0.5F * -3.0F, 0.5F * 1.0F,
			0.5F * 2.0F, 0.5F * -5.0F, 0.5F * 4.0F, 0.5F * -1.0F,
			0.5F * -1.0F, 0.5F * 0.0F, 0.5F * 1.0F, 0.5F * 0.0F,
			0.5F * 0.0F, 0.5F * 2.0F, 0.5F * 0.0F, 0.5F * 0.0F
		);

		// the matrix for catmull-rom, which has not had the (1/2) scalar applied.
		static const util::math::Mat4 M_CATMULL_ROM_DBL
		(
			-1, 3, -3, 1,
			2, -5, 4, -1,
			-1, 0, 1, 0,
			0, 2, 0, 0
		);

		// lerps between two values. If (t) is greater than 1, then it is set to 1. And if it's less than 0, it is set to 0.
		inline float lerp(float a, float b, float t);

		// linear interpolation (vec2). If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		inline util::math::Vec2 lerp(const util::math::Vec2& v0, const util::math::Vec2& v1, float t);

		// linear interpolation (vec3). If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		inline util::math::Vec3 lerp(const util::math::Vec3& v0, const util::math::Vec3& v1, float t);

		// linear interpolation (vec4). If (t) is less than 0, it's set at 0. If (t) is greater than 1, it's set to 1.
		inline util::math::Vec4 lerp(const util::math::Vec4& v0, const util::math::Vec4& v1, float t);

		// Easing Functions
		// 1. Ease In 1 - Slow In, Fast Out (Quadratic)
		util::math::Vec3 easeIn1(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 2. Ease In 1 - Slow In, Fast Out (Cubic)
		util::math::Vec3 easeIn2(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 3. Ease In 3 - Slow In, Slow Out (Octic)
		util::math::Vec3 easeIn3(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 4. Ease Out 1 - Fast In, Slow Out (Inverse Quadratic)
		util::math::Vec3 easeOut1(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 5. Ease Out 2 - Fase In, Slow Out (Inverse Cubic)
		util::math::Vec3 easeOut2(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 6. Ease Out 3 - Fade In, Slow Out (Inverse Octic)
		util::math::Vec3 easeOut3(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 7. Ease In-Out 1 - Shrink, Offset, Simplify In / Out
		util::math::Vec3 easeInOut1(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 8. Ease In-Out 2 - Shrink, Offset, Simplify In / Out
		util::math::Vec3 easeInOut2(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 9. Ease In-Out 2 - Shrink, Offset, Simplify In / Out
		util::math::Vec3 easeInOut3(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 10. Ease In Circular - Inwards (Valley) Curve
		util::math::Vec3 easeInCircular(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 11. Ease Out Circular - Outwards (Hill) Curve
		util::math::Vec3 easeOutCircular(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 12. Ease In-Out Circular - curve inward, then outwards (Valley -> Hill)
		util::math::Vec3 easeInOutCircular(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 13. Ease In Bounce 1 - Offset Power Composition
		util::math::Vec3 easeInBounce1(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 14. Ease In Bounce 2 - Offset Power Composition
		util::math::Vec3 easeInBounce2(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 15. Ease In Bounce 3 - Offset Power Composition
		util::math::Vec3 easeInBounce3(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 16. Ease Out Bounce 1 - inverse offset, power composition
		util::math::Vec3 easeOutBounce1(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 17. Ease Out Bounce 2 - inverse offset, power composition
		util::math::Vec3 easeOutBounce2(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 18. Ease Out Bounce 3 - inverse offset, power composition
		util::math::Vec3 easeOutBounce3(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 19. EaseInOutBounce 1 - shrink, offset, simplify in/out
		util::math::Vec3 easeInOutBounce1(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 20. EaseInOutBounce 2 - shrink, offset, simplify in/out
		util::math::Vec3 easeInOutBounce2(util::math::Vec3 v1, util::math::Vec3 v2, float t);

		// 21. EaseInOutBounce 3 - Shrink, offset, simplify In / Out
		util::math::Vec3 easeInOutBounce3(util::math::Vec3 v1, util::math::Vec3 v2, float t);



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