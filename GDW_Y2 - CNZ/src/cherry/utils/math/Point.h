#ifndef POINT_H
#define POINT_H

#include "Vector.h"
#include <string>

// class for points. It's recommended that the vector classes be used instead.
namespace util
{
	namespace math
	{
		// abstract class for point
		struct Point
		{
			// ostream for points
			friend std::ostream& operator<<(std::ostream&, const Point&); // << ostream operator

			// to_string operation
			virtual std::string toString() const = 0;
		};

		// class for points in 2D space (x, y)
		struct Point2 : public Point
		{
			Point2(float x, float y);

			// converts a vec2 to a point2
			Point2(util::math::Vec2 v);

			// istream
			friend std::istream& operator>>(std::istream&, Point2&);

			// lerps between this point and a passed point. This point is conisdered p0.
			// (t) will be set to 0 if it is less than 0, and 1 if it is greater than 1.
			Point2 inline lerp(const Point2 & p1, float t) const;

			// lerps between two points
			// (t) will be set to 0 if it is less than 0, and 1 if it is greater than 1.
			static inline Point2 lerp(const Point2 & p0, const Point2 & p1, float t);

			// blinear interpolation 
			Point2 bilerp(const util::math::Point2& p1, float t) const;

			// blinear interpolation 
			static Point2 bilerp(const util::math::Point2& p0, const util::math::Point2& p1, float t);

			// calculates a point on a catmull-rom curve
			static Point2 bezier(const Point2 p0, const Point2 p1, const Point2 p2, const Point2 p3, float u);

			// calculates a point on a catmull-rom curve
			static Point2 catmullRom(const Point2 p0, const Point2 p1, const Point2 p2, const Point2 p3, float u);

			// de-castlejau calculation
			static Point2 deCastlejau(const Point2 p1, const Point2 t1, const Point2 t2, const Point2 p2, float u);
			
			// returns a string representing this point.
			virtual std::string toString() const;

			float x = 0.0F;
			float y = 0.0F;
		};

		// class for points in 3D space (x, y, z)
		struct Point3 : public Point
		{
			Point3(float x, float y, float z);

			// converts from point2 to point3
			Point3(Point2 & p2);

			// converts a vec3 to a point3
			Point3(util::math::Vec3 v);

			// istream
			friend std::istream& operator>>(std::istream&, Point3&);

			// lerps between this point and a passed point. This point is conisdered p0.
			// (t) will be set to 0 if it is less than 0, and 1 if it is greater than 1.
			Point3 inline lerp(const Point3 & p1, float t) const;

			// lerps between two points
			// (t) will be set to 0 if it is less than 0, and 1 if it is greater than 1.
			static Point3 inline lerp(const Point3 & p0, const Point3 & p1, float t);

			// calculates a point on a catmull-rom curve
			static Point3 bezier(const Point3 p0, const Point3 p1, const Point3 p2, const Point3 p3, float u);

			// calculates a point on a catmull-rom curve
			static Point3 catmullRom(const Point3 p0, const Point3 p1, const Point3 p2, const Point3 p3, float u);

			// de-castlejau calculation
			static Point3 deCastlejau(const Point3 p0, const Point3 t1, const Point3 t2, const Point3 p2, float u);

			// toString function
			virtual std::string toString() const;

			float x = 0.0F;
			float y = 0.0F;
			float z = 0.0F;
		};

	}
}

#endif // !POINT_H
