package competitive.programming.gametheory.maxntree;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import competitive.programming.common.Constants;
import competitive.programming.gametheory.IGame;
import competitive.programming.gametheory.IMove;
import competitive.programming.gametheory.IMoveGenerator;
import competitive.programming.timemanagement.TimeoutException;
import competitive.programming.timemanagement.Timer;

/**
 * @author Manwe
 *
 *         MaxNTree class allows to find the best move a player can do
 *         considering the other N players will be playing their best move at
 *         each iteration It's algorithm is quite simple, it explores the game
 *         tree applying and canceling all the possible moves of each player
 *         successively When reaching the fixed depth, evaluate the board. Then
 *         it back propagate the best move considering at each game tree node
 *         that the player will play its best promising move
 * 
 * Hint: If you are in pure zero sum 2 player games you should have a
 *         look to Minimax implementation 
 * Hint: You might want to use MaxN tree
 *         only considering your current player and exploring the possible moves
 *         without taking into account the others
 *
 * @param <M>
 *            The class that model a move in the game tree
 * @param <G>
 *            The class that model the Game state
 */

public class MaxNTree<M extends IMove<G>, G extends IGame> {

	private class EvaluatedMove {
		private final double[] evaluation;
		private final M move;
		private final G game;

		EvaluatedMove(double[] evaluation, M move, G game) {
			this.evaluation = evaluation;
			this.move = move;
			this.game = game;
		}

		public double[] getEvaluation() {
			return evaluation;
		}

		public M getMove() {
			return move;
		}

		@Override
		public String toString() {
			return "EvaluatedMove [evaluation=" + Arrays.toString(evaluation) + ", move=" + move + "]";
		}
	}

	private class EvaluatedMoveSorter implements Comparator<EvaluatedMove> {

		private int playerId;

		public EvaluatedMove best(List<EvaluatedMove> moves, int playerId) {
			this.playerId = playerId;
			Collections.sort(moves, this);
			return moves.get(0);
		}

		@Override
		public int compare(EvaluatedMove o1, EvaluatedMove o2) {
			final double[] scores1 = o1.getEvaluation();
			final double[] scores2 = o2.getEvaluation();
			final double diff = converter.convert(scores1, playerId) - converter.convert(scores2, playerId);
			if (diff < 0) {
				return 1;
			}
			if (diff > 0) {
				return -1;
			}
			return 0;
		}
	}

	private IMoveGenerator<M, G> generator;

	private final EvaluatedMoveSorter sorter;

	private final IScoreConverter converter;

	private final Timer timer;

	private EvaluatedMove best;

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
		this.sorter = new EvaluatedMoveSorter();
		this.converter = converter;
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
		return best.game;
	}

	private EvaluatedMove bestInternal(int depth, G board) throws TimeoutException {
		final List<M> generatedMoves = generator.generateMoves(board);
		if (generatedMoves.size() > 0) {
			final List<EvaluatedMove> evaluatedMoves = evaluatesMoves(generatedMoves, board, depth);
			final EvaluatedMove bestMove = sorter.best(evaluatedMoves, board.currentPlayer());
			if (Constants.TRACES) {
				System.err.println("Evaluated moves at depth " + depth + ": " + evaluatedMoves);
			}
			return bestMove;
		}
		// Final state?
		return new EvaluatedMove(board.evaluate(depth), null, board);
	}

	private List<EvaluatedMove> evaluatesMoves(List<M> generatedMoves, G board, int depth) throws TimeoutException {
		final List<EvaluatedMove> evaluatedMoves = new ArrayList<>();

		for (final M move : generatedMoves) {
			timer.timeCheck();
			board = move.execute(board);

			if (depth == 0) {
				evaluatedMoves.add(new EvaluatedMove(board.evaluate(depth), move, board));
			} else {
				final EvaluatedMove bestSubTree = bestInternal(depth - 1, board);
				evaluatedMoves.add(new EvaluatedMove(bestSubTree.getEvaluation(), move, bestSubTree.game));
			}

			board = move.cancel(board);
		}

		return evaluatedMoves;
	}
}