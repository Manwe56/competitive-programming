#ifndef __GAME_THEORY_TREENODE_INCLUDED__
#define __GAME_THEORY_TREENODE_INCLUDED__

/**
* @author Manwe
*
* Internal class used by the game tree exploration algorithm. A TreeNode represent a node in the game tree.
*/

#include <vector>
#include "competitive/programming/gametheory/Common.hpp"

namespace competitive {
	namespace programming {
		namespace gametheory {

/**
*
* Template class to represent a Node of the game tree.
* Pay attention to the fact that Move and Games are just references and might become invalid!
*
*/
template <typename M>
class TreeNode {
public:
	TreeNode(M& move, int currentPlayer, const std::vector<double>& evaluation, int depth, IScoreConverter& converter):
		m_move(move), m_currentPlayer(currentPlayer), m_evaluation(evaluation), m_depth(depth), m_converter(converter) {
	}

	bool operator<(const TreeNode<M>& other) const {
		double score = m_converter(m_evaluation, m_currentPlayer);
		double otherScore = other.m_converter(other.m_evaluation, other.m_currentPlayer);

		return score < otherScore;
	}

	int getDepth() const {
		return m_depth;
	}

	const std::vector<double>& getEvaluation() const {
		return m_evaluation;
	}

	M& getMove() const {
		return m_move;
	}

protected:
	M& m_move;
private:
	std::vector<double> m_evaluation;
	int m_depth;
	int m_currentPlayer;
	IScoreConverter& m_converter;
};

/**
*
* Template class to represent a Node of the game tree. The move to which it points will be kept in memory so that the node could be returned safely
* Pay attention to the fact that Games are just references and might become invalid!
*
*/
template <typename M>
class TreeNodeWithMove: public TreeNode<M> {
public:
	TreeNodeWithMove(M& move, int currentPlayer, const std::vector<double>& evaluation, int depth, IScoreConverter& converter)
		:TreeNode<M>(move, currentPlayer, evaluation, depth, converter), m_moveCopy(move) {
		m_move = m_moveCopy;
	}
private:
	M m_moveCopy;
};
		}
	}
}

#endif