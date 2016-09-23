package competitive.programming.gametheory.minimax;

import org.junit.Test;

import competitive.programming.gametheory.StickGame;
import competitive.programming.gametheory.StickMove;
import competitive.programming.gametheory.Tester;
import competitive.programming.timemanagement.Timer;

public class MinimaxTest {
    @Test
    public void testStickGame() {
        final Timer timer = new Timer();
        final Minimax<StickMove, StickGame> minimax = new Minimax<StickMove, StickGame>(timer);

        Tester.testAlgo((game, generator, maxdepth) -> minimax.best(game, generator, maxdepth), false);
    }
}
