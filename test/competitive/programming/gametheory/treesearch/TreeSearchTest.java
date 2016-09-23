package competitive.programming.gametheory.treesearch;

import org.junit.Test;

import competitive.programming.gametheory.StickGame;
import competitive.programming.gametheory.StickMove;
import competitive.programming.gametheory.Tester;
import competitive.programming.timemanagement.Timer;

public class TreeSearchTest {

    @Test
    public void testStickGame() {
        final Timer timer = new Timer();
        final TreeSearch<StickMove, StickGame> treeSearch = new TreeSearch<StickMove, StickGame>(timer, 0.5, (rawScores, player) -> rawScores[player]);

        Tester.testAlgo((game, generator, maxdepth) -> treeSearch.best(game, generator), true);
    }

}
