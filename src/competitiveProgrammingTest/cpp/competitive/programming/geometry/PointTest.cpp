#include "gtest/gtest.h"

#include "competitive/programming/geometry/Point.hpp"

#include <math.h>
#include <string>

typedef competitive::programming::geometry::Point<int> Coord;
typedef competitive::programming::geometry::Point<double> Vector;

TEST(Coord, Add)
{
	Coord coord(3, 5);
	Coord result = coord + coord;
	coord += Coord(2, -1);

	ASSERT_EQ(Coord(5, 4), coord);
	ASSERT_EQ(5, coord.getX());
	ASSERT_EQ(4, coord.getY());
	ASSERT_EQ(Coord(6, 10), result);
}

TEST(Coord, Distances)
{
	Coord coord(3, 5);

	ASSERT_EQ(0, coord.distanceSquare(coord));
	ASSERT_EQ(40, coord.distanceSquare(Coord(1, -1)));
	ASSERT_EQ(5, coord.distance(Coord(-1, 2)));
}

TEST(Coord, Minus)
{
	Coord coord(3, 5);
	Coord result = coord - coord;
	coord -= Coord(2, -1);

	ASSERT_EQ(Coord(1, 6), coord);
	ASSERT_EQ(Coord(0, 0), result);
}

TEST(Coord, Multiply)
{
	Coord coord(3, 5);
	Coord result = coord * 2;
	coord *= -1;

	ASSERT_EQ(Coord(-3, -5), coord);
	ASSERT_EQ(Coord(6, 10), result);
}


TEST(Vector, BasicOperations) {
	Vector vector(1, -1);

	ASSERT_EQ(2, vector.lengthSquare());
	ASSERT_EQ(sqrt(2), vector.length());
	ASSERT_EQ(Vector(-1, 1), vector.negate());
	ASSERT_EQ(Vector(1, 1), vector.ortho());
	ASSERT_EQ(Vector(sqrt(2) / 2, -sqrt(2) / 2), vector.norm());

	Vector toNormalize(5, 0);
	toNormalize.normInplace();
	ASSERT_EQ(Vector(1, 0), toNormalize);
	ASSERT_EQ(0, vector.dot(Vector(5, 5)));
	ASSERT_EQ(-45, vector.angleInDegree());
	ASSERT_EQ(Vector(1, 1), vector.rotateInDegree(90));
	Vector zero(0, 0);
	zero.normInplace();
	ASSERT_EQ(Vector(0, 0), zero);
}
