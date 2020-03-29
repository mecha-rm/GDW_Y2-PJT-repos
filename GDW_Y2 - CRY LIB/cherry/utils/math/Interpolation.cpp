#include "Interpolation.h"

// lerps between two values
float util::math::lerp(float a, float b, float t) { return (1.0f - t) * a + t * b; }

// linear interpolation (vec2)
util::math::Vec2 util::math::lerp(const util::math::Vec2& v0, const util::math::Vec2& v1, float t)
{
	t = (t < 0.0F) ? 0.0F : (t > 1.0F) ? 1.0F : t; // bounds checking

	// Equation: p_cur = (1 - t)p1 + (t)(p2)
	return ((1.0F - t) * v0 + t * v1);
}

// linear interpolation (vec3)
util::math::Vec3 util::math::lerp(const util::math::Vec3& v0, const util::math::Vec3& v1, float t)
{
	t = (t < 0.0F) ? 0.0F : (t > 1.0F) ? 1.0F : t; // bounds checking
	
	// Equation: p_cur = (1 - t)p1 + (t)(p2)
	return ((1.0F - t) * v0 + t * v1);
}

// linear interpolation for vec4
util::math::Vec4 util::math::lerp(const util::math::Vec4& v0, const util::math::Vec4& v1, float t)
{
	t = (t < 0.0F) ? 0.0F : (t > 1.0F) ? 1.0F : t; // bounds checking

	// Equation: p_cur = (1 - t)p1 + (t)(p2)
	return ((1.0F - t) * v0 + t * v1);
}

// EASING FUNCTIONS
// 1. EASE_IN - Slow In, Fast Out (Quadratic)
util::math::Vec3 util::math::easeIn1(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, powf(t, 2)); }

// 2. EASE_IN_2 - Slow In, Fast Out (Cubic)
util::math::Vec3 util::math::easeIn2(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, powf(t, 3)); }

// 3. EASE_IN_3 - Slow In, Fast Out (Optic)
util::math::Vec3 util::math::easeIn3(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, powf(t, 8)); }


// 4. EASE_OUT_1 - Fast In, Slow Out
util::math::Vec3 util::math::easeOut1(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, 1.0F - powf(1.0F - t, 2)); }

// 5. EASE_OUT_2 - Fast In, Slow Out (Inverse Cubic)
util::math::Vec3 util::math::easeOut2(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, 1.0F - powf(1.0F - t, 3)); }

// 6. EASE_OUT_3 - Fast In, Slow Out (Inverse Octic)
util::math::Vec3 util::math::easeOut3(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, 1.0F - powf(1.0F - t, 8)); }

// 7. EASE_IN_OUT_1 - Shrink, Offset, Simplify In / Out
util::math::Vec3 util::math::easeInOut1(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    t = (t < 0.5F) ? 2 * powf(t, 2) : -2 * powf(t, 2) + 4 * t - 1;

    return lerp(v1, v2, t);
}

// 8. EASE_IN_OUT_2 - Shrink, Offset, Simplify In / Out
// Equation: y = (x < 0.5) ? 4x ^ 3 : 4x ^ 3-12x ^ 2 + 12x - 4
util::math::Vec3 util::math::easeInOut2(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    t = (t < 0.5F) ? 4 * powf(t, 3) : 4 * powf(t, 3) - 12 * powf(t, 2) + 12 * t - 3;

    return lerp(v1, v2, t);
}

// 9. EASE_IN_OUT_3 - Shrink, Offset, Simplify In / Out
util::math::Vec3 util::math::easeInOut3(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    t = (t < 0.5F) ? 128 * powf(t, 8) : 0.5F + (1 - powf(2 * (1 - t), 8)) / 2.0F;

    return lerp(v1, v2, t);
}

// 10. EASE_IN_CIRCULAR - Inwards (Valley) Curve
util::math::Vec3 util::math::easeInCircular(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, 1.0F - sqrtf(1 - powf(t, 2))); }

// 11. EASE_OUT_CIRCULAR - Outwards (Hill) Curve
util::math::Vec3 util::math::easeOutCircular(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, sqrtf(-(t - 2) * t)); }

// 12. EASE_IN_OUT_CIRCULAR - Curve Inward, Then Outwards (Valley -> Hill)
util::math::Vec3 util::math::easeInOutCircular(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    // changing the value of 't'.
    t = (t < 0.5F) ?
        0.5F * (1 - sqrtf(1 - 4 * powf(t, 2))) :
        0.5F * (sqrtf(-4 * (t - 2) * t - 3) + 1);

    return lerp(v1, v2, t);
}

// 13. EASE_IN_BOUNCE_1 - Offset Power Composition
util::math::Vec3 util::math::easeInBounce1(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, powf(t, 2) * (2 * t - 1)); }

// 14. EASE_IN_BOUNCE_2 - Offset Power Composition
util::math::Vec3 util::math::easeInBounce2(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, powf(t, 2) * (3 * t - 2)); }

// 15. EASE_IN_BOUNCE_3 - Offset Power Composition
util::math::Vec3 util::math::easeInBounce3(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, powf(t, 2) * (4 * t - 3)); }

// 16. EASE_OUT_BOUNCE_1 - Inverse offset, power composition
util::math::Vec3 util::math::easeOutBounce1(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, t * (t * (2 * t - 5) + 4)); }

// 17. EASE_OUT_BOUNCE_2 - Inverse offset, power composition
util::math::Vec3 util::math::easeOutBounce2(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, t * (t * (3 * t - 7) + 5)); }

// 18. EASE_OUT_BOUNCE_3 - Inverse offset, power composition
util::math::Vec3 util::math::easeOutBounce3(util::math::Vec3 v1, util::math::Vec3 v2, float t) { return lerp(v1, v2, t * (t * (4 * t - 9) + 6)); }

// 19. EASE_IN_OUT_BOUNCE_1 - Shrink, offset, simplify In / Out
util::math::Vec3 util::math::easeInOutBounce1(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    t = (t < 0.5F) ?
        8 * powf(t, 3) - 2 * powf(t, 2) :
        8 * powf(t, 3) - 22 * powf(t, 2) + 20 * t - 5;

    return lerp(v1, v2, t);
}

// 20. EASE_IN_OUT_BOUNCE_2 - Shrink, offset, simplify In / Out
util::math::Vec3 util::math::easeInOutBounce2(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    t = (t < 0.5F) ?
        12 * powf(t, 3) - 4 * powf(t, 2) :
        12 * powf(t, 3) - 32 * powf(t, 2) + 28 * t - 7;

    return lerp(v1, v2, t);
}

// 21. EASE_IN_OUT_BOUNCE_3 - Shrink, offset, simplify In / Out
util::math::Vec3 util::math::easeInOutBounce3(util::math::Vec3 v1, util::math::Vec3 v2, float t)
{
    t = (t < 0.5F) ?
        16 * powf(t, 3) - 6 * powf(t, 2) :
        16 * powf(t, 3) - 42 * powf(t, 2) + 36 * t - 9;

    return lerp(v1, v2, t);
}


// inverse lerp. Re-uses the inverseLerp for vec3.
float util::math::inverseLerp(const util::math::Vec2& v0, const const util::math::Vec2& v1, const util::math::Vec2& vt, char axis)
{
	// making sure a valid axis was requested.
	switch(axis)
	{
	case 'x':
	case 'X':
	case 'y':
	case 'Y':
		break;

	default:
		axis = 'x';
		break;
	}
	
	return inverseLerp(Vec4(v0), Vec4(v1), Vec4(vt), axis);
}

// calculates the inverse of the lerp equation. 'Axis' determines what axis is used (x, y, or z)
float util::math::inverseLerp(const util::math::Vec3 & v0, const const util::math::Vec3 & v1, const util::math::Vec3 & vt, char axis)
{
	// checking for valid input. vec3 can only use x, y, or z
	switch (axis)
	{
	case 'x':
	case 'X':
	case 'y':
	case 'Y':
	case 'z':
	case 'Z':
		break;

	default:
		axis = 'x';
		break;
	}

	return inverseLerp(Vec4(v0), Vec4(v1), Vec4(vt), axis);
}

// inverse lerp for a vec4
float util::math::inverseLerp(const util::math::Vec4& v0, const const util::math::Vec4& v1, const util::math::Vec4& vt, char axis)
{
	float val0; // the start value
	float val1; // the end value
	float valt; // the value to find between v0 and v1.

	// getting the values
	switch (axis)
	{
	case 'x':
	case 'X':
	default:
		val0 = v0.x;
		val1 = v1.x;
		valt = vt.x;
		break;

	case 'y':
	case 'Y':
		val0 = v0.y;
		val1 = v1.y;
		valt = vt.y;
		break;

	case 'z':
	case 'Z':
		val0 = v0.z;
		val1 = v1.z;
		valt = vt.z;
		break;

	case 'w':
	case 'W':
		val0 = v0.w;
		val1 = v1.w;
		valt = vt.w;
		break;
	}

	// checking if valt is actually between val0 and val1, and if val0 and val1 are the same.
	if (valt < val0 || valt > val1 || val0 == val1)
		return 0.0f;

	// (val_want - val_start) / (val_end - val_start)
	return (valt - val0) / (val1 - val0);
}


// inverse lerp on x-axis (vec2)
float util::math::inverseLerpX(const util::math::Vec2 v0, const const util::math::Vec2 v1, const util::math::Vec2 vt)
{
	return inverseLerp(v0, v1, vt, 'x');
}

// inverse lerp on x-axis (vec3)
float util::math::inverseLerpX(const util::math::Vec3 v0, const const util::math::Vec3 v1, const util::math::Vec3 vt)
{
	return inverseLerp(v0, v1, vt, 'x');
}

// inverse lerp on x-axis (vec3)
float util::math::inverseLerpX(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt)
{
	return inverseLerp(v0, v1, vt, 'x');
}


// inverse lerp on y-axis (vec2)
float util::math::inverseLerpY(const util::math::Vec2 v0, const const util::math::Vec2 v1, const util::math::Vec2 vt)
{
	return inverseLerp(v0, v1, vt, 'y');
}

// inverse lerp on y-axis (vec3)
float util::math::inverseLerpY(const util::math::Vec3 v0, const const util::math::Vec3 v1, const util::math::Vec3 vt)
{
	return inverseLerp(v0, v1, vt, 'y');
}

// inverse lerp on x-axis (vec4)
float util::math::inverseLerpY(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt)
{
	return inverseLerp(v0, v1, vt, 'y');
}


// inverse lerp on z-axis (vec3)
float util::math::inverseLerpZ(const util::math::Vec3 v0, const const util::math::Vec3 v1, const util::math::Vec3 vt)
{
	return inverseLerp(v0, v1, vt, 'z');
}

// inverse lerp on x-axis (vec4)
float util::math::inverseLerpZ(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt)
{
	return inverseLerp(v0, v1, vt, 'z');
}


// inverse lerp on x-axis (vec4)
float util::math::inverseLerpW(const util::math::Vec4 v0, const const util::math::Vec4 v1, const util::math::Vec4 vt)
{
	return inverseLerp(v0, v1, vt, 'w');
}



// blinear interpolation
util::math::Vec2 util::math::bilerp(const util::math::Vec2& v0, const util::math::Vec2& v1, float t)
{
	t = (t < 0.0F) ? 0.0F : (t > 1.0F) ? 1.0F : t; // bounds checking

	// gettign the LERP across the passed points
	util::math::Vec2 blvec1 = util::math::lerp(Vec2(0.0F, v0.y), v0, t);
	util::math::Vec2 blvec2 = util::math::lerp(Vec2(0.0F, v1.y), v1, t);

	return util::math::lerp(blvec1, blvec2, t);
}


// calculates a point on a spline (vec2).
util::math::Vec2 util::math::pointOnCurve(const util::math::Vec2 p0, const util::math::Vec2 p1, const util::math::Vec2 p2, const util::math::Vec2 p3, float u, bool catmullRom)
{
	u = (u < 0.0F) ? 0.0F : (u > 1.0F) ? 1.0F : u; // bounds checking

	// reuses the vec3 calculation
	util::math::Vec3 v = pointOnCurve(Vec3(p0.x, p0.y, 0.0F), Vec3(p1.x, p1.y, 0.0F), Vec3(p2.x, p2.y, 0.0F), Vec3(p3.x, p3.y, 0.0F), u, catmullRom);

	// gets the appropriate values
	return util::math::Vec2(v.x, v.y);
}

// calculates a point on a spline (vec3).
util::math::Vec3 util::math::pointOnCurve(const util::math::Vec3 p0, const util::math::Vec3 p1, const util::math::Vec3 p2, const util::math::Vec3 p3, float u, bool catmullRom)
{
	u = (u < 0.0F) ? 0.0F : (u > 1.0F) ? 1.0F : u; // bounds checking

	const Mat4 M = (catmullRom) ? M_CATMULL_ROM : M_BEZIER; // the constant matrix. Checks if using catmull-rom or bezier.

	// these use mat4 since there isn't a multiplication function setup for matrices of different sizes
	// the matrix of 'u', which is 1 X 4
	
	// TODO: using vec4 constructor instead of float constructor. Fix this.
	// Mat4 u1x4(powf(u, 3), powf(u, 2), powf(u, 1), 0.0F);
	Mat4 u1x4;

	// sets the rows. It needs to be done this way because the vec4 constructor gets called by accident during initialization.
	u1x4[0][0] = powf(u, 3);
	u1x4[0][1] = powf(u, 2);
	u1x4[0][2] = powf(u, 1);
	u1x4[0][3] = powf(u, 0);

	// the matrix of points, which is 4 X 2
	Mat4 p4x2(
		p0.x, p0.y, p0.z, 0.0F,
		p1.x, p1.y, p1.z, 0.0F,
		p2.x, p2.y, p2.z, 0.0F,
		p3.x, p3.y, p3.z, 0.0F
	);

	// matrix used for the result.
	Mat4 result = u1x4 * (M * p4x2);

	// returning the result
	return util::math::Vec3(result[0][0], result[0][1], result[0][2]);
}

// calculate bezier curve (vec2)
util::math::Vec2 util::math::bezier(const util::math::Vec2 p0, const util::math::Vec2 p1, const util::math::Vec2 p2, const util::math::Vec2 p3, float u)
{
	return pointOnCurve(p0, p1, p2, p3, u, false);
}

// bezier spline (vec3)
util::math::Vec3 util::math::bezier(const util::math::Vec3 p0, const util::math::Vec3 p1, const util::math::Vec3 p2, const util::math::Vec3 p3, float u)
{
	return pointOnCurve(p0, p1, p2, p3, u, false);
}

// calculates a catmull-rom curve (vec2)
util::math::Vec2 util::math::catmullRom(const util::math::Vec2 p0, const util::math::Vec2 p1, const util::math::Vec2 p2, const util::math::Vec2 p3, float u)
{
	return pointOnCurve(p0, p1, p2, p3, u, true);
}

// catmull-rom (vec3)
util::math::Vec3 util::math::catmullRom(const util::math::Vec3 p0, const util::math::Vec3 p1, const util::math::Vec3 p2, const util::math::Vec3 p3, float u)
{
	return pointOnCurve(p0, p1, p2, p3, u, true);
}

// calculates a point on a bezier curve using de-Castlejau algorithm (vec2)
util::math::Vec2 util::math::deCastlejau(const util::math::Vec2 p1, const util::math::Vec2 t1, const util::math::Vec2 t2, const util::math::Vec3 p2, float u)
{
	util::math::Vec3 v = deCastlejau(util::math::Vec3(p1), util::math::Vec3(t1), util::math::Vec3(t2), util::math::Vec3(p2), u);
	return util::math::Vec2(v.x, v.y);
}

// calculates a point on a bezier curve using de-Castlejau algorithm (vec3)
util::math::Vec3 util::math::deCastlejau(const util::math::Vec3 p1, const util::math::Vec3 t1, const util::math::Vec3 t2, const util::math::Vec3 p2, float u)
{
	u = (u < 0.0F) ? 0.0F : (u > 1.0F) ? 1.0F : u; // bounds checking

	// the three lerp variables used for the calculation
	util::math::Vec3 lerpA = lerp(p1, t1, u); // lerp 1
	util::math::Vec3 lerpB = lerp(t1, t2, u); // lerp 2
	util::math::Vec3 lerpC = lerp(t2, p2, u); // lerp 3

	lerpA = lerp(lerpA, lerpB, u); // lerp 4
	lerpB = lerp(lerpB, lerpC, u); // lerp 5;

	return lerp(lerpA, lerpB, u); // lerp 6
}
