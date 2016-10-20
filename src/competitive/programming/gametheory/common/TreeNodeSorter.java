package competitive.programming.gametheory.common;

import java.util.Collections;
import java.util.List;

public class TreeNodeSorter<M, G> {
    public final IScoreConverter converter;

    public TreeNodeSorter(IScoreConverter converter) {
        this.converter = converter;
    }

    public TreeNode<M, G> best(List<? extends TreeNode<M, G>> moves, int playerId) {
        Collections.sort(moves, (n1, n2) -> compare(n1.getEvaluation(), 1.0, playerId, n2.getEvaluation(), 1.0, playerId, converter));
        return moves.get(0);
    }

    private static int compare(double[] scores1, double evaluation1Factor, int player1Id, double[] scores2, double evaluation2Factor, int player2Id,
            IScoreConverter converter) {
        final double diff = converter.convert(scores1, player1Id) * evaluation1Factor - converter.convert(scores2, player2Id) * evaluation2Factor;
        if (diff < 0) {
            return 1;
        }
        if (diff > 0) {
            return -1;
        }
        return 0;
    }

    public int compare(double[] evaluation1, double evaluation1Factor, int player1Id, double[] evaluation2, double evaluation2Factor, int player2Id) {
        return compare(evaluation1, evaluation1Factor, player1Id, evaluation2, evaluation2Factor, player2Id, converter);
    }

    public boolean isBetter(double[] evaluation1, double evaluation1Factor, int player1Id, double[] evaluation2, double evaluation2Factor, int player2Id) {
        return compare(evaluation1, evaluation1Factor, player1Id, evaluation2, evaluation2Factor, player2Id, converter) < 0;
    }
}