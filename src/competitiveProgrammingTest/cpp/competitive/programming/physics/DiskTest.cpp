#include "gtest/gtest.h"

#include "competitive/programming/physics/Disk.hpp"

#include <cmath>

using competitive::programming::physics::Disk;

TEST(Disk, Move)
{
	Disk disk(2, 1, 2, 3, 5);
	ASSERT_EQ(Disk(4, 4, 2, 3, 5), disk.move());
	ASSERT_EQ(Disk(2, 1, 2, 3, 5), disk);
	disk.moveInPlace();
	ASSERT_EQ(Disk(4, 4, 2, 3, 5), disk);
}

TEST(Disk, Accelerate)
{
	Disk disk(2, 1, 2, 3, 5);
	ASSERT_EQ(Disk(2, 1, 3, 5, 5), disk.accelerate(Disk::Vector(1,2)));
	ASSERT_EQ(Disk(2, 1, 4, 6, 5), disk.accelerate(2));
	ASSERT_EQ(Disk(2, 1, 2, 3, 5), disk);
	disk.accelerateInPlace(Disk::Vector(2,1));
	ASSERT_EQ(Disk(2, 1, 4, 4, 5), disk);
	disk.accelerateInPlace(0.5);
	ASSERT_EQ(Disk(2, 1, 2, 2, 5), disk);
}

TEST(Disk, CollisionDetection)
{
	Disk disk(2, 1, 2, 3, 5);
	Disk oppositeMoves(-2, -5,-2, -3, 1);
	ASSERT_FALSE(disk.willCollide(oppositeMoves));
	Disk frontCollision(6, 7,-2, -3, 1);
	ASSERT_TRUE(disk.willCollide(frontCollision));
	Disk alreadyCollide(2, 1,-2, -3, 1);
	ASSERT_TRUE(disk.willCollide(alreadyCollide));
	Disk noRelativeMovement(10, 10,2, 3, 2);
	ASSERT_FALSE(disk.willCollide(noRelativeMovement));
	Disk goingRight(0, 0,10, 0, 5);
	Disk radiusTouch(2, 6,0, 0, 1);
	ASSERT_TRUE(goingRight.willCollide(radiusTouch));
	Disk radiusNotTouch(2, 6,0, 0, 0.5);
	ASSERT_FALSE(goingRight.willCollide(radiusNotTouch));
}

TEST(Disk, CollisionTime)
{
	Disk disk(2, 1, 2, 3, 5);

	Disk oppositeMoves(-2, -5,-2, -3, 1);
	bool nan = std::isnan(disk.collisionTime(oppositeMoves));
	ASSERT_TRUE(nan);
	Disk goingRight(0, 0,10, 0, 5);
	Disk radiusTouch(2, 6,0, 0, 1);
	ASSERT_EQ(0.2, goingRight.collisionTime(radiusTouch));
	ASSERT_EQ(2, goingRight.collisionTime(Disk(26, 0,0, 0, 1)));
	ASSERT_EQ(0, goingRight.collisionTime(goingRight));
	ASSERT_EQ(0, goingRight.collisionTime(Disk(1, 1,-10, 0, 5)));
}
