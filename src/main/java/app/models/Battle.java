package app.models;

import app.dtos.UserDTO;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class Battle {
    private UUID battle_id;
    private UUID user1_id;
    private UUID user2_id;
    private UUID winner_id;

    public Battle () {}
}
