package app.models;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class BattleRound {
    private UUID battleRound_id;
    private UUID battle_id;
    private UUID user1_id;
    private UUID user2_id;
    private UUID user1card_id;
    private UUID user2card_id;

    public BattleRound () {}
}
