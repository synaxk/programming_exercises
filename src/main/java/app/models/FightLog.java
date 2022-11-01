package app.model;

import lombok.Getter;
import lombok.Setter;


public class FightLog {
    @Getter
    @Setter
    private int round;
    private User user1;
    private User user2;

    private int winner;

    public FightLog(int round, User user1, User user2, int winner) {
        this.round = round;
        this.user1 = user1;
        this.user2 = user2;
        this.winner = winner;
    }

    @Override
    public String toString() {
        return "FightLog{" +
                "round=" + round +
                ", user1=" + user1.getUser_id() +
                ", user1Card=" + user1.getCurrentCard().getCard_id() +
                ", user2=" + user2.getUser_id() +
                ", user2Card=" + user1.getCurrentCard().getCard_id() +
                ", result=" + winner +
                '}';
    }
}
