package app.model;

import java.util.ArrayList;

public class Battle {
    private int battle_id;
    private User user1;
    private User user2;


    /**fightLog: round, player, card, win/loose*/
    private ArrayList<FightLog> fightLog;

    public Battle(User user1, User user2) {
        this.user1 = user1;
        this.user2 = user2;
    }

    public void startFight() {
        for (int i = 1; i < 5; i++) {
            if (user1.selectCard().damage < user2.selectCard().damage) {
                // user1 wins
                this.fightLog.add(new FightLog(i, user1, user2, 1));
                /**transaction -> */
                user1.removeCard();
                user2.addCard(user1.getCurrentCard());
            } else if (user1.selectCard().damage > user2.selectCard().damage) {
                this.fightLog.add(new FightLog(i, user1, user2, 2));

                user2.removeCard();
                user1.addCard(user2.getCurrentCard());

            } else {
                // draw
                this.fightLog.add(new FightLog(i, user1, user2, 0));
            }
        }
    }
}
