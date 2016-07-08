package competitive.programming.geometry;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class CoordTest {

    @Test
    public void add() {
        Coord coord = new Coord(3, 5);

        assertEquals(new Coord(5, 4), coord.add(new Coord(2, -1)));
    }

    @Test
    public void distances() {
        Coord coord = new Coord(3, 5);

        assertEquals(0, coord.distanceSquare(coord));
        assertEquals(40, coord.distanceSquare(new Coord(1, -1)));
        assertEquals(5, coord.distance(new Coord(-1, 2)), 0.001);
    }

    @Test
    public void minus() {
        Coord coord = new Coord(3, 5);

        assertEquals(new Coord(1, 6), coord.minus(new Coord(2, -1)));
    }

    @Test
    public void convertFromVector() {
        assertEquals(new Coord(1, 2), new Coord(new Vector(1.2, 2.999)));
    }
}
