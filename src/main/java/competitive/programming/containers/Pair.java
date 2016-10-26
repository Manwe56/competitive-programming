package competitive.programming.containers;

/**
 * Equivalent of the std::pair in C++ Allows to easily return two values
 * 
 * @author Manwe
 *
 * @param <F>
 *            Type of the first object
 * 
 * @param <S>
 *            Type of the second object
 */
public class Pair<F, S> {
	private final F first;
	private final S second;

	public Pair(F first, S second) {
		this.first = first;
		this.second = second;
	}

	public S getSecond() {
		return second;
	}

	public F getFirst() {
		return first;
	}

}
