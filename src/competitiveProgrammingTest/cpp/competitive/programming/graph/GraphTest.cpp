#include "gtest/gtest.h"

#include "competitive/programming/graph/Graph.hpp"
#include "competitive/programming/timemanagement/Timer.hpp"

#include <vector>
using competitive::programming::graph::Graph;
using competitive::programming::timemanagement::Timer;

TEST(Graph, UndirectedGraph)
{
	/*
	* 1-2-3
	* |   |
	* 4---5---6
	* |
	* 7
	*
	* 8---9---0
	* **/
	std::vector<int> nodes({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
	Graph<int> graph(nodes,
		std::vector<int> {0, 1, 1, 2, 3, 4, 4, 5, 8},
		std::vector<int> {9, 2, 4, 3, 5, 5, 7, 6, 9},
		false);

	std::vector<int> sourcesIndex({ 0, 1, 6 });

	std::vector<double> resultsDouble = graph.breadthFirstSearch<double>(-1.0, 0.0, [](int node) {return node != 3; }, [](double value, int iteration) {return iteration;}, sourcesIndex);
	std::vector<int> resultsInt = graph.breadthFirstSearch<int>(-1, 0, [](int node) {return node != 3; }, [](int value, int iteration) {return iteration; }, sourcesIndex);
	ASSERT_EQ(std::vector<double> ({0, 0, 1, -1, 1, 1, 0, 2, 2, 1}), resultsDouble);
	ASSERT_EQ(std::vector<int>( {0, 0, 1, -1, 1, 1, 0, 2, 2, 1}), resultsInt);
}

TEST(Graph, DirectedGraph) {
	/*
	*
	* 0->1<->2<-3
	* ^         ^
	* |         |
	* 4<--------5
	*
	* */
	std::vector<int> nodes({ 0, 1, 2, 3, 4, 5 });
	Graph<int> directedGraph(nodes, std::vector<int> {0, 1, 2, 3, 4, 5, 5}, std::vector<int> {1, 2, 1, 2, 0, 3, 4}, true);

	std::vector<int> sourcesIndex({ 4 });
	std::vector<double> resultsDouble = directedGraph.breadthFirstSearch<double>(-1.0, 0.0, [](int node) {return node != 2; }, [](double value, int iteration) {return iteration; }, sourcesIndex);
	std::vector<int> resultsInt = directedGraph.breadthFirstSearch<int>(-1, 0, [](int node) {return node != 2; }, [](int value, int iteration) {return iteration; }, sourcesIndex);
	ASSERT_EQ(std::vector<double> ({1, 2, -1, -1, 0, -1}), resultsDouble);
	ASSERT_EQ(std::vector<int>({1, 2, -1, -1, 0, -1}), resultsInt);
}

TEST(Graph, PerformancesBFS) {
	/*
	* computing distances in a n*n grid starting from top left angle
	* */
	int n = 500;

	Timer timer;
	timer.startTimer(100000);

	std::vector<int> nodes(n*n, 0);
	for (int i = 0; i<n*n; i++) {
		nodes[i] = i;
	}

	std::vector<int> sources;
	std::vector<int> destinations;
	
	for (int i = 0; i<n*n; i++) {
		if (i%n != n - 1) {
			//link to the right
			sources.push_back(i);
			destinations.push_back(i + 1);
		}
		if (i<n*n - n) {
			//link to the bottom
			sources.push_back(i);
			destinations.push_back(i + n);
		}
	}

	Graph<int> grid(nodes, sources, destinations, false);

	std::cerr << "Time taken to create the graph for a square of " << n << " :" << std::chrono::duration_cast<std::chrono::milliseconds>(timer.currentTimeTakenInNanoSeconds()).count() << "ms" << std::endl;
	timer.startTimer(10000);
	std::vector<int> startingNodesIndex;

	startingNodesIndex.push_back(0);

	std::vector<int> values = grid.breadthFirstSearch<int>(0, 0, [](int node) {return true; }, [](int value, int iteration) {return iteration; }, startingNodesIndex);

	std::cerr << "Time taken to BFS on a square of " << n << " :" << std::chrono::duration_cast<std::chrono::milliseconds>(timer.currentTimeTakenInNanoSeconds()).count() << "ms" << std::endl;
	
	std::vector<int> expected(n*n, 0);
	for (int line = 0; line<n; line++) {
		for (int column = 0; column<n; column++) {
			expected[line + n*column] = line + column;
		}
	}

	ASSERT_EQ(expected, values);
}
