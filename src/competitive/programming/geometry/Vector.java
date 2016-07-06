package competitive.programming.geometry;

/**
 * @author Manwe
 * 
 * Class representing a vector (x,y) with double precision
 * It contains final fields and will return a new instance on each performed operations
 *
 */
public class Vector {
    private static String doubleToString(double d) {
        return String.format("%.3f", d);
    }

    public final double x;

    public final double y;

    /**
     * Used in the equals method in order to consider two double are "equals"
     */
    public static double COMPARISON_TOLERANCE = 0.0000001;

    /**
     * Constructor from a given coord
     * @param coord
     * 	The coord from which we will take the x and y
     */
    public Vector(Coord coord) {
        this(coord.x, coord.y);
    }

    /**
     * Constructor from two double
     * @param x
     * 	the x value of the vector
     * @param y
     *  the y value of the vector
     */
    public Vector(double x, double y) {
        this.x = x;
        this.y = y;
    }

    
    /**
     * Constructor from another vector
     * @param other
     * 		use the x and y values of the given vector
     */
    public Vector(Vector other) {
        this(other.x, other.y);
    }

    
    /**
     * Add to this vector the given vector
     * @param other
     * @return
     * 	a new instance of vector sum of this and the given vector
     */
    public Vector add(Vector other) {
        return new Vector(x + other.x, y + other.y);
    }

    /**
     * Negates this vector. The vector has the same magnitude as before, but its direction is now opposite.
     * 
     * @return a new vector instance with both x and y negated
     */
    public Vector negate() {
        return new Vector(-x, -y);
    }

    /**
     * Return a new instance of vector rotated from the given number of degrees.
     * @param degree
     * 		the number of degrees to rotate
     * @return
     * 		a new instance rotated
     */
    public Vector rotateInDegree(double degree){
    	return rotateInRadian(Math.toRadians(degree));
    }

    /**
     * Return a new instance of vector rotated from the given number of radians.
     * @param radians
     * the number of radians to rotate
     * @return
     * a new instance rotated
     */
    public Vector rotateInRadian(double radians) {
        final double length = length();
        double angle = angleInRadian();
        angle += radians;
        final Vector result = new Vector(Math.cos(angle), Math.sin(angle));
        return result.multiply(length);
    }

    /**
     * @return
     * 	the angle between this vector and the vector (1,0) in degrees
     */
    public double angleInDegree() {
        return Math.toDegrees(angleInRadian());
    }

	/**
	 * @return
     * 	the angle between this vector and the vector (1,0) in radians
	 */
	private double angleInRadian() {
		return Math.atan2(y, x);
	}

    /**
     * dot product operator
     * two vectors that are perpendicular have a dot product of 0
     * @param other
     * 		the other vector of the dot product
     * @return
     * 		the dot product
     */
    public double dot(Vector other) {
        return x * other.x + y * other.y;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Vector other = (Vector) obj;
        if (Math.abs(x - other.x) > COMPARISON_TOLERANCE) {
            return false;
        }
        if (Math.abs(y - other.y) > COMPARISON_TOLERANCE) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        long temp;
        temp = Double.doubleToLongBits(x);
        result = prime * result + (int) (temp ^ (temp >>> 32));
        temp = Double.doubleToLongBits(y);
        result = prime * result + (int) (temp ^ (temp >>> 32));
        return result;
    }

    /**
     * @return the length of the vector
     * Hint: prefer length2 to perform length comparisons
     */
    public double length() {
        return Math.sqrt(x * x + y * y);
    }

    /**
     * @return the square of the length of the vector
     */
    public double length2() {
        return x * x + y * y;
    }

    /**
     * Return the vector resulting in this vector minus the values of the other vector
     * @param other
     * the instance to substract from this
     * @return
     * 
     * a new instance of vector result of the minus operation.
     */
    public Vector minus(Vector other) {
        return new Vector(x - other.x, y - other.y);
    }

    /**
     * multiplication operator
     * @param factor
     * the double coefficient to multiply the vector with
     * @return
     * return a new instance multiplied by the given factor
     */
    public Vector multiply(double factor) {
        return new Vector(x * factor, y * factor);
    }

    /**
     * @return
     * the new instance normalized from this. A normalized instance has a length of 1
     * If the length of this is 0 returns a (0,0) vector
     */
    public Vector norm() {
        final double length = length();
        if (length>0)
        	return new Vector(x / length, y / length);
        return new Vector(0,0);
    }

    /**
     * Returns the orthogonal vector (-y,x).
     * @return
     *  a new instance of vector perpendicular to this
     */
    public Vector ortho() {
        return new Vector(-y, x);
    }

    @Override
    public String toString() {
        return "[x=" + doubleToString(x) + ", y=" + doubleToString(y) + "]";
    }
}