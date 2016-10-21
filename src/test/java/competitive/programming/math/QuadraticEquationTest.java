package competitive.programming.math;

import static org.junit.Assert.*;

import java.util.Arrays;

import org.junit.Test;

import competitive.programming.math.Complex;
import competitive.programming.math.QuadraticEquation;

public class QuadraticEquationTest {

	@Test
	public void testRealSolutions() {
		assertEquals(Arrays.asList(new Complex(1,0)), QuadraticEquation.solve(1, -2, 1));
		assertEquals(Arrays.asList(new Complex(-2,0)), QuadraticEquation.solve(0, 1, 2));
		//(x-2)*(x-1)=x*x-3x+2
		assertEquals(Arrays.asList(new Complex(1,0), new Complex(2,0)), QuadraticEquation.solve(1, -3, 2));
		assertEquals(Arrays.asList(new Complex(1,0), new Complex(2,0)), QuadraticEquation.solve(-1, 3, -2));
	}

	@Test
	public void imaginarySolutions() {
		assertEquals(Arrays.asList(new Complex(-1,-1), new Complex(-1,1)), QuadraticEquation.solve(1, 2, 2));
		assertEquals(Arrays.asList(new Complex(-1,-1), new Complex(-1,1)), QuadraticEquation.solve(-1, -2, -2));
	}
}
