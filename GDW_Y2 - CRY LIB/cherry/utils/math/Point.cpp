#include "Point.h"

#include "Interpolation.h"

// POINT
// brings a point into the ostream.
std::ostream& util::math::operator<<(std::ostream& os, const Point& p) { return os << p.toString(); }

// POINT 2 
// point
util::math::Point2::Point2(float x, float y) : x(x), y(y) {}

// converts from vec2 to Point2
util::math::Point2::Point2(util::math::Vec2 v) : x(v.x), y(v.y) {}

// istram for point2
std::istream& util::math::operator>>(std::istream& in, Point2& p)
{
	in >> p.x >> p.y;
	return in;
}

// lerps between this point and the following point
inline util::math::Point2 util::math::Point2::lerp(const Point2& p1, float t) const { return Point2::lerp(*this, p1, t); }

// lerps between two points
inline util::math::Point2 util::math::Point2::lerp(const Point2& p0, const Point2& p1, float t)
{
	util::math::Vec2 v0(p0.x, p0.y);
	util::math::Vec2 v1(p1.x, p1.y);
	util::math::Vec2 vTemp = util::math::lerp(v0, v1, t);

	return Point2(vTemp);
}

// bilinear interpolation
util::math::Point2 util::math::Point2::bilerp(const util::math::Point2& p1, float t) const { return util::math::Point2::bilerp(*this, p1, t); }

// bilinear interpolation
util::math::Point2 util::math::Point2::bilerp(const util::math::Point2& p0, const util::math::Point2& p1, float t)
{
	Vec2 v0(p0.x, p0.y);
	Vec2 v1(p1.x, p1.y);

	return Point2(util::math::bilerp(v0, v1, t));
}

// bezier curve
util::math::Point2 util::math::Point2::bezier(const Point2 p0, const Point2 p1, const Point2 p2, const Point2 p3, float u)
{
	// vector conversion
	util::math::Vec2 v0(p0.x, p0.y);
	util::math::Vec2 v1(p1.x, p1.y);
	util::math::Vec2 v2(p2.x, p2.y);
	util::math::Vec2 v3(p3.x, p3.y);

	return Point2(util::math::bezier(v0, v1, v2, v3, u));
}

// catmull-rom
util::math::Point2 util::math::Point2::catmullRom(const Point2 p0, const Point2 p1, const Point2 p2, const Point2 p3, float u)
{
	// vector conversion
	util::math::Vec2 v0(p0.x, p0.y);
	util::math::Vec2 v1(p1.x, p1.y);
	util::math::Vec2 v2(p2.x, p2.y);
	util::math::Vec2 v3(p3.x, p3.y);

	return Point2(util::math::catmullRom(v0, v1, v2, v3, u));
}

// de-castlejau
util::math::Point2 util::math::Point2::deCastlejau(const Point2 p0, const Point2 t1, const Point2 t2, const Point2 p2, float u)
{
	// vector conversion
	util::math::Vec2 v0(p0.x, p0.y);
	util::math::Vec2 v1(t1.x, t1.y);
	util::math::Vec2 v2(t2.x, t2.y);
	util::math::Vec2 v3(p2.x, p2.y);

	return Point2(util::math::deCastlejau(v0, v1, v2, v3, u));
}

// returns a string representing this point.
std::string util::math::Point2::toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }


// POINT 3
// constructor
util::math::Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

// point2 to point3
util::math::Point3::Point3(Point2& p2) : x(p2.x), y(p2.y), z(0.0F) {}

// vec3 to point3
util::math::Point3::Point3(util::math::Vec3 v) : x(v.x), y(v.y), z(v.z) {}

// istream
std::istream& util::math::operator>>(std::istream& in, Point3& p)
{
	in >> p.x >> p.y >> p.z;
	return in;
}

// lerp for point3
inline util::math::Point3 util::math::Point3::lerp(const Point3& p1, float t) const { return Point3::lerp(*this, p1, t); }

// lerp for point3
inline util::math::Point3 util::math::Point3::lerp(const Point3& p0, const Point3& p1, float t)
{
	util::math::Vec3 v0(p0.x, p0.y, p0.z);
	util::math::Vec3 v1(p1.x, p1.y, p1.z);
	util::math::Vec3 vTemp = util::math::lerp(v0, v1, t);

	return Point3(vTemp);
}

// calculates a point on a catmull-rom curve
util::math::Point3 util::math::Point3::bezier(const Point3 p0, const Point3 p1, const Point3 p2, const Point3 p3, float u)
{
	// vector conversion
	util::math::Vec3 v0(p0.x, p0.y, p0.z);
	util::math::Vec3 v1(p1.x, p1.y, p1.z);
	util::math::Vec3 v2(p2.x, p2.y, p2.z);
	util::math::Vec3 v3(p3.x, p3.y, p3.z);

	return Point3(util::math::bezier(v0, v1, v2, v3, u));
}

// calculates a point on a catmull-rom curve
util::math::Point3 util::math::Point3::catmullRom(const Point3 p0, const Point3 p1, const Point3 p2, const Point3 p3, float u)
{
	// vector conversion
	util::math::Vec3 v0(p0.x, p0.y, p0.z);
	util::math::Vec3 v1(p1.x, p1.y, p1.z);
	util::math::Vec3 v2(p2.x, p2.y, p2.z);
	util::math::Vec3 v3(p3.x, p3.y, p3.z);

	return Point3(util::math::catmullRom(v0, v1, v2, v3, u));
}

// de-castlejau calculation
util::math::Point3 util::math::Point3::deCastlejau(const Point3 p1, const Point3 t1, const Point3 t2, const Point3 p2, float u)
{
	// vector conversion
	util::math::Vec3 v0(p1.x, p1.y, p1.z);
	util::math::Vec3 v1(t1.x, t1.y, t1.z);
	util::math::Vec3 v2(t2.x, t2.y, t2.z);
	util::math::Vec3 v3(p2.x, p2.y, p2.z);

	return Point3(util::math::deCastlejau(v0, v1, v2, v3, u));
}

// toString function for point3
std::string util::math::Point3::toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }