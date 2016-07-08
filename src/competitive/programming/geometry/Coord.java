package competitive.programming.geometry;

/**
 * @author Manwe
 *
 *         Class representing a position x y using integers This class contains
 *         immutable fields and will return a new instance each time an
 *         operation is done
 *
 */
public class Coord {
    public final int x;
    public final int y;

    /**
     * Constructor
     *
     * @param x
     *            the x coordinate
     * @param y
     *            the y coordinate
     */
    public Coord(int x, int y) {
        this.x = x;
        this.y = y;
    }

    /**
     * Constructor from a vector A rounding (cast to int) is performed on each x
     * and y doubles of the vector
     *
     * @param v
     *            the vector from which you want to create a coord
     */
    public Coord(Vector v) {
        this.x = (int) v.x;
        this.y = (int) v.y;
    }

    /**
     * Add this coord instance to another one to return the sum of the coords
     *
     * @param coord
     *            the other coord to add
     * @return a new instance with coordinate as the sum of this and the given
     *         coord
     */
    public Coord add(Coord coord) {
        return new Coord(x + coord.x, y + coord.y);
    }

    /**
     * return the euclidian distance between two coords
     *
     * @return sqrt(dx*dx+dy*dy)
     */
    public double distance(Coord coord) {
        return Math.sqrt(distanceSquare(coord));
    }

    /**
     * return the euclidian distance square between two coords
     *
     * Hint : prefer this square distance if you want to compare distances
     * rather than the exact distance that cost more
     *
     * @return dx*dx+dy*dy
     */
    public long distanceSquare(Coord coord) {
        long dx = Math.abs(coord.x - x);
        long dy = Math.abs(coord.y - y);
        return dx * dx + dy * dy;
    }

    /**
     * Substract to the instance x and y the values of the given coord
     *
     * @param coord
     *            the other coord to substract
     * @return a new instance with coordinate as the substraction of this and
     *         the given coord
     */
    public Coord minus(Coord coord) {
        return new Coord(x - coord.x, y - coord.y);
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
        final Coord other = (Coord) obj;
        if (x != other.x) {
            return false;
        }
        if (y != other.y) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + x;
        result = prime * result + y;
        return result;
    }

    @Override
    public String toString() {
        return "[x=" + x + ", y=" + y + "]";
    }

}