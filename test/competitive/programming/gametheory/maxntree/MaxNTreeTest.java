package competitive.programming.gametheory.maxntree;

import org.junit.Test;

import competitive.programming.gametheory.StickGame;
import competitive.programming.gametheory.StickMove;
import competitive.programming.gametheory.Tester;
import competitive.programming.gametheory.maxntree.MaxNTree;
import competitive.programming.timemanagement.Timer;

public class MaxNTreeTest {

    @Test
    public void testStickGame() {
        final Timer timer = new Timer();
        final MaxNTree<StickMove, StickGame> maxNTree = new MaxNTree<StickMove, StickGame>(timer, (rawScores, player) -> rawScores[player]);

        Tester.testAlgo((game, generator, maxdepth) -> maxNTree.best(game, generator, maxdepth));
    }

}
