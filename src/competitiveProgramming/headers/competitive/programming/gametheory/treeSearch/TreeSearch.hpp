#ifndef __GAMETHEORY_TREESEARCH_INCLUDED__
#define __GAMETHEORY_TREESEARCH_INCLUDED__

/**
* @author Manwe
*
*         TreeSearch class allows to find the best move a player can do
*         considering the other N players will be playing their best move at each iteration
*
*         It's core algorithm is strongly inspired from the Monte Carlo Tree Search and tries to take advantage of the same idea.
*         Why not a Monte Carlo Tree Search? Because you might not want to perform random moves until you reach the end of the game but take advantage of the value of an evaluation function you have.
*
*         So it proceeds as follow:
*         until you get out of time, you select a node to explore.
*         You expand the children of this node and you use the evaluation function to evaluate the sub nodes.
*         For each evaluated child, you backpropagate the evaluation to the upper nodes. Here as in a MaxNTree, you consider that each player will always select the best move he can play.
*         When running out of time (or you reached the maximum number of evaluations you fixed) you return the best node.
*
*         In order to fine tunes the way the game tree is explored, you have a depthPenaltyFactor parameter that will allow you to get deeper or explore wider the tree.
*
*   Warning:
*         When you apply a move, you MUST return a new instance of the game because of the fact any node might be explored at next exploration
*
*   Hint: If you are in pure zero sum 2 player games you should have a
*         look to Minimax implementation to take advantage of the alpha beta prunning
*   Hint: Due to the fact you must return a new instance of the game each time you execute a move
*         if you can cancel at a low cost the move, you might prefer to use a MaxNTree instead to explore more nodes
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
#include <stdexcept>
#include <algorithm>
#include <cmath>

namespace competitive {
	namespace programming {
		namespace gametheory {
			namespace treeSearch {
template <typename M, typename G>
class TreeSearchNode {
public:
	TreeSearchNode(const std::vector<double>& evaluation, const M& move, const std::shared_ptr<G>& game, int depth, TreeSearchNode<M, G>* father, double depthPenaltyFactor, double eval, IScoreConverter& converter) :
		m_base(move, game->currentPlayer(), evaluation, depth, converter),
		m_game(game),
		m_father(father),
		m_depthPenaltyFactor(depthPenaltyFactor),
		m_subTreeValue(evaluation),
		m_eval(std::pow(depthPenaltyFactor, depth))
	{
	}

	TreeSearchNode(const TreeSearchNode<M,G>& other) :
		m_base(other.m_base),
		m_game(other.m_game),
		m_father(other.m_father),
		m_depthPenaltyFactor(other.m_depthPenaltyFactor),
		m_subTreeValue(other.m_subTreeValue),
		m_eval(std::pow(other.m_depthPenaltyFactor, m_base.getDepth()))
	{
	}

	TreeSearchNode<M, G>& operator=(const TreeSearchNode<M, G>& other) {
		m_base = other.m_base;
		m_game = other.m_game;
		m_father = other.m_father;
		m_depthPenaltyFactor = other.m_depthPenaltyFactor;
		m_subTreeValue = other.m_subTreeValue;
		m_eval = std::pow(other.m_depthPenaltyFactor, m_base.getDepth());
		return *this;
	}

	bool isBetter(const TreeSearchNode<M, G>& other) const {
		IScoreConverter& converter = m_base.getConverter();
		return converter(m_subTreeValue, m_game->currentPlayer()) < converter(other.m_subTreeValue, other.m_game->currentPlayer());
	}

	void backPropagate(const std::vector<double>& subNodeValue, bool backPropagateToFather) {
		if (m_subTreeValue.empty() || isBetter(subNodeValue, m_game->currentPlayer(), m_subTreeValue , m_game->currentPlayer())) {
			m_subTreeValue = subNodeValue;
			if (m_father != nullptr && backPropagateToFather) {
				m_father->backPropagate(subNodeValue, backPropagateToFather);
			}
		}
	}

	void decrementDepth() {
		m_base.decrementDepth();
		m_eval *= std::pow(m_depthPenaltyFactor, m_base.getDepth()) / std::pow(m_depthPenaltyFactor, m_base.getDepth() + 1);
	};

	std::vector<TreeSearchNode<M, G> >& getSubNodes() {
		return m_subNodes;
	}

	std::shared_ptr<G> getGame() const {
		return m_game;
	}

	TreeNode<M>& getBase() {
		return m_base;
	}

	bool operator<(const TreeSearchNode& other) {
		if (m_eval == other.m_eval) {
			return this < &other;//Maybe we want something better than pointer comparison in case of equality...
		}
		return m_eval < other.m_eval;
	}

	void resetEvaluation() {
		bool resetFather = false;

		if (m_father != nullptr && m_father->m_subTreeValue == m_subTreeValue) {
			resetFather = true;
		}
		m_subTreeValue.clear();
		for (TreeSearchNode<M, G>& subNode : m_subNodes) {
			backPropagate(subNode.m_subTreeValue, !resetFather);
		}
		if (resetFather) {
			m_father->resetEvaluation();
		}
	}
private:
	bool isBetter(const std::vector<double>& nodeValue1, int player1, const std::vector<double>& nodeValue2, int player2) {
		IScoreConverter& converter = m_base.getConverter();
		return converter(nodeValue1, player1) > converter(nodeValue2, player2);
	}
private:
	TreeNode<M> m_base;
	TreeSearchNode<M, G>* m_father;
	std::shared_ptr<G> m_game;
	std::vector<double> m_subTreeValue;
	double m_depthPenaltyFactor;
	double m_eval;
	std::vector<TreeSearchNode<M, G> > m_subNodes;
};

template <typename M, typename G>
class TreeSearchNodeSetItem {
public:
	TreeSearchNodeSetItem(TreeSearchNode<M, G>& node): m_node(node) {
	}
	bool operator<(const TreeSearchNodeSetItem<M, G>& other) const {
		return m_node < other.m_node;
	}
	TreeSearchNode<M, G>& getNode()const {
		return m_node;
	}
private:
	TreeSearchNode<M, G>& m_node;//Memory managed by the treeSearchNode vectors of subNodes and TreeSearch rootNodes
};

template <typename M, typename G>
class TreeSearch {
public:
	typedef TreeSearchNode<M, G> Node;
	typedef std::set<TreeSearchNodeSetItem<M, G>> TreeNodeSet;
public:
	/**
	* Creates a new Tree Search.
	*
	* @param timer
	*            timer instance in order to interrupt the search of the best move
	*            if we are running out of time
	* @param depthPenaltyFactor
	*            Configure if the search should favor deeper search or wider search.
	*            for each depth, the evaluation is multiplied by depthPenaltyFactor power depth.
	*            So if depthPenaltyFactor is 1, depth won't be taken into consideration for exploration, and you will explore the tree deeply
	*            If you imput a small value near 0, you will mostly explore the game tree as in a MaxNTree : depth by depth until you run out of time
	* @param converter
	*            A score converter is used so we can configure how the players
	*            are taking into consideration other players scores.
	*/
	TreeSearch(timemanagement::Timer& timer, double depthPenaltyFactor, IScoreConverter& converter):
		m_timer(timer),
		m_depthPenaltyFactor(depthPenaltyFactor), 
		m_converter(converter), 
		m_toBeExpanded(), 
		m_evaluationsPerformed(0),
		m_evaluationsMax(0),
		m_rootNodes(),
		m_best()
	{
	}

	/**
	* @param game
	*            The current state of the game
	* @param generator
	*            The move generator that will generate all the possible move of
	*            the playing player at each turn
	* @return the best move you can play considering all players are selecting
	*         the best move for them
	*/
	M best(const std::shared_ptr<G>& game, IMoveGenerator<M, G>& generator) {
		m_toBeExpanded.clear();
		m_evaluationsPerformed = 0;
		m_rootNodes.clear();
		try {
			std::vector<double> eval = game->evaluate(game->currentPlayer());
			Node root(eval, M(), game, 0, nullptr, m_depthPenaltyFactor, m_converter(eval, game->currentPlayer()), m_converter);
			m_toBeExpanded.insert(root);
			auto rootIterator = selection();
			expansion(m_rootNodes, rootIterator, generator);
			m_toBeExpanded.erase(rootIterator);
			root.getSubNodes() = m_rootNodes;
			treeSearchLoop(generator);
		}
		catch (timemanagement::TimeoutException& e) {
			// Times up!
		}

		return returnCurrentBest(game->currentPlayer());
	}

	/**
	* For testing purpose
	* */
	const std::set<Node >& getToBeExpanded() {
		return m_toBeExpanded;
	}

	/**
	* Print the entire tree representation to the PrintStream.
	*
	* Particularly useful to understand why the best move has been chosen
	* @param out
	* 		  A print stream such as System.err for example
	*/
	void print(std::iostream& out) {
		for (Node& node : m_rootNodes) {
			printNode(node, out);
		}
	}

	/**
	* @return the best game state corresponding to the best move returned by
	*         best method It is mandatory to run best method first!
	*/
	std::shared_ptr<G> bestGame() const {
		return m_best.getGame();
	}


	/**
	* @param evaluationsMax limit the number of node to evaluate.
	*
	* This is nice for testing because you don't want to rely on your computer performances
	*/
	void setEvaluationsMax(int evaluationsMax) {
		m_evaluationsMax = evaluationsMax;
	}

	/**
	* @return the total count of evaluations performed. Useful for performances stats :)
	*/
	int evaluations() const {
		return m_evaluationsPerformed;
	}

	/**
	* Prun the tree of all the nodes that are not under the selected executedMove.
	* Use this when you want to keep a part of the tree between several iterations.
	* Then call continueBest in order to find the next interesting move.
	*
	* @param executedMove
	*        The move that will be your new root of the tree. All the other subtrees will be removed
	* @param generator
	*        The move generator
	*/
	void prun(const M& executedMove, IMoveGenerator<M, G>& generator) {
		Node* newRoot = nullptr;

		for (Node& rootNode : m_rootNodes) {
			if (rootNode.getMove() == executedMove) {
				newRoot = &rootNode;
			}
		}
		if (newRoot == nullptr) {
			throw std::runtime_error("Pruning failed : executedMove not found in the possible moves!");
		}
		m_toBeExpanded.clear();
		m_best.reset();
		m_evaluationsPerformed = 0;
		m_rootNodes = newRoot->m_subNodes;
		newRoot->father = nullptr;
		if (m_rootNodes.empty()) {
			try {
				m_rootNodes = expansion(*newRoot, generator);
			}
			catch (timemanagement::TimeoutException& e) {
			}
		}
		for (Node& rootNode : m_rootNodes) {
			repushToBeExpandedNodes(rootNode);
		}
	}

	/**
	* Continue the exploration of a game tree in order to find the best move possible until we reach the timeout.
	* @param generator
	*            The move generator that will generate all the possible move of
	*            the playing player at each turn
	* @return the best move you can play considering all players are selecting
	*         the best move for them
	* @throws TimeoutException
	*/
	M continueBest(IMoveGenerator<M, G>& generator) {
		m_evaluationsPerformed = 0;
		try {
			treeSearchLoop(generator);
		}
		catch (timemanagement::TimeoutException& e) {
			//Time out
		}
		return returnCurrentBest(m_rootNodes[0].father->getGame().currentPlayer());
	}


	/**
	* @return the evaluation vector of the best evaluation so far.
	*/
	std::vector<double> bestEval() const {
		return m_best->m_subTreeValue;
	}

private:
	void expansion(std::vector<Node>&subNodes, typename TreeNodeSet::const_iterator toExpand, IMoveGenerator<M, G>& generator) {
		TreeNode<M>& node = toExpand->getNode().getBase();
		int depth = node.getDepth();
		std::shared_ptr<G> game = toExpand->getNode().getGame();
		std::vector<M> moves = generator.generateMoves(*game);
		for (M& move : moves) {
			Node node = evaluate(move.execute(game), move, depth + 1, &toExpand->getNode());
			subNodes.push_back(node);
		}
		for (Node& node : subNodes) {
			pushInToBeExpanded(node);
		}
	}

	void pushInToBeExpanded(Node& node) {
		m_toBeExpanded.insert(node);
	}

	Node evaluate(std::shared_ptr<G> newNodeState, M& move, int depth, const Node* father) {
		m_evaluationsPerformed++;
		if (father && newNodeState.get() == father->getGame().get()) {
			throw std::runtime_error("Your game state is not duplicated! Tree search require to duplicate the game state since it will explore the tree incrementally");
		}
		if (m_evaluationsMax > 0 && m_evaluationsPerformed > m_evaluationsMax) {
			throw timemanagement::TimeoutException();
		}
		std::vector<double> eval = newNodeState->evaluate(depth);
		
		return TreeSearchNode<M,G>(eval, move, newNodeState, depth, const_cast<Node*>(father), m_depthPenaltyFactor, m_converter(eval, newNodeState->currentPlayer()), m_converter);
	}

	void treeSearchLoop(IMoveGenerator<M, G>& generator) {
		while (!m_toBeExpanded.empty()) {
			m_timer.timeCheck();
			auto toExpand = selection();
			std::vector<Node>& expandeds = toExpand->getNode().getSubNodes();
			
			expansion(expandeds, toExpand, generator);
			
			if (!expandeds.empty()) {
				toExpand->getNode().resetEvaluation();
			}
			m_toBeExpanded.erase(toExpand);
		}
	}

	M returnCurrentBest(int currentPlayer) {
		if (m_rootNodes.empty())
			return M();
		Node* best = &m_rootNodes[0];
		for (Node& node : m_rootNodes) {
			if (node.isBetter(*best)) {
				best = &node;
			}
		}

		return best->getBase().getMove();
	}

	void repushToBeExpandedNodes(Node& node) {
		node.decrementDepth();
		if (!node.m_subNodes.empty()) {
			for (Node& subNode : node.m_subNodes) {
				repushToBeExpandedNodes(subNode);
			}
		}
		else {
			pushInToBeExpanded(node);
		}
	}

	void printNode(const Node& node, std::iostream& out) {
		for (int i = 0; i < node.getDepth(); i++) {
			out << "\t";
		}
		out << node;
		std::for_each(node.m_subNodes.begin(), node.m_subNodes.end(), [&](const Node& subNode) {printNode(subNode, out); });
	}

	typename TreeNodeSet::const_iterator selection() const {
		return m_toBeExpanded.begin();
	}
private:
	timemanagement::Timer& m_timer;
	double m_depthPenaltyFactor;
	IScoreConverter& m_converter;
	TreeNodeSet m_toBeExpanded;
	int m_evaluationsPerformed;
	int m_evaluationsMax;
	std::vector<Node> m_rootNodes;
	std::shared_ptr<Node> m_best;
};

			}
		}
	}
}

#endif
