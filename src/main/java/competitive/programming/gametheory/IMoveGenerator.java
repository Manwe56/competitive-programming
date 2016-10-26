package competitive.programming.gametheory;

import java.util.List;

/**
 * @author Manwe
 *
 *         Interface producing the possible moves in function of the game state
 *
 *         Hint: It might be worth not generating all the possible moves, but only "interesting" ones so that you can search deeper in the game tree.
 *
 * @param <M>
 *            The move class representing the action a player can do
 * @param <G>
 *            The game class representing the game state
 */
public interface IMoveGenerator<M extends IMove<G>, G extends IGame> {
    /**
     * Generate all the moves a player can do from a given game state.
     * If no moves are generated, we consider the game is ended
     * Hint: if a player is dead but the others continue to play, you should either return a neutral move that does not change the game state, either manage it
     * directly in the game state to skip the player once a move is executed
     *
     * @param game
     *            The game state from which you must generate the moves
     * @return
     *         The list of all the moves you want to be taken into account during the game tree exploration
     */
    List<M> generateMoves(G game);
}
