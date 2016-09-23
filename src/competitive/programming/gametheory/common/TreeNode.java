package competitive.programming.gametheory.common;

import java.util.Arrays;

public class TreeNode<M, G> {
    private final double[] evaluation;
    private final M move;
    private final G game;
    private final int depth;

    public TreeNode(double[] evaluation, M move, G game, int depth) {
        this.evaluation = evaluation;
        this.move = move;
        this.game = game;
        this.depth = depth;
    }

    public int getDepth() {
        return depth;
    }

    public double[] getEvaluation() {
        return evaluation;
    }

    public M getMove() {
        return move;
    }

    public G getGame() {
        return game;
    }

    @Override
    public String toString() {
        return "TreeNode [evaluation=" + Arrays.toString(evaluation) + ", move=" + move + ", depth=" + depth + "]";
    }
}