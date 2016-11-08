#include "gtest/gtest.h"

#include "competitive/programming/gametheory/maxntree/MaxNTree.hpp"
#include "competitive/programming/gametheory/minimax/Minimax.hpp"
#include "competitive/programming/gametheory/Common.hpp"
#include "competitive/programming/timemanagement/Timer.hpp"

#include <memory>
#include <functional>

using competitive::programming::gametheory::IGame;
using competitive::programming::gametheory::IMoveGenerator;
using competitive::programming::gametheory::IMove;
using competitive::programming::gametheory::ICancellableMove;
using competitive::programming::gametheory::IScoreConverter;
using competitive::programming::gametheory::maxntree::MaxNTree;
using competitive::programming::gametheory::minimax::Minimax;
using competitive::programming::timemanagement::Timer;
using competitive::programming::timemanagement::TimeoutException;

namespace {

	class StickGame: public IGame {
	public:
		StickGame(int currentPlayer, int sticksRemaining, bool gameStateDuplication)
		: m_player(currentPlayer), m_sticksRemaining(sticksRemaining), m_gameStateDuplication(gameStateDuplication)
		{
		}

		void changePlayer() {
			m_player = (m_player + 1) % 2;
		}

		virtual int currentPlayer() const override {
			return m_player;
		}

		std::vector<double> evaluate(int depth) const override{
			std::vector<double> evaluation(2, 0);
			if (getSticksRemaining() == 0) {
				// Player lost.
				assignEvaluation(evaluation, -100);
			}
			else {
				if (getSticksRemaining() % 4 == 1) {
					// If the opponent plays well, he will lose
					assignEvaluation(evaluation, 1);
				}
				else {
					// player can win, it is a valuable advantage
					assignEvaluation(evaluation, -1);
				}
			}
			return evaluation;
		}

		int getSticksRemaining() const {
			return m_sticksRemaining;
		}

		void setSticksRemaining(int sticksRemaining) {
			m_sticksRemaining = sticksRemaining;
		}

		bool isGameStateDuplication() const {
			return m_gameStateDuplication;
		}
	private:
		void assignEvaluation(std::vector<double>& evaluation, double eval) const {
			if (m_player == 0) {
				evaluation[0] = -eval;
				evaluation[1] = eval;
			}
			else {
				evaluation[0] = eval;
				evaluation[1] = -eval;
			}
		}
	private:
		int m_player;
		int m_sticksRemaining;
		bool m_gameStateDuplication;
	};


	class StickMove : public ICancellableMove<StickGame> {
	public:
		StickMove(int sticks): m_sticks(sticks), m_previousGame(), m_nextGame()
		{
		}

		StickMove(): m_sticks(0), m_previousGame(), m_nextGame()
		{}
		StickMove(const StickMove& other): m_sticks(other.m_sticks), m_previousGame(other.m_previousGame), m_nextGame(other.m_nextGame) {
		}
		StickMove& operator=(const StickMove& other) {
			m_sticks = other.m_sticks;
			m_previousGame = other.m_previousGame;
			m_nextGame = other.m_nextGame;
			return *this;
		}
		bool operator==(const StickMove& other) const {
			return m_sticks == other.m_sticks;
		}

		StickGame& cancel(StickGame& game) {
			if (game.isGameStateDuplication()) {
				return *m_previousGame.get();
			}
			game.changePlayer();
			game.setSticksRemaining(game.getSticksRemaining() + getSticks());
			return game;
		}

		StickGame& execute(StickGame& game) {
			int sticksRemaining = game.getSticksRemaining() - getSticks();
			if (game.isGameStateDuplication()) {
				m_previousGame.reset(new StickGame(game));
				m_nextGame.reset(new StickGame(1 - game.currentPlayer(), sticksRemaining, true));
				return *m_nextGame.get();
			}
			game.setSticksRemaining(sticksRemaining);
			game.changePlayer();
			return game;
		}

		int getSticks() const {
			return m_sticks;
		}

		void setSticks(int sticks) {
			m_sticks = sticks;
		}
	private:
		int m_sticks;
		std::shared_ptr<StickGame> m_previousGame;
		std::shared_ptr<StickGame> m_nextGame;
	};

	class StickGenerator : public IMoveGenerator<StickMove, StickGame> {
	public:
		std::vector<StickMove> generateMoves(const StickGame& game) const override {
			std::vector<StickMove> moves;

			if (game.getSticksRemaining() > 2) {
				moves.push_back(StickMove(3));
			}
			if (game.getSticksRemaining() > 1) {
				moves.push_back(StickMove(2));
			}
			if (game.getSticksRemaining() > 0) {
				moves.push_back(StickMove(1));
			}
			return moves;
		}
	};


	class Tester {
	public:
		static void testAlgo(const std::function<StickMove (StickGame&, StickGenerator&, int)>& evaluator, bool gameStateDuplication) {
			StickGenerator generator;

			StickGame game(0, 4, gameStateDuplication);
			StickMove move(0);

			try {
				move = evaluator(game, generator, 2);
				ASSERT_EQ(3, move.getSticks());

				for (int player = 0; player < 2; player++) {
					for (int sticks = 2; sticks < 10; sticks++) {
						for (int depth = 1; depth < 10; depth++) {
							game = StickGame(player, sticks, gameStateDuplication);
							move = evaluator(game, generator, depth);
							int sticksExpected = (sticks - 1) % 4;

							if (sticksExpected != 0) {// There is no solution where we can win and algo can return any move...
								ASSERT_EQ(sticksExpected, move.getSticks());
							}
							ASSERT_EQ(sticks, game.getSticksRemaining());// ensure algo is restoring correctly game state
						}
					}
				}
			}
			catch (TimeoutException& e) {
				FAIL();
			}
		}
	};
}

TEST(MaxNTree, StickGame) {
	Timer timer;
	IScoreConverter converter = [](const std::vector<double>& rawScores, int player) {return rawScores[player]; };
	MaxNTree<StickMove, StickGame> maxNTree(timer, converter);

	Tester::testAlgo([&](StickGame& game, StickGenerator& generator, int maxdepth) { return maxNTree.best(game, generator, 0, maxdepth); }, false);
	Tester::testAlgo([&](StickGame& game, StickGenerator& generator, int maxdepth) { return maxNTree.best(game, generator, 0, maxdepth); }, true);
}

TEST(Minimax, StickGame) {
	Timer timer;
	Minimax<StickMove, StickGame> minimax(timer);

	Tester::testAlgo([&](StickGame& game, StickGenerator& generator, int maxdepth) { return minimax.best(game, generator, 0, maxdepth); }, false);
	Tester::testAlgo([&](StickGame& game, StickGenerator& generator, int maxdepth) { return minimax.best(game, generator, 0, maxdepth); }, true);
}
