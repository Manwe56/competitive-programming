package competitive.programming.gametheory.treesearch;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

import org.junit.Test;

import competitive.programming.gametheory.IGame;
import competitive.programming.gametheory.IMove;
import competitive.programming.gametheory.IMoveGenerator;
import competitive.programming.gametheory.StickGame;
import competitive.programming.gametheory.StickMove;
import competitive.programming.gametheory.Tester;
import competitive.programming.gametheory.treesearch.TreeSearch.TreeSearchNode;
import competitive.programming.timemanagement.Timer;

public class TreeSearchTest {

    @Test
    public void testStickGame() {
        final Timer timer = new Timer();
        final TreeSearch<StickMove, StickGame> treeSearch = new TreeSearch<StickMove, StickGame>(timer, 0.5, (rawScores, player) -> rawScores[player]);

        Tester.testAlgo((game, generator, maxdepth) -> treeSearch.best(game, generator), true);
        
    }

    static class NegValueGame implements IGame{
    	private final int score;
    	private final int depth;
    	
    	public NegValueGame(int score, int depth) {
			this.score = score;
			this.depth = depth;
		}
    	
		@Override
		public int currentPlayer() {
			return 0;
		}

		@Override
		public double[] evaluate(int depth) {
			if (depth>=5){
				return new double []{-10};
			}
			return new double[]{score};
		}
    }
    
    static class NegValueMove implements IMove<NegValueGame>{
    	private static int counter = 2;
		@Override
		public NegValueGame execute(NegValueGame game) {
			return new NegValueGame(counter++, game.depth+1);
		}
		@Override
		public String toString() {
			return "M";
		}
		static void reset(){
			counter=2;
		}
    }
    
    @Test
    public void testExpansionResetValue(){
    	NegValueMove.reset();
    	TreeSearch<NegValueMove, NegValueGame> treeSearch = new TreeSearch<>(new Timer(), 0.9, (s,p)->s[p]);
    	
    	treeSearch.best(new NegValueGame(1,0), new IMoveGenerator<NegValueMove, NegValueGame>() {
			
			@Override
			public List<NegValueMove> generateMoves(NegValueGame game) {
				List<NegValueMove> moves = new ArrayList<>();
				if (game.depth<5){
					moves.add(new NegValueMove());
					moves.add(new NegValueMove());	
				}
				return moves;
			}
		});
    	assertEquals(-10.0, treeSearch.bestEval()[0], 0.001);
    }
    
    @Test
    public void testPrunning(){
    	NegValueMove.reset();
    	TreeSearch<NegValueMove, NegValueGame> treeSearch = new TreeSearch<>(new Timer(), 0.1, (s,p)->s[p]);
    	
    	treeSearch.setEvaluationsMax(20);
    	
    	NegValueGame game = new NegValueGame(1,0);
		IMoveGenerator<NegValueMove, NegValueGame> generator = new IMoveGenerator<NegValueMove, NegValueGame>() {
			@Override
			public List<NegValueMove> generateMoves(NegValueGame game) {
				List<NegValueMove> moves = new ArrayList<>();
				if (game.depth<5){
					moves.add(new NegValueMove());
					moves.add(new NegValueMove());
					moves.add(new NegValueMove());
					moves.add(new NegValueMove());
					moves.add(new NegValueMove());	
				}
				return moves;
			}
		};
		NegValueMove best = treeSearch.best(game, generator);
    	//treeSearch.print(System.err);
    	//System.err.println("");
    	assertEquals(21.0, treeSearch.bestEval()[0], 0.001);
    	treeSearch.prun(best, generator);
    	//treeSearch.print(System.err);
    	//System.err.println("");
    	Queue<TreeSearchNode<NegValueMove, NegValueGame>> nodes = treeSearch.getToBeExpanded();
    	for (TreeSearchNode<NegValueMove, NegValueGame> node : nodes){
    		assertTrue(node.getEvaluation()[0]>=17);
    	}
    	//treeSearch.print(System.err);
    	//System.err.println("");
    	best = treeSearch.continueBest(generator);
    	//treeSearch.print(System.err);
    	//System.err.println("");
    	assertEquals(42, treeSearch.bestEval()[0], 0.001);
    }
}
