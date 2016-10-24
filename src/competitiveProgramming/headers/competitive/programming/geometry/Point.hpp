#ifndef _GEOMETRY_POINT_INCLUDED
#define _GEOMETRY_POINT_INCLUDED

/**
* @author Manwe
*
* Class representing a Point (x,y)
*
*/

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

namespace competitive{
namespace programming{
namespace geometry{

template <typename T>
class Point {
private:
	static constexpr double COMPARISON_TOLERANCE = 0.0000001;

public:

    /**
     * Constructor from two values
     * @param x
     * 	the x value of the point
     * @param y
     *  the y value of the point
     */
    Point(T x, T y):m_x(x), m_y(y) {
    }

    
    /**
     * Constructor from another point
     * @param other
     * 		use the x and y values of the given point
     */
    Point(const Point<T>& other):m_x(other.m_x), m_y(other.m_y) {
    }

    
    /**
     * Add to this point the given point
     * @param other
     * @return
     * 	a new instance of point sum of this and the given point
     */
    Point operator+(const Point& other) const{
        return Point(m_x + other.m_x, m_y + other.m_y);
    }


	/**
	* Return X attribute
	*/
	T getX() const {
		return m_x;
	}

	/**
	* Return Y attribute
	*/
	T getY() const {
		return m_y;
	}

	/**
	* Add to this point the given point
	* @param other
	*/
	void operator+=(const Point& other) {
		m_x += other.m_x;
		m_y +=other.m_y;
	}

	/**
	* Return the point resulting in this point minus the values of the other point
	* @param other
	*   the instance to substract from this
	* @return
	*   a new instance of point result of the minus operation.
	*/
	Point operator-(const Point& other) const{
		return Point(m_x - other.m_x, m_y - other.m_y);
	}

	/**
	* substract values of the other point
	* @param other
	*   the instance to substract from this
	*/
	void operator-=(const Point& other) {
		m_x -= other.m_x;
		m_y -= other.m_y;
	}

	/**
	* Return the point resulting in this point multiplied by the given double
	* @param factor
	*   the double coefficient to multiply the vector with
	* @return
	*   return a new instance multiplied by the given factor
	*/
	Point operator*(double factor) const {
		return Point(m_x *factor, m_y *factor);
	}

	/**
	* @param factor
	*   the double coefficient to multiply the vector with
	*/
	void operator*=(double factor) {
		m_x *= factor;
		m_y *= factor;
	}

	/**
	* return the euclidian distance square between two points
	*
	* Hint : prefer this square distance if you want to compare distances
	* rather than the exact distance that cost more
	*
	* @return dx*dx+dy*dy
	*/
	T distanceSquare(const Point& coord) const {
		T dx = coord.m_x - m_x;
		T dy = coord.m_y - m_y;
		return dx * dx + dy * dy;
	}

	/**
	* return the euclidian distance between two points
	*
	* @return sqrt(dx*dx+dy*dy)
	*/
	double distance(const Point& coord) const {
		return sqrt(distanceSquare(coord));
	}

    /**
     * Negates this point. The point has the same magnitude as before, but its direction is now opposite.
     * 
     * @return a new point instance with both x and y negated
     */
    Point negate() {
        return Point(-m_x, -m_y);
    }

    /**
     * Return a new instance of point rotated from the given number of degrees.
     * @param degree
     * 		the number of degrees to rotate
     * @return
     * 		a new instance rotated
     */
    Point rotateInDegree(double degree){
    	return rotateInRadian(toRadians(degree));
    }

    /**
     * Return a new instance of point rotated from the given number of radians.
     * @param radians
     * the number of radians to rotate
     * @return
     * a new instance rotated
     */
    Point rotateInRadian(double radians) const {
        double l = length();
        double angle = angleInRadian();
        angle += radians;
        Point result(cos(angle), sin(angle));
		result *= l;
        return result;
    }

    /**
     * @return
     * 	the angle between this point and the point (1,0) in degrees
     */
    double angleInDegree() const {
        return toDegrees(angleInRadian());
    }

	/**
	 * @return
     * 	the angle between this point and the point (1,0) in radians
	 */
	double angleInRadian() const {
		return atan2(m_y, m_x);
	}

    /**
     * dot product operator
     * two points that are perpendicular have a dot product of 0
     * @param other
     * 		the other point of the dot product
     * @return
     * 		the dot product
     */
    T dot(Point other) const {
        return m_x * other.m_x + m_y * other.m_y;
    }

    bool operator==(const Point<T>& other) const {
		return abs(m_x - other.m_x)<COMPARISON_TOLERANCE && abs(m_y - other.m_y)<COMPARISON_TOLERANCE;
    }

	/**
     * @return the length of the point
     * Hint: prefer length2 to perform length comparisons
     */
    double length() const{
        return sqrt(m_x * m_x + m_y * m_y);
    }

    /**
     * @return the square of the length of the point
     */
    T lengthSquare() const {
        return m_x * m_x + m_y * m_y;
    }

    /**
     * @return
     * the new instance normalized from this. A normalized instance has a length of 1
     * If the length of this is 0 returns a (0,0) point
     */
    Point norm() const{
		Point result(*this);
		result.normInplace();
		return result;
    }

	/**
	* Normalize the point inplace. A normalized instance has a length of 1
	* If the length of this is 0 returns a (0,0) point
	*/
	void normInplace() {
		double l = length();
		if (l > 0) {
			m_x /= l;
			m_y /= l;
		}
		else {
			m_x = 0; 
			m_y = 0;
		}
	}

    /**
     * Returns the orthogonal point (-y,x).
     * @return
     *  a new instance of point perpendicular to this
     */
    Point ortho() {
        return Point(-m_y, m_x);
    }
private:
	static double toDegrees(double radians) {
		return radians * 180.0 / M_PI;
	}
	static double toRadians(double degrees) {
		return degrees * M_PI / 180.0;
	}
private:
    T m_x;
    T m_y;
};

template<typename T>
std::ostream& operator<<(std::ostream &os, const Point<T>& point)
{
	os << "[" << point.getX() << "," << point.getY()<< "]";
	return os;
}

}}}

#endif
