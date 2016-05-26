package competitive.programming.gametheory;

import competitive.programming.gametheory.IGame;


public class StickGame implements IGame {
    private int player;
    private int sticksRemaining;

    public StickGame(int currentPlayer, int sticksRemaining) {
        this.player = currentPlayer;
        this.setSticksRemaining(sticksRemaining);
    }

    private void assignEvaluation(double[] evaluation, double eval) {
        if (player == 0) {
            evaluation[0] = -eval;
            evaluation[1] = eval;
        } else {
            evaluation[0] = eval;
            evaluation[1] = -eval;
        }
    }

    public void changePlayer() {
        player = (player + 1) % 2;
    }

    @Override
    public int currentPlayer() {
        return player;
    }

    @Override
    public double[] evaluate(int depth) {
        final double[] evaluation = new double[] { 0, 0 };
        if (getSticksRemaining() == 0) {
            //Player lost.
            assignEvaluation(evaluation, -100);
        } else {
            if (getSticksRemaining() % 4 == 1) {
                //If the opponent plays well, he will lose
                assignEvaluation(evaluation, 1);
            } else {
                //player can win, it is a valuable advantage
                assignEvaluation(evaluation, -1);
            }
        }
        //System.out.println("Evaluation:" + Arrays.toString(evaluation));
        return evaluation;
    }

    public int getSticksRemaining() {
        return sticksRemaining;
    }

    public void setSticksRemaining(int sticksRemaining) {
        this.sticksRemaining = sticksRemaining;
    }
}