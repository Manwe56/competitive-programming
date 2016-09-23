package competitive.programming.gametheory.maxntree;

import java.util.ArrayList;
import java.util.List;

import competitive.programming.common.Constants;
import competitive.programming.gametheory.ICancellableMove;
import competitive.programming.gametheory.IGame;
import competitive.programming.gametheory.IMoveGenerator;
import competitive.programming.gametheory.common.IScoreConverter;
import competitive.programming.gametheory.common.TreeNode;
import competitive.programming.gametheory.common.TreeNodeSorter;
import competitive.programming.timemanagement.TimeoutException;
import competitive.programming.timemanagement.Timer;

/**
 * @author Manwe
 *
 *         MaxNTree class allows to find the best move a player can do
 *         considering the other N players will be playing their best move at
 *         each iteration
 *         It's algorithm is quite simple, it explores the game
 *         tree applying and canceling all the possible moves of each player
 *         successively When reaching the fixed depth, evaluate the board. Then
 *         it back propagate the best move considering at each game tree node
 *         that the player will play its best promising move
 *
 *   Hint: If you are in pure zero sum 2 player games you should have a
 *         look to Minimax implementation
 *   Hint: You might want to use MaxN tree
 *         only considering your current player and exploring the possible moves
 *         without taking into account the others
 *
 * @param <M>
 *            The class that model a move in the game tree
 * @param <G>
 *            The class that model the Game state
 */

public class MaxNTree<M extends ICancellableMove<G>, G extends IGame> {

    private IMoveGenerator<M, G> generator;

    private final TreeNodeSorter<M, G> sorter;

    private int evaluations;

    private final Timer timer;

    private TreeNode<M, G> best;

    /**
     * Creates a new Max-N tree.
     *
     * @param timer
     *            timer instance in order to cancel the search of the best move
     *            if we are running out of time
     * @param converter
     *            A score converter is used so we can configure how the players
     *            are taking into consideration other players scores.
     */
    public MaxNTree(Timer timer, IScoreConverter converter) {
        this.sorter = new TreeNodeSorter<>(converter);
        this.timer = timer;
    }

    /**
     * @param game
     *            The current state of the game
     * @param generator
     *            The move generator that will generate all the possible move of
     *            the playing player at each turn
     * @param depth
     *            the fixed depth up to which the game tree will be expanded
     * @return the best move you can play considering all players are selecting
     *         the best move for them
     * @throws TimeoutException
     */
    public M best(G game, IMoveGenerator<M, G> generator, int depth) throws TimeoutException {
        this.generator = generator;
        best = bestInternal(depth, game);
        return best.getMove();
    }

    /**
     * @return the best game state corresponding to the best move returned by
     *         best method It is mandatory to run best method first!
     */
    public G bestGame() {
        return best.getGame();
    }

    private TreeNode<M, G> bestInternal(int depth, G board) throws TimeoutException {
        final List<M> generatedMoves = generator.generateMoves(board);
        if (!generatedMoves.isEmpty()) {
            final List<TreeNode<M, G>> evaluatedMoves = evaluatesMoves(generatedMoves, board, depth);
            final TreeNode<M, G> bestMove = sorter.best(evaluatedMoves, board.currentPlayer());
            if (Constants.TRACES) {
                System.err.println("Evaluated moves at depth " + depth + ": " + evaluatedMoves);
            }
            return bestMove;
        }
        // Final state?
        evaluations++;
        return new TreeNode<>(board.evaluate(depth), null, board, depth);
    }

    private List<TreeNode<M, G>> evaluatesMoves(List<M> generatedMoves, G board, int depth) throws TimeoutException {
        final List<TreeNode<M, G>> evaluatedMoves = new ArrayList<>();

        for (final M move : generatedMoves) {
            timer.timeCheck();
            board = move.execute(board);

            if (depth == 0) {
                evaluations++;
                evaluatedMoves.add(new TreeNode<M, G>(board.evaluate(depth), move, board, depth));
            } else {
                final TreeNode<M, G> bestSubTree = bestInternal(depth - 1, board);
                evaluatedMoves.add(new TreeNode<>(bestSubTree.getEvaluation(), move, bestSubTree.getGame(), depth));
            }

            board = move.cancel(board);
        }

        return evaluatedMoves;
    }

    /**
     * @return the total count of evaluations performed. Useful for performances stats :)
     */
    public int evaluations() {
        return evaluations;
    }
}
