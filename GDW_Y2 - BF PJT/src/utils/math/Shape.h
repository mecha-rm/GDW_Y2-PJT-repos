#ifndef SHAPES_H
#define SHAPES_H

#include "Vector.h"

namespace util
{
	namespace math
	{
		// standard shape
		class Shape
		{
		public:
			Shape();

			// get position x
			float getPositionX() const;

			// set position x
			void setPositionX(float x);

			// get position y
			float getPositionY() const;

			// set position y
			void setPositionY(float y);

			// translate on the x-axis
			void translateX(float xt);

			// translate on the y-axis
			void translateY(float yt);

			virtual std::string toString() = 0;

		private:

		protected:
			util::math::Vec3 position; // position vector
		};

		// 2-dimensional shape
		class Shape2D : public Shape
		{
		public:
			Shape2D(const float SIDES);

			// gets the position of the shape
			util::math::Vec2 getPosition() const;

			// sets the position of the shape
			void setPosition(util::math::Vec2 pos);

			// sets the position of the shape
			void setPosition(float x, float y);

			// moves the shape
			void translate(util::math::Vec2 translation);

			// moves the shape
			void translate(float xt, float yt);

			// calculates the area of the shape
			virtual float area() = 0;

			virtual std::string toString() = 0;

			const float SIDES; // number of sides

		private:

		protected:

		};

		// 3-dimensional shape
		class Shape3D : public Shape
		{
		public:
			Shape3D(const unsigned int FACES, const unsigned int EDGES, const unsigned int VERTICES);

			// gets the position of the shape
			util::math::Vec3 getPosition() const;

			// sets the position of the shape
			void setPosition(util::math::Vec3 pos);

			// sets the position of the shape
			void setPosition(float x, float y, float z);

			// get position z
			float getPositionZ() const;

			// get position z
			void setPositionZ(float z);

			// moves the shape (x, y, z)
			void translate(util::math::Vec3 translation);

			// translates the shape (x, y, z)
			void translate(float xt, float yt, float zt);

			// translate along the z-axis
			void translateZ(float zt);

			// calculates the volume of the shape
			virtual float volume() = 0;

			// toString() function for shapes
			virtual std::string toString() = 0;

			const unsigned int FACES; // number of faces
			const unsigned int EDGES; // number of edges
			const unsigned int VERTICES; // number of vertices

		private:


		protected:
		};
	}
}
//class Circle : public Shape
//{
//	Circle(util::math::Vec2 pos, float radius);
//
//	Circle(util::math::Vec3 pos, float radius);
//};

#endif // !SHAPES_H