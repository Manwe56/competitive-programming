package competitive.programming.gametheory;

/**
 * @author Manwe
 *
 *         Interface that represent a move. This is one edge of a graph in the
 *         game tree.
 *
 * @param <G>
 *            The game state the move can impact
 */
public interface IMove<G extends IGame> {

    /**
     * Execute a move on a game
     *
     * @param game
     *            the game state
     * @return the (new or modified) game state with the move applied
     */
    G execute(G game);
}
