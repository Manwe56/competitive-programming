package competitive.programming.gametheory;

/**
 * @author Manwe
 *
 *	Interface representing a game state
 */
public interface IGame {
	
    /**
     * The game state must handle the player which is currently playing.
     * Convention: player id represent the index of the player in the evaluated array
     * @return the current player id
     */
    int currentPlayer();

    /**
     * Evaluate the game for each player and score it. This is a key piece of your IA efficiency!
     * 
     * @param depth the current depth when exploring the game tree. 
     * the depth is incremented each time a move is executed. Initial game state correspond to a depth of 0.
     * @return the array of evaluation for each player
     * Convention: player id represent the index of the player in the evaluated array
     */
    double[] evaluate(int depth);
}
