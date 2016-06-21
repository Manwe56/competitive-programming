package competitive.programming.graph;

import static org.junit.Assert.assertArrayEquals;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

import competitive.programming.timemanagement.Timer;

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
		
		List<Integer> sourcesIndex = Arrays.asList(0,1,6);
		double[] resultsDouble = graph.breadthFirstSearch(-1.0, 0.0, node3IsNotVisitable, doubleVaueIterator, sourcesIndex);
		int[] resultsInt = graph.breadthFirstSearch(-1, node3IsNotVisitable, 0, integerValueIterator, sourcesIndex);
		assertArrayEquals(new double[]{0,0,1,-1,1,1,0,2,2,1}, resultsDouble, 0.001);
		assertArrayEquals(new int[]{0,0,1,-1,1,1,0,2,2,1}, resultsInt);
		
		sourcesIndex = Arrays.asList(4);
		resultsDouble = directedGraph.breadthFirstSearch(-1.0, 0.0, node2IsNotVisitable, doubleVaueIterator, sourcesIndex);
		resultsInt = directedGraph.breadthFirstSearch(-1, node2IsNotVisitable, 0, integerValueIterator, sourcesIndex);
		assertArrayEquals(new double[]{1,2,-1,-1,0,-1}, resultsDouble, 0.001);
		assertArrayEquals(new int[]{1,2,-1,-1,0,-1}, resultsInt);
	}
	
	@Test
	public void performancesBFS(){
		/*
		 * computing distances in a n*n grid starting from top left angle
		 * Initial timings:
		 * ~750ms create graph
		 * ~2800ms bfs
		 * */
		int n=500;
		
		Timer timer = new Timer();
		timer.startTimer(100000);
		
		Integer[] nodes = new Integer[n*n];
		for (int i=0; i<n*n; i++){
			nodes[i] = i;
		}
		
		int[] sources = new int[2*n*n-2*n];
		int[] destinations = new int[2*n*n-2*n];
		int index = 0;
		
		for (int i=0; i<n*n; i++){
			if (i%n!=n-1){
				//link to the right
				sources[index]=i;
				destinations[index]=i+1;
				index++;
			}
			if (i<n*n-n){
				//link to the bottom
				sources[index]=i;
				destinations[index]=i+n;
				index++;
			}
		}
		
		Graph<Integer> grid = new Graph<>(nodes, sources, destinations, false);
		

		System.err.println("Time taken to create the graph for a square of "+n+" :"+(timer.currentTimeTakenInNanoSeconds()/1000000+"ms"));
		timer.startTimer(10000);
		List<Integer> startingNodesIndex = new ArrayList<>();
		
		startingNodesIndex.add(0);
		
		IBFSTraversable<Integer> allTraversable = node -> true;
		IIntegerBfsNextValueIterator<Integer> integerValueIterator = (value, iteration) -> iteration;
		
		int[] values = grid.breadthFirstSearch(0, allTraversable, 0, integerValueIterator, startingNodesIndex);
		
		int[] expected = new int[n*n];
		for (int line=0; line<n; line++){
			for (int column=0; column<n; column++){
				expected[line+n*column]=line+column;
			}
		}
		
		System.err.println("Time taken to BFS on a square of "+n+" :"+(timer.currentTimeTakenInNanoSeconds()/1000000+"ms"));
		
		assertArrayEquals(expected, values);
	}
}
