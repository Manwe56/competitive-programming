#include "gtest/gtest.h"

#include "competitive/programming/math/Complex.hpp"
#include "competitive/programming/math/QuadraticEquation.hpp"

using competitive::programming::math::QuadraticEquation;
using competitive::programming::math::Complex;

TEST(QuadraticEquation, RealSolutions)
{
	QuadraticEquation first(1, -2, 1);
	ASSERT_EQ(1, first.getSolutionsCount());
	ASSERT_EQ(Complex(1, 0), first.getFirstRoot());

	QuadraticEquation second(0, 1, 2);
	ASSERT_EQ(1, second.getSolutionsCount());
	ASSERT_EQ(Complex(-2, 0), second.getFirstRoot());

	QuadraticEquation third(1, -3, 2);
	ASSERT_EQ(2, third.getSolutionsCount());
	ASSERT_EQ(Complex(1, 0), third.getFirstRoot());
	ASSERT_EQ(Complex(2, 0), third.getSecondRoot());

	QuadraticEquation fourth(-1, 3, -2);
	ASSERT_EQ(2, fourth.getSolutionsCount());
	ASSERT_EQ(Complex(1, 0), fourth.getFirstRoot());
	ASSERT_EQ(Complex(2, 0), fourth.getSecondRoot());
}

TEST(QuadraticEquation, ImaginarySolutions)
{
	QuadraticEquation first(1, 2, 2);
	ASSERT_EQ(2, first.getSolutionsCount());
	ASSERT_EQ(Complex(-1, -1), first.getFirstRoot());
	ASSERT_EQ(Complex(-1, 1), first.getSecondRoot());

	QuadraticEquation second(-1, -2, -2);
	ASSERT_EQ(2, second.getSolutionsCount());
	ASSERT_EQ(Complex(-1, -1), second.getFirstRoot());
	ASSERT_EQ(Complex(-1, 1), second.getSecondRoot());
}

