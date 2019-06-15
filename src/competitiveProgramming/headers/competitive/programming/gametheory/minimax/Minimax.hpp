#ifndef __GAMETHEORY_MINIMAX_INCLUDED__
#define __GAMETHEORY_MINIMAX_INCLUDED__

/**
* @author Manwe
*
*         Minimax class allows to find the best move a player can do
*         in a zero sum game considering the other player will be playing his best move at
*         each iteration.
*         It includes the alpha beta prunning optimisation in order to explore less branches.
*         It also stores the current best "killer" move in order to explore the best branches first and enhance the pruning rate
* @see <a href="https://en.wikipedia.org/wiki/Minimax">Minimax</a> and <a href="https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning">Alpha-beta pruning</a>
*
* @param <M>
*            The class that model a move in the game tree
* @param <G>
*            The class that model the Game state
*/

#include "competitive/programming/timemanagement/Timer.hpp"
#include "competitive/programming/gametheory/Common.hpp"

#include <vector>
#include <set>
#include <memory>
#include <limits>
#include <algorithm>

namespace competitive {
	namespace programming {
		namespace gametheory {
			namespace minimax {

class AlphaBetaPrunningException: public std::exception {
};

template <typename M>
class MinMaxEvaluatedMove {
public:
	MinMaxEvaluatedMove<M>(const M& move, double value, const std::shared_ptr<MinMaxEvaluatedMove<M> >& bestSubMove)
		:m_move(move), m_value(value), m_bestSubMove(bestSubMove)
	{}

	bool operator<(const MinMaxEvaluatedMove<M> & other) const {
		if (m_value == other.m_value) {
			return this < &other;//We might want to find something better than memory adresses...
		}
		return m_value < other.m_value;
	}

	const M& getMove() const {
		return m_move;
	}

	std::shared_ptr<MinMaxEvaluatedMove<M> > getBestSubMove() const {
		return m_bestSubMove;
	}

	double getValue() const {
		return m_value;
	}
private:
	M m_move;
	double m_value;
	std::shared_ptr<MinMaxEvaluatedMove<M> > m_bestSubMove;
};

template <typename M, typename G>			
class Minimax {
public:
	/**
	* Minimax constructor
	*
	* @param timer
	*            timer instance in order to cancel the search of the best move
	*            if we are running out of time
	*/
	Minimax(const timemanagement::Timer& timer): m_depthmax(0), m_killer(), m_timer(timer) {
	}
	/**
	* Search in the game tree the best move using minimax with alpha beta pruning
	* Search will start at depthMin and increment up to depthMax until a timeout is reached.
	* Thanks to the previous depth search, it first tries to replay the best found move so far so it
	* take maximum advantage of the pruning
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
	* @return the best move you can play considering the other player is selecting
	*         the best move for him at each turn
	*/
	M best(G& game, IMoveGenerator<M, G>& generator, int depthStart, int depthMax) {
		try {
			for (int depth = depthStart+1; depth<depthMax+1; depth++) {
				try {
					m_depthmax = depth;
					m_killer = minimax(game, generator, m_depthmax, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), game.currentPlayer() == 0, m_killer);
				}
				catch (AlphaBetaPrunningException& e) {
					// Should never happen
					throw std::runtime_error("evaluated move found with value not between + infinity and - infinity...");
				}
			}
		}
		catch (timemanagement::TimeoutException& e) {
			//Expected, we just reach a timeout.
		}
		if (!m_killer.get()) {
			return M();
		}
		return m_killer->getMove();
	}
private:
	std::set<MinMaxEvaluatedMove<M> > evaluateSubPossibilities(
		G& game, 
		IMoveGenerator<M, G>& generator, 
		int depth, 
		double alpha, 
		double beta, 
		bool player,
		bool alphaBetaAtThisLevel, 
		const std::shared_ptr<MinMaxEvaluatedMove<M> >& previousAnalysisBest) const {
		
	std::set<MinMaxEvaluatedMove<M> > moves;

	std::vector<M> generatedMoves = generator.generateMoves(game);

	// killer first
	if (previousAnalysisBest.get()) {
		auto found =  std::find(generatedMoves.begin(), generatedMoves.end(), previousAnalysisBest->getMove());
		if (found != generatedMoves.end()) {
			std::swap(*found, *generatedMoves.begin());
		}
	}
	
	for (M& move : generatedMoves) {
		m_timer.timeCheck();
		G& movedGame = move.execute(game);
		std::shared_ptr<MinMaxEvaluatedMove<M> > child;
		try {
			std::shared_ptr<MinMaxEvaluatedMove<M> > bestSubChild = minimax(movedGame, generator, depth - 1, alpha, beta, !player, previousAnalysisBest.get() ? previousAnalysisBest->getBestSubMove()	: std::shared_ptr<MinMaxEvaluatedMove<M> >() );
			child = std::shared_ptr<MinMaxEvaluatedMove<M> >(new MinMaxEvaluatedMove<M>(move, bestSubChild->getValue(), bestSubChild));
		}
		catch (AlphaBetaPrunningException& e) {
			move.cancel(movedGame);
		}
		if (child.get()) {
			// Alpha beta prunning
			if (alphaBetaAtThisLevel) {
				if (player) {
					alpha = std::max(alpha, child->getValue());
					if (beta <= alpha) {
						move.cancel(movedGame);
						throw AlphaBetaPrunningException();
					}
				}
				else {
					beta = std::min(beta, child->getValue());
					if (beta <= alpha) {
						move.cancel(movedGame);
						throw AlphaBetaPrunningException();
					}
				}
			}
			moves.insert(*child.get());
			move.cancel(movedGame);
		}
	}
	return moves;
	}

	std::shared_ptr<MinMaxEvaluatedMove<M> > minimax (
		G& game, 
		IMoveGenerator<M, G>& generator, 
		int depth, 
		double alpha, 
		double beta, 
		bool player,
		const std::shared_ptr<MinMaxEvaluatedMove<M> >& previousAnalysisBest) const {
		if (depth == 0) {
			return finalStateEvaluation(game, depth);
		}
		std::set<MinMaxEvaluatedMove<M> > moves = evaluateSubPossibilities(game, generator, depth, alpha, beta, player, true, previousAnalysisBest);
		if (!moves.empty()) {
			const MinMaxEvaluatedMove<M> & best = player ? *moves.rbegin(): *moves.begin();
			return std::shared_ptr<MinMaxEvaluatedMove<M> >(new MinMaxEvaluatedMove<M>(best));
		}
		return finalStateEvaluation(game, depth);// Real end game status
	}
	std::shared_ptr<MinMaxEvaluatedMove<M> > finalStateEvaluation(G& game, int depth) const {
		return std::shared_ptr<MinMaxEvaluatedMove<M> >(new MinMaxEvaluatedMove<M>(M(), scoreFromEvaluatedGame(game.evaluate(depth)), std::shared_ptr<MinMaxEvaluatedMove<M> >()));// Evaluated game status
	}
	double scoreFromEvaluatedGame(const std::vector<double>& scores) const {
		return scores[0] - scores[1];
	}
private:
	int m_depthmax;
	std::shared_ptr<MinMaxEvaluatedMove<M> > m_killer;
	timemanagement::Timer m_timer;
};

			}
		}
	}
}

#endif
