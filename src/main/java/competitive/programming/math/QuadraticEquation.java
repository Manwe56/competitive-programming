package competitive.programming.math;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Manwe
 * 
 * class providing a static method to solve quadratic equations
 *
 */
public class QuadraticEquation {
	
	/**
	 * static method solving quadratic equations:
	 * Finding x where a*x*x+b*x+c=0
	 * 
	 * @return
	 *  a list of root solving the equation.
	 *  The first item in the list will be the item with the lowest real part
	 */
	public static List<Complex> solve(double a, double b, double c){
		List<Complex> result = new ArrayList<>();
		if (a==0){
			if (b!=0){
				result.add(new Complex(-c/b, 0));	
			}
			// else no solutions!
		}
		else{
			if (a<0){//to have smallest root first
				a=-a;
				b=-b;
				c=-c;
			}
			double delta = b*b-4*a*c;
			
			if (delta<0){
				double deltaSqrt = Math.sqrt(-delta);
				
				result.add(new Complex(-b/(2*a), -deltaSqrt/(2*a)));
				result.add(new Complex(-b/(2*a), deltaSqrt/(2*a)));
				
			} else if (delta>0){
				double deltaSqrt = Math.sqrt(delta);
				
				result.add(new Complex((-b-deltaSqrt)/(2*a), 0));
				result.add(new Complex((-b+deltaSqrt)/(2*a), 0));
			}
			else{
				result.add(new Complex(-b/(2*a), 0));
			}	
		}
		
		return result;
	}
}
