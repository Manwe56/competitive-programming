package competitive.programming.math;

/**
 * @author Manwe
 * 
 * Class representing a complex number.
 * This complex number has a real and imaginary part
 */
public class Complex {
	private final double real;
	private final double imaginary;
	/**
	 * Construct a complex from a real and imaginary value
	 * @param real
	 * 	assigned to the real part of the complex number
	 * @param imaginary
	 *  assigned to the imaginary part of the complex number
	 */
	public Complex(double real, double imaginary) {
		this.real = real;
		this.imaginary = imaginary;
	}
	public double getReal() {
		return real;
	}
	public double getImaginary() {
		return imaginary;
	}
	/**
	 * @return
	 * true if the complex number has no imaginary value (equals 0)
	 */
	public boolean isReal(){
		return imaginary==0;
	}
	@Override
	public String toString() {
		return "C[" + real + ", " + imaginary + "i]";
	}
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		long temp;
		temp = Double.doubleToLongBits(imaginary);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		temp = Double.doubleToLongBits(real);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		return result;
	}
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Complex other = (Complex) obj;
		if (Double.doubleToLongBits(imaginary) != Double.doubleToLongBits(other.imaginary))
			return false;
		if (Double.doubleToLongBits(real) != Double.doubleToLongBits(other.real))
			return false;
		return true;
	}
}
