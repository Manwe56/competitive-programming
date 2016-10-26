package competitive.programming.gametheory;

import java.util.ArrayList;
import java.util.List;

import competitive.programming.gametheory.IMoveGenerator;

public class StickGenerator implements IMoveGenerator<StickMove, StickGame> {

    @Override
    public List<StickMove> generateMoves(StickGame game) {
        final List<StickMove> moves = new ArrayList<StickMove>();

        if (game.getSticksRemaining() > 2) {
            moves.add(new StickMove(3));
        }
        if (game.getSticksRemaining() > 1) {
            moves.add(new StickMove(2));
        }
        if (game.getSticksRemaining() > 0) {
            moves.add(new StickMove(1));
        }
        return moves;
    }
}
