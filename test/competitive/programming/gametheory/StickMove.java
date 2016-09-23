package competitive.programming.gametheory;


public class StickMove implements ICancellableMove<StickGame> {

    private int sticks;
    private StickGame previousGame;

    public StickMove(int sticks) {
        this.setSticks(sticks);
    }

    @Override
    public StickGame cancel(StickGame game) {
        if (game.isGameStateDuplication()) {
            return previousGame;
        }
        game.changePlayer();
        game.setSticksRemaining(game.getSticksRemaining() + getSticks());
        log(game, "cancel ");
        return game;
    }

    @Override
    public StickGame execute(StickGame game) {
        int sticksRemaining = game.getSticksRemaining() - getSticks();
        if (game.isGameStateDuplication()) {
            previousGame = game;
            return new StickGame(1 - game.currentPlayer(), sticksRemaining, true);
        }
        game.setSticksRemaining(sticksRemaining);
        log(game, "execute ");
        game.changePlayer();
        return game;
    }

    public int getSticks() {
        return sticks;
    }

    private void log(StickGame game, String action) {
        // System.out.println("player " + game.currentPlayer() + " " + action + "move " + sticks + " : sticks remaining= " + game.getSticksRemaining());
    }

    public void setSticks(int sticks) {
        this.sticks = sticks;
    }

    @Override
    public String toString() {
        return "Move[" + sticks + "]";
    }
}