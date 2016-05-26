package competitive.programming.gametheory;

import competitive.programming.gametheory.IMove;

public class StickMove implements IMove<StickGame> {

    private int sticks;

    public StickMove(int sticks) {
        this.setSticks(sticks);
    }

    @Override
    public StickGame cancel(StickGame game) {
        game.changePlayer();
        game.setSticksRemaining(game.getSticksRemaining() + getSticks());
        log(game, "cancel ");
        return game;
    }

    @Override
    public StickGame execute(StickGame game) {
        game.setSticksRemaining(game.getSticksRemaining() - getSticks());
        log(game, "execute ");
        game.changePlayer();
        return game;
    }

    public int getSticks() {
        return sticks;
    }

    private void log(StickGame game, String action) {
        //System.out.println("player " + game.currentPlayer() + " " + action + "move " + sticks + " : sticks remaining= " + game.getSticksRemaining());
    }

    public void setSticks(int sticks) {
        this.sticks = sticks;
    }

    @Override
    public String toString() {
        return "Move[" + sticks + "]";
    }
}