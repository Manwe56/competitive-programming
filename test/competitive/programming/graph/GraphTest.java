package competitive.programming.graph;

import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertArrayEquals;

import org.junit.Before;
import org.junit.Test;

import competitive.programming.graph.Graph;
import competitive.programming.graph.IBFSTraversable;
import competitive.programming.graph.IDoubleBfsNextLevelValueIterator;
import competitive.programming.graph.IIntegerBfsNextValueIterator;

public class GraphTest {

	private Graph<Integer> graph;
	private Graph<Integer> directedGraph;

	@Before
	public void init(){
		/*
		 * 1-2-3
		 * |   |
		 * 4---5---6
		 * |
		 * 7
		 * 
		 * 8---9---0
		 * **/
		graph = new Graph<Integer>(new Integer[]{0,1,2,3,4,5,6,7,8,9}, 
				new int[]{0, 1, 1, 2, 3, 4, 4, 5, 8}, 
				new int[]{9, 2, 4, 3, 5, 5, 7, 6, 9}, 
				false);
		/*
		 * 
		 * 0->1<->2<-3
		 * ^         ^
		 * |         |
		 * 4<--------5
		 * 
		 * */
		
		directedGraph = new Graph<Integer>(new Integer[]{0,1,2,3,4,5}, new int[]{0, 1, 2, 3, 4, 5, 5}, new int[]{1, 2, 1, 2, 0, 3, 4}, true);
	}
	
	@Test
	public void bfsOnGraph() {
		IBFSTraversable<Integer> node3IsNotVisitable = node -> node!=3;
		IBFSTraversable<Integer> node2IsNotVisitable = node -> node!=2;
		IDoubleBfsNextLevelValueIterator<Integer> doubleVaueIterator = (value, iteration) -> iteration;
		IIntegerBfsNextValueIterator<Integer> integerValueIterator = (value, iteration) -> iteration;
		
		List<Integer> sources = Arrays.asList(0,1,6);
		double[] resultsDouble = graph.breadthFirstSearch(-1.0, 0.0, node3IsNotVisitable, doubleVaueIterator, sources);
		int[] resultsInt = graph.breadthFirstSearch(-1, node3IsNotVisitable, 0, integerValueIterator, sources);
		assertArrayEquals(new double[]{0,0,1,-1,1,1,0,2,2,1}, resultsDouble, 0.001);
		assertArrayEquals(new int[]{0,0,1,-1,1,1,0,2,2,1}, resultsInt);
		
		sources = Arrays.asList(4);
		resultsDouble = directedGraph.breadthFirstSearch(-1.0, 0.0, node2IsNotVisitable, doubleVaueIterator, sources);
		resultsInt = directedGraph.breadthFirstSearch(-1, node2IsNotVisitable, 0, integerValueIterator, sources);
		assertArrayEquals(new double[]{1,2,-1,-1,0,-1}, resultsDouble, 0.001);
		assertArrayEquals(new int[]{1,2,-1,-1,0,-1}, resultsInt);
	}
}
