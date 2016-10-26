#ifndef _PHYSICS_DISK_INCLUDED
#define _PHYSICS_DISK_INCLUDED

/**
* @author Manwe
*
*    Class representing a disk. A disk is defined by a position, a speed and a radius
*/

#include "competitive/programming/geometry/Point.hpp"
#include "competitive/programming/math/Complex.hpp"
#include "competitive/programming/math/QuadraticEquation.hpp"

#include <math.h>
#include <limits>
#include <iostream>

namespace competitive{
namespace programming{
namespace physics{
	class Disk {
	public:
		typedef competitive::programming::geometry::Point<double> Vector;
	
		/**
		* Disk construction
		*
		* @param position
		*            the vector representing disk position
		* @param speed
		*            the vector representing disk speed
		* @param radius
		*            radius of the disk
		*/
		Disk(const Vector& position, const Vector& speed, double radius): m_position(position), m_speed(speed), m_radius(radius) {
		}

		/**
		* Disk construction
		*
		* @param positionX, positionY
		*            the vector representing disk position
		* @param speedX, speedY
		*            the vector representing disk speed
		* @param radius
		*            radius of the disk
		*/
		Disk(double positionX, double positionY, double speedX, double speedY, double radius) : m_position(positionX, positionY), m_speed(speedX, speedY), m_radius(radius) {
		}

		const Vector& getPosition()const {
			return m_position;
		}

		const Vector& getSpeed() const {
			return m_speed;
		}

		double getRadius() const {
			return m_radius;
		}

		bool operator==(const Disk& other) const {
			return m_position == other.m_position && m_radius == other.m_radius && m_speed == other.m_speed;
		}

		/**
		* move the disk by its speed vector.
		*
		* @return a new instance of disk with the same speed but a position equals
		*         to position + speed
		*/
		Disk move() const {
			return Disk(m_position+m_speed, m_speed, m_radius);
		}

		/**
		* move the disk by its speed vector.
		*/
		void moveInPlace() {
			m_position += m_speed;
		}

		/**
		* Modify the disk speed adding an acceleration vector
		*
		* @param acceleration
		*            the acceleration vector to be added to the speed vector
		* @return a new instance
		*/
		Disk accelerate(const Vector& acceleration) const {
			return Disk(m_position, m_speed+acceleration, m_radius);
		}

		/**
		* Modify the disk speed adding an acceleration vector
		*
		* @param acceleration
		*            the acceleration vector to be added to the speed vector
		*/
		void accelerateInPlace(const Vector& acceleration) {
			m_speed += acceleration;
		}

		/**
		* Modify the speed of the disk by multiplying its current speed with the
		* given factor. Hint: You might use this method to decelerate also.
		*
		* @return a new instance with the same radius and position but a speed
		*         equals to speed * factor.
		*/
		Disk accelerate(double factor) const {
			return Disk(m_position, m_speed*factor, m_radius);
		}


		/**
		* Modify the speed of the disk by multiplying its current speed with the given factor. 
		* Hint: You might use this method to decelerate also.
		*/
		void accelerateInPlace(double factor) {
			m_speed*=factor;
		}

		/**
		* identify if the disk will collide with each other assuming that both
		* disks will remains with a constant speed. A collision occurs when the two
		* circles touch each other
		*
		* @param other
		*            the other disk
		* @return true if a collision will occurs
		*/
		bool willCollide(const Disk& other) const {
			Vector toOther = other.m_position - m_position;
			Vector relativeSpeed = m_speed - other.m_speed;
			if (relativeSpeed.lengthSquare() <= 0)// No relative movement
				return false;
			if (toOther.dot(relativeSpeed) < 0)// Opposite directions
				return false;
			relativeSpeed.normInplace();
			
			return abs(relativeSpeed.ortho().dot(toOther)) <= m_radius + other.m_radius;
		}

		/**
		* returns the shortest time when the two disks will collide considering
		* that each disk is moving at its speed vector by time unit 
		* A collision occurs when the two circles touch each other.
		*
		* Will return std::numeric_limits<double>::quiet_NaN() if no collision occurs
		* Keep in mind that you must check through std::isnan() if the value is NaN or not.
		*
		* @param other
		*            the other disk
		* @return the time of collision std::numeric_limits<double>::quiet_NaN() if no collision occurs 0
		*         if the two disks are allready colliding
		*/
		double collisionTime(Disk other) const{
			Vector toOther = other.m_position - m_position;
			double distanceCollision = other.m_radius + m_radius;
			if (toOther.lengthSquare() <= distanceCollision * distanceCollision)
				return 0;
			Vector relativeSpeed = m_speed - other.m_speed;

			double a = relativeSpeed.lengthSquare();
			double b = -2 * relativeSpeed.dot(toOther);
			double c = toOther.lengthSquare() - distanceCollision * distanceCollision;

			competitive::programming::math::QuadraticEquation equation(a, b, c);

			int solutions = equation.getSolutionsCount();
			if (solutions == 0) {
				return std::numeric_limits<double>::quiet_NaN();
			}
			if (solutions == 1) {
				double solution = equation.getFirstRoot().getReal();
				if (solution >= 0)
					return solution;
				return std::numeric_limits<double>::quiet_NaN();
			}

			competitive::programming::math::Complex root1 = equation.getFirstRoot();
			if (!root1.isReal()) {
				return std::numeric_limits<double>::quiet_NaN();
			}
			double root1Solution = root1.getReal();
			if (root1Solution >= 0)
				return root1Solution;
			double root2Solution = equation.getSecondRoot().getReal();
			if (root2Solution >= 0)
				return root2Solution;
			return std::numeric_limits<double>::quiet_NaN();
		}
	private:
		Vector m_position;
		Vector m_speed;
		double m_radius;

	};

inline std::ostream& operator<<(std::ostream &os, const Disk& disk)
{
	os << "{ Position:" << disk.getPosition() << ", Speed:" << disk.getSpeed() << ", Radius:" << disk.getRadius() << "}";
	return os;
}

}}}

#endif
