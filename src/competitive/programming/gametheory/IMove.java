package competitive.programming.gametheory;

/**
 * @author Manwe
 *
 * Interface that represent a move. This is one edge of a graph in the game tree.
 * 
 * Hint: depending on the game clone complexity/cost, or if you can revert it, you might be interested in either:
 * 	1-each time a move is executed clone the game state and execute the move. When the move is canceled return the original game state
 *  2-execute the move when it is applied, and revert it when canceled
 *
 * @param <G>
 * 	The game state the move can impact
 */
public interface IMove<G extends IGame> {

    /**
     * Cancel the move
     * @param game
     * the game state
     * @return
     * the (cached or reverted) game state with the move canceled
     */
    G cancel(G game);
    /**
     * Execute a move on a game
     * @param game
     * the game state
     * @return
     * the (new or modified) game state with the move applied
     */
    G execute(G game);
}
