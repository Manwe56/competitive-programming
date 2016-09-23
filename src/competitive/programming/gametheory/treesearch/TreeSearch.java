package competitive.programming.gametheory.treesearch;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.TreeSet;

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
    private final TreeSet<TreeSearchNode<M, G>> toBeExpanded;
    private int evaluationsPerformed;
    private int evaluationsMax = 0;
    private List<TreeSearchNode<M, G>> rootNodes;
    private TreeSearchNode<M, G> best;

    private static class TreeSearchNode<M, G extends IGame> extends TreeNode<M, G> {
        private final TreeSearchNode<M, G> father;
        private double[] subTreeValue;
        private final double evaluationFactor;
        private List<TreeSearchNode<M, G>> subNodes;

        public TreeSearchNode(double[] evaluation, M move, G game, int depth, TreeSearchNode<M, G> father, double depthPenaltyFactor) {
            super(evaluation, move, game, depth);
            this.father = father;
            subTreeValue = evaluation;
            evaluationFactor = Math.pow(depthPenaltyFactor, depth);
        }

        @Override
        public String toString() {
            return "TreeSearchNode{eval:" + Arrays.toString(subTreeValue) + ",Move=" + getMove().toString() + ", depth=" + getDepth() + "}";
        }

        public void backPropagate(double[] subNodeValue, TreeNodeSorter<M, G> sorter) {
            if (sorter.isBetter(subNodeValue, 1.0, getGame().currentPlayer(), subTreeValue, 1.0, getGame().currentPlayer())) {
                subTreeValue = subNodeValue;
                if (father != null) {
                    father.backPropagate(subNodeValue, sorter);
                }
            }
        }

        public void setSubNodes(List<TreeSearchNode<M, G>> subNodes) {
            this.subNodes = subNodes;
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
        this.toBeExpanded = new TreeSet<>((o1, o2) -> {
            if (o2 == o1) {
                return 0;
            }
            int comparison = sorter.compare(o1.getEvaluation(), o1.evaluationFactor, o1.getGame().currentPlayer(), o2.getEvaluation(), o2.evaluationFactor, o2
                    .getGame().currentPlayer());
            if (comparison == 0) {
                int o1hash = System.identityHashCode(o1);
                int o2hash = System.identityHashCode(o2);
                return o1hash - o2hash;
            }
            return comparison;
        });
    }

    private List<TreeSearchNode<M, G>> expansion(TreeSearchNode<M, G> toExpand, IMoveGenerator<M, G> generator) throws TimeoutException {
        int depth = toExpand.getDepth();
        G game = toExpand.getGame();
        List<TreeSearchNode<M, G>> subNodes = new ArrayList<>();
        List<M> moves = generator.generateMoves(game);
        for (M move : moves) {
            subNodes.add(evaluate(move.execute(game), move, depth + 1, toExpand));
        }
        toBeExpanded.addAll(subNodes);
        return subNodes;
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
        return new TreeSearchNode<>(newNodeState.evaluate(depth), move, newNodeState, depth, father, depthPenaltyFactor);
    }

    /**
     * @param game
     *            The current state of the game
     * @param generator
     *            The move generator that will generate all the possible move of
     *            the playing player at each turn
     * @return the best move you can play considering all players are selecting
     *         the best move for them
     * @throws TimeoutException
     */
    public M best(final G game, final IMoveGenerator<M, G> generator) {
        toBeExpanded.clear();
        evaluationsPerformed = 0;
        rootNodes = new ArrayList<>();
        try {
            TreeSearchNode<M, G> root = new TreeSearchNode<>(game.evaluate(0), null, game, 0, null, depthPenaltyFactor);
            rootNodes = expansion(root, generator);
            root.setSubNodes(rootNodes);
            while (!toBeExpanded.isEmpty()) {
                timer.timeCheck();
                TreeSearchNode<M, G> toExpand = selection();
                List<TreeSearchNode<M, G>> expandeds = expansion(toExpand, generator);
                toBeExpanded.remove(toExpand);
                toExpand.setSubNodes(expandeds);
                for (TreeSearchNode<M, G> expanded : expandeds) {
                    toExpand.backPropagate(expanded.getEvaluation(), sorter);
                }
            }
        } catch (TimeoutException e) {
            // Times up!
        }

        best = rootNodes.get(0);
        for (TreeSearchNode<M, G> node : rootNodes) {
            if (sorter.isBetter(node.subTreeValue, 1.0, game.currentPlayer(), best.subTreeValue, 1.0, game.currentPlayer())) {
                best = node;
            }
        }

        if (best == null) {
            return null;
        }
        return best.getMove();
    }

    public void print(PrintStream out) {
        if (rootNodes != null) {
            for (TreeSearchNode<M, G> node : rootNodes) {
                printNode(node, out);
            }
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

    private TreeSearchNode<M, G> selection() {
        return toBeExpanded.first();
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

    public void setEvaluationsMax(int evaluationsMax) {
        this.evaluationsMax = evaluationsMax;
    }

    /**
     * @return the total count of evaluations performed. Useful for performances stats :)
     */
    public int evaluations() {
        return evaluationsPerformed;
    }
}
