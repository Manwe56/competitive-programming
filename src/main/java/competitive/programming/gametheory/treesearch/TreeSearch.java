package competitive.programming.gametheory.treesearch;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;

import competitive.programming.gametheory.IGame;
import competitive.programming.gametheory.IMove;
import competitive.programming.gametheory.IMoveGenerator;
import competitive.programming.gametheory.common.IScoreConverter;
import competitive.programming.gametheory.common.TreeNode;
import competitive.programming.gametheory.common.TreeNodeSorter;
import competitive.programming.timemanagement.TimeoutException;
import competitive.programming.timemanagement.Timer;

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
public class TreeSearch<M extends IMove<G>, G extends IGame> {
    private final Timer timer;
    private final double depthPenaltyFactor;
    private final TreeNodeSorter<M, G> sorter;
    private final PriorityQueue<TreeSearchNode<M, G>> toBeExpanded;
	private int evaluationsPerformed;
    private int evaluationsMax = 0;
    private List<TreeSearchNode<M, G>> rootNodes;
    private TreeSearchNode<M, G> best;

    static class TreeSearchNode<M, G extends IGame> extends TreeNode<M, G> {
        private TreeSearchNode<M, G> father;
        private double[] subTreeValue;
        private final double depthPenaltyFactor;
        double eval;
        private List<TreeSearchNode<M, G>> subNodes;

        public TreeSearchNode(double[] evaluation, M move, G game, int depth, TreeSearchNode<M, G> father, double depthPenaltyFactor, double eval) {
            super(evaluation, move, game, depth);
            this.father = father;
            this.depthPenaltyFactor = depthPenaltyFactor;
            subTreeValue = evaluation;
            this.eval = eval*Math.pow(depthPenaltyFactor, depth);
        }

        @Override
        public String toString() {
            return "TreeSearchNode{subTree:" + Arrays.toString(subTreeValue)+ ",Evaluation="+Arrays.toString(getEvaluation()) +",Eval:"+eval+",Player:"+ getGame().currentPlayer()+ ",Move=" + getMove().toString() + ", depth=" + getDepth() + "}";
        }

        public void backPropagate(double[] subNodeValue, TreeNodeSorter<M, G> sorter, boolean backPropagateToFather) {
            if (subTreeValue==null || sorter.isBetter(subNodeValue, 1.0, getGame().currentPlayer(), subTreeValue, 1.0, getGame().currentPlayer())) {
                subTreeValue = subNodeValue;
                if (father != null && backPropagateToFather) {
                    father.backPropagate(subNodeValue, sorter, backPropagateToFather);
                }
            }
        }
        
        @Override 
        public void decrementDepth() {
        	super.decrementDepth();
        	eval *= Math.pow(depthPenaltyFactor, getDepth())/Math.pow(depthPenaltyFactor, getDepth()+1);
        };

        public void setSubNodes(List<TreeSearchNode<M, G>> subNodes) {
            this.subNodes = subNodes;
        }

		public void resetEvaluation(TreeNodeSorter<M, G> sorter) {
			boolean resetFather = false;
			
			if (father!=null && Arrays.equals(father.subTreeValue, subTreeValue)){
				resetFather = true;
			}
			subTreeValue = null;
			for (TreeSearchNode<M,G> subNode : subNodes){
				backPropagate(subNode.subTreeValue, sorter, !resetFather);
			}
			if (resetFather){
				father.resetEvaluation(sorter);
			}
		}
    }

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
    public TreeSearch(Timer timer, double depthPenaltyFactor, IScoreConverter converter) {
        this.timer = timer;
        this.depthPenaltyFactor = depthPenaltyFactor;
        this.sorter = new TreeNodeSorter<>(converter);
        this.toBeExpanded = createToBeExpanded();
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
    public M best(final G game, final IMoveGenerator<M, G> generator) {
        toBeExpanded.clear();
        evaluationsPerformed = 0;
        rootNodes = new ArrayList<>();
        try {
        	double[] eval = game.evaluate(0);
            TreeSearchNode<M, G> root = new TreeSearchNode<>(eval, null, game, 0, null, depthPenaltyFactor, sorter.converter.convert(eval, game.currentPlayer()));
            rootNodes = expansion(root, generator);
            root.setSubNodes(rootNodes);
            treeSearchLoop(generator);
        } catch (TimeoutException e) {
            // Times up!
        }

        return returnCurrentBest(game.currentPlayer());
    }

	/**
	 * For testing purpose
	 * */
    protected Queue<TreeSearchNode<M, G>> getToBeExpanded() {
		return toBeExpanded;
	}
    
    /**
     * Print the entire tree representation to the PrintStream.
     * 
     * Particularly useful to understand why the best move has been chosen
     * @param out
     * 		  A print stream such as System.err for example
     */
    public void print(PrintStream out) {
        if (rootNodes != null) {
            for (TreeSearchNode<M, G> node : rootNodes) {
                printNode(node, out);
            }
        }
    }

    /**
     * @return the best game state corresponding to the best move returned by
     *         best method It is mandatory to run best method first!
     */
    public G bestGame() {
        if (best == null)
            return null;
        return best.getGame();
    }

    
    /**
     * @param evaluationsMax limit the number of node to evaluate.
     * 
     * This is nice for testing because you don't want to rely on your computer performances
     */
    public void setEvaluationsMax(int evaluationsMax) {
        this.evaluationsMax = evaluationsMax;
    }

    /**
     * @return the total count of evaluations performed. Useful for performances stats :)
     */
    public int evaluations() {
        return evaluationsPerformed;
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
	public void prun(M executedMove, IMoveGenerator<M, G> generator) {
		TreeSearchNode<M,G> newRoot = null;
		
		for (TreeSearchNode<M,G> rootNode: rootNodes){
			if (rootNode.getMove()==executedMove){
				newRoot = rootNode;
			}
		}
		toBeExpanded.clear();
		best = null;
		evaluationsPerformed=0;
		rootNodes = newRoot.subNodes;
		newRoot.father = null;
		if (rootNodes==null){
			try {
				rootNodes = expansion(newRoot, generator);
			} catch (TimeoutException e) {
			}
		}
		for (TreeSearchNode<M,G> rootNode: rootNodes){
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
    public M continueBest(IMoveGenerator<M, G> generator) {
		evaluationsPerformed=0;
		try {
			treeSearchLoop(generator);
		} catch (TimeoutException e) {
			//Time out
		}
		return returnCurrentBest(rootNodes.get(0).father.getGame().currentPlayer());
	}

    
	/**
	 * @return the double [] of the best evaluation so far.
	 */
	public double[] bestEval() {
		return best.subTreeValue;
	}

	private PriorityQueue<TreeSearchNode<M, G>> createToBeExpanded() {
		return new PriorityQueue<>((o1, o2)->Double.compare(o2.eval, o1.eval));
	}

    private List<TreeSearchNode<M, G>> expansion(TreeSearchNode<M, G> toExpand, IMoveGenerator<M, G> generator) throws TimeoutException {
        int depth = toExpand.getDepth();
        G game = toExpand.getGame();
        List<TreeSearchNode<M, G>> subNodes = new ArrayList<>();
        List<M> moves = generator.generateMoves(game);
        for (M move : moves) {
            TreeSearchNode<M, G> node = evaluate(move.execute(game), move, depth + 1, toExpand);
			subNodes.add(node);
			pushInToBeExpanded(node);	
        }
        
        return subNodes;
    }

	private void pushInToBeExpanded(TreeSearchNode<M, G> node) {
		toBeExpanded.add(node);		
	}

    private TreeSearchNode<M, G> evaluate(G newNodeState, M move, int depth, TreeSearchNode<M, G> father) throws TimeoutException {
        evaluationsPerformed++;
        if (father != null && newNodeState == father.getGame()) {
            throw new IllegalArgumentException(
                    "Your game state is not duplicated! Tree search require to duplicate the game state since it will explore the tree incrementally");
        }
        if (evaluationsMax > 0 && evaluationsPerformed > evaluationsMax) {
            throw new TimeoutException();
        }
        double[] eval = newNodeState.evaluate(depth);
        return new TreeSearchNode<>(eval, move, newNodeState, depth, father, depthPenaltyFactor, sorter.converter.convert(eval, newNodeState.currentPlayer()));
    }

	private void treeSearchLoop(final IMoveGenerator<M, G> generator) throws TimeoutException {
		while (!toBeExpanded.isEmpty()) {
		    timer.timeCheck();
		    TreeSearchNode<M, G> toExpand = toBeExpanded.poll();
		    List<TreeSearchNode<M, G>> expandeds = expansion(toExpand, generator);
		    
		    toExpand.setSubNodes(expandeds);
		    if (!expandeds.isEmpty()){
		    	toExpand.resetEvaluation(sorter);
		    }
		}
	}

	private M returnCurrentBest(int currentPlayer) {
		if (rootNodes.isEmpty())
			return null;
		best = rootNodes.get(0);
        for (TreeSearchNode<M, G> node : rootNodes) {
            if (sorter.isBetter(node.subTreeValue, 1.0, currentPlayer, best.subTreeValue, 1.0, currentPlayer)) {
                best = node;
            }
        }

        return best.getMove();
	}

	private void repushToBeExpandedNodes(TreeSearchNode<M, G> node) {
		node.decrementDepth();
		if (node.subNodes!=null){
			for (TreeSearchNode<M, G> subNode: node.subNodes){
				repushToBeExpandedNodes(subNode);
			}
		}
		else{
			pushInToBeExpanded(node);
		}
	}

	private void printNode(TreeSearchNode<M, G> node, PrintStream out) {
        for (int i = 0; i < node.getDepth(); i++) {
            out.print("\t");
        }
        out.println(node.toString());
        if (node.subNodes != null) {
            node.subNodes.stream().forEach(n -> printNode(n, out));
        }
    }
}
