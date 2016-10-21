package builder.sample;

import java.util.Arrays;
import java.util.List;

import competitive.programming.geometry.Coord;

public class Sample {
	public static void main(String[] args) {

		/*
		 * this is a multiline comment
		 * 
		 * In the generated Sample file we are expecting the classes from the
		 * geometry package
		 */

		// Another comment

		/* a single line comment */

		List<String> importString = Arrays.asList("to", "test", "imports");
		Coord coord = new Coord(1, 2);
		System.out.println(coord.toString());
		System.out.println(importString);
	}
}
