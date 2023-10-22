package app.models;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class BattleLogEntry {
    private int battleRound;
    private UUID user1_id;
    private UUID user2_id;
    private UUID user1card_id;
    private UUID user2card_id;
    private int winner;

    public BattleLogEntry(int battleRound, UUID user1_id, UUID user2_id, UUID user1card_id, UUID user2card_id) {
        setBattleRound(battleRound);
        setUser1_id(user1_id);
        setUser2_id(user2_id);
        setUser1card_id(user1card_id);
        setUser2card_id(user2card_id);
    }
}
