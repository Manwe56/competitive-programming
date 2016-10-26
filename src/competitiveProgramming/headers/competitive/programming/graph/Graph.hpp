#ifndef _GRAPH_GRAPH_INCLUDED
#define _GRAPH_GRAPH_INCLUDED

/**
* @author Manwe
*
*         class that models a graph and allows to scan it in order to produce
*         various results (just using <a
*         href="https://en.wikipedia.org/wiki/Breadth-first_search">BFS</a> for
*         now) Graph can be directed or bi directional
*
*         Hint: you can use this graph implementation to: compute easily
*         distances from anywhere to one or several targets. Heat map
*         constructions Voronoi territory constructions etc
*
*         for optimization reasons, results are produced as a vector of values
*         (int or double) and by convention the relation between the result and
*         the node is by index
*
* @param <N>
*            The class representing a node in the graph. 
*/

#include <functional>
#include <stdexcept>
#include <vector>
#include <set>

namespace competitive{
namespace programming{
namespace graph{
	template<typename N>
	class Graph {
	public:

		/**
		* Graph constructor
		*
		* Conventions: the nodes you give are in N*. They are identified by
		* their index in this vector meaning that the links source and destination
		* you provide indicates the index of the node in this vector. Code will clearly crash if you 
		* make a mistake and provides indexes on which there is no items at nodes[index]
		*
		* @param nodes
		*            the vector of nodes that does exists in the graph
		* @param linksSource
		*            the vector of index of the source nodes
		* @param linksDestination
		*            the vector of index of the destination nodes
		* @param directed
		*            if true will consider the links you give are directed. It
		*            means you can only go from the source node to the destination
		*            node but not the contrary
		* @throws std::exception
		*             if there is not the same number of indexes between source and
		*             destination
		*/
		Graph(const std::vector<N>& nodes, const std::vector<int>& linksSource, const std::vector<int>& linksDestination, bool directed): m_nodes(nodes), m_neighboursIndexes(nodes.size(), std::vector<int>()) {
			if (linksSource.size() != linksDestination.size())
				throw std::length_error("Number of links source and destination provided does not match!");

			for (int i = 0; i < linksSource.size(); i++) {
				int sourceIndex = linksSource[i];
				int destinationIndex = linksDestination[i];
				createLink(sourceIndex, destinationIndex);
				if (!directed) {
					createLink(destinationIndex, sourceIndex);
				}
			}
		}
		/**
		* Breadth-first search implementation on your graph.
		*
		* It will iteratively:
		* - assign current level value to all source nodes
		* - scan source nodes neighbor to find the reachable nodes that have not been reached yet
		* - Compute next level value and consider all the reachable neighbors as the new source nodes
		*
		* @param intialValue
		* 		The value that will be assigned to all nodes before starting the BFS
		* 		Hint: giving a value impossible to reach can allow you to identify which nodes have never been reached
		* @param firstValue
		* 		The value that will be assigned to all the nodes in sources
		* @param canBeVisited
		* 		Determines if a node should be considered in the BFS or simply ignored
		* @param nextValueIterator
		* 		Give the next level value from the current level value. Add 1 to compute distances
		* @param sources
		* 		The list of nodes that will receive the first value
		* @return
		* 		a vector with the values of each node. The index in this vector correspond to the index of the node given during the constructor
		*/
		template <typename T>
		std::vector<T> breadthFirstSearch(
			const T& initialValue, 
			const T& firstValue, 
			std::function<bool(const N&)> canBeVisited,
			std::function<T(const T&, int)> nextValueIterator,
			const std::vector<int>& sourcesIndex
			) {
			
			std::vector<T> results(m_nodes.size(), initialValue);
			std::vector<bool> alreadyScanned(m_nodes.size(), false);
			
			std::set<int> currentNodesIndex(sourcesIndex.begin(), sourcesIndex.end());

			iterativeBreadthFirstSearch<T>(results, alreadyScanned, currentNodesIndex, firstValue, 0, canBeVisited, nextValueIterator);

			return results;
		}
	private:
		template<typename T>
		void iterativeBreadthFirstSearch(std::vector<T>& results, std::vector<bool>& alreadyScanned, std::set<int>& currentNodes, const T& value, int iteration,
			std::function<bool (const N&)> canBeVisited,
			std::function<T(const T&, int)> nextValueIterator) {
			std::set<int> nextNodes;

			for (int index : currentNodes) {
				if (!alreadyScanned[index]) {
					alreadyScanned[index] = true;
					if (canBeVisited(m_nodes[index])) {
						results[index] = value;
						nextNodes.insert(m_neighboursIndexes[index].begin(), m_neighboursIndexes[index].end());
					}
				}
			}

			if (!nextNodes.empty()) {
				iterativeBreadthFirstSearch(results, alreadyScanned, nextNodes, nextValueIterator(value, iteration + 1), iteration + 1,
					canBeVisited, nextValueIterator);
			}
		}
		void createLink(int sourceIndex, int destinationIndex) {
			m_neighboursIndexes[sourceIndex].push_back(destinationIndex);
		}

	private:
		const std::vector<N>& m_nodes;
		std::vector<std::vector<int> > m_neighboursIndexes;
	};

}}}

#endif
