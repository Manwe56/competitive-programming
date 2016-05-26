package competitive.programming.geometry;

import static org.junit.Assert.*;

import org.junit.Test;

import competitive.programming.geometry.Coord;
import competitive.programming.geometry.Vector;

public class CoordTest {

	@Test
	public void add() {
		Coord coord = new Coord(3,5);
		
		assertEquals(new Coord(5,4), coord.add(new Coord(2,-1)));
	}

	@Test
	public void convertFromVector(){
		assertEquals(new Coord(1,2), new Coord(new Vector(1.2, 2.999)));
	}
}
