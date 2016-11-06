#ifndef __GAMETHEORY_MAXNTREE_INCLUDED__
#define __GAMETHEORY_MAXNTREE_INCLUDED__


/**
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
*            The class that model a move in the game tree. We expect G to implement a IGame
* @param <G>
*            The class that model the Game state. We expect G to implement a ICancellableMove
*/

#include "competitive/programming/timemanagement/Timer.hpp"
#include "competitive/programming/gametheory/Common.hpp"
#include "competitive/programming/gametheory/TreeNode.hpp"

#include <vector>
#include <set>
#include <memory>

namespace competitive {
	namespace programming {
		namespace gametheory {
			namespace maxntree {

template<typename M, typename G>
class MaxNTree {
public:
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
	MaxNTree(timemanagement::Timer& timer, IScoreConverter& converter):
		m_converter(converter), m_timer(timer), m_evaluations(0), m_best()
	{
	}

	/**
	* Explore the game tree incrementally from the depthStart to depthMax.
	* At each depth, update the new best move at this depth. If a time out occurs during the exploration, return the best result of previous depth
	*
	* @param game
	*            The current state of the game
	* @param generator
	*            The move generator that will generate all the possible move of
	*            the playing player at each turn
	* @param depthStart
	*            the start depth up to which the game tree will be expanded
	* @param depthMax
	*            the maximum depth up to which the game tree will be expanded
	* @return the best move you can play considering all players are selecting
	*         the best move for them
	*/
	M best(G& game, IMoveGenerator<M, G>& generator, int depthStart, int depthMax) {
						
		try {
			for (int depth = depthStart; depth<depthMax; depth++) {
				m_best.reset(new TreeNodeWithMove<M>(bestInternal(depth, game, generator)));
			}
		}
		catch (TimeoutException& e) {
			//Expected, we just reach a timeout.
		}

		if (m_best.get()==0)
			return M();
		return m_best->getMove();
	}

	/**
	* @return the total count of evaluations performed. Useful for performances stats :)
	*/
	int evaluations() {
		return m_evaluations;
	}
private:
	TreeNodeWithMove<M> bestInternal(int depth, G& board, IMoveGenerator<M, G>& generator) {
		std::vector<M> generatedMoves = generator.generateMoves(board);
		if (!generatedMoves.empty()) {
			std::set<TreeNodeWithMove<M> > evaluatedMoves = evaluatesMoves(generatedMoves, board, depth, generator);
							
			return *(evaluatedMoves.begin());
		}
		// Final state?
		m_evaluations++;
		return TreeNodeWithMove<M>(M(), board.currentPlayer(), board.evaluate(depth), depth, m_converter);
	}

	std::set<TreeNodeWithMove<M> > evaluatesMoves(std::vector<M>& generatedMoves, G& board, int depth, IMoveGenerator<M, G>& generator) {
		std::set<TreeNodeWithMove<M> > evaluatedMoves;

		for (M& move : generatedMoves) {
			m_timer.timeCheck();
			board = move.execute(board);

			if (depth == 0) {
				m_evaluations++;
				evaluatedMoves.insert(TreeNodeWithMove<M>(move, board.currentPlayer(), board.evaluate(depth), depth, m_converter));
			}
			else {
				TreeNodeWithMove<M> bestSubMove = bestInternal(depth - 1, board, generator);
				evaluatedMoves.insert(TreeNodeWithMove<M>(move, board.currentPlayer(), bestSubMove.getEvaluation(), depth, m_converter));
			}

			board = move.cancel(board);
		}

		return evaluatedMoves;
	}
private:
	IScoreConverter& m_converter;
	timemanagement::Timer& m_timer;
	int m_evaluations;
	std::unique_ptr<TreeNode<M> > m_best;
};

			}
		}
	}
}

#endif
