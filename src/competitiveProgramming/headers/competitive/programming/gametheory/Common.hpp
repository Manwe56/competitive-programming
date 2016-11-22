#ifndef __GAME_THEORY_COMMON_INCLUDED__
#define __GAME_THEORY_COMMON_INCLUDED__

/**
* @author Manwe
*
*	Interfaces used by the various game exploration algorithms.
*   TODO: this is a translation from the Java classes. But maybe we can replace one or two of those interfaces by std::functions no?
*/

#include <vector>
#include <functional>
#include <memory>

namespace competitive {
	namespace programming {
		namespace gametheory {
			/**
			*	Interface representing a game state
			*/
			class IGame {
			public:
				/**
				* The game state must handle the player which is currently playing.
				* Convention: player id represent the index of the player in the evaluated array
				* @return the current player id
				*/
				virtual int currentPlayer() const = 0;

				/**
				* Evaluate the game for each player and score it. This is a key piece of your IA efficiency!
				*
				* @param depth the current depth when exploring the game tree.
				* the depth is incremented each time a move is executed. Initial game state correspond to a depth of 0.
				* @return the vector of evaluation for each player
				* Convention: player id represent the index of the player in the evaluated array
				*/
				virtual std::vector<double> evaluate(int depth) const = 0;
			};

			/**
			*         Interface that represent a move. This is one edge of a graph in the
			*         game tree.
			*
			* @param <G>
			*            The game state the move can impact. We expect G to implement IGame
			*/
			template <typename G>
			class IMove {
			public:
				/**
				* Execute a move on a game
				*
				* @param game
				*            the game state
				* @return a new game state with the move applied
				*/
				virtual std::shared_ptr<G> execute(std::shared_ptr<G>& game) = 0;
			};

			/**
			*         Interface that represent a move. This is one edge of a graph in the game tree.
			*
			*         Hint: depending on the game clone complexity/cost, or if you can easily cancel a move, you might be interested in either:
			*         1-each time a move is executed clone the game state and execute the move. When the move is canceled return the original game state
			*         2-execute the move when it is applied, and revert it when canceled
			*
			* @param <G>
			*            The game state the move can impact. We expect G to implement IGame
			*/
			template <typename G>
			class ICancellableMove {
			public:
				/**
				* Execute a move on a game
				*
				* @param game
				*            the game state
				* @return a new game state with the move applied
				*/
				virtual G& execute(G& game) = 0;

				/**
				* Cancel the move
				*
				* @param game
				*            the game state
				* @return the (cached or reverted) game state with the move canceled
				*/
				virtual G& cancel(G& game)=0;
			};

			/**
			*         Interface producing the possible moves in function of the game state
			*
			*         Hint: It might be worth not generating all the possible moves, but only "interesting" ones so that you can search deeper in the game tree.
			*
			* @param <M>
			*            The move class representing the action a player can do. It might be an implementation of a ICancellableMove or a IMove
			* @param <G>
			*            The game class representing the game state. We expect G to implement IGame
			*/
			template <typename M, typename G>
			class IMoveGenerator {
			public:
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
				virtual std::vector<M> generateMoves(const G& game) const=0;
			};

			/**
			* This interface allows to evaluate for a player the relative value of a game state.
			* This value will be used to determine the best move the player will select in the Max-N tree
			* This is particularly useful if you want to consider that the player is trying to maximize only its score, or its score minus the others scores or...
			*/
			typedef std::function<double(const std::vector<double>& rawScores, int player)> IScoreConverter;
		}
	}
}

#endif
