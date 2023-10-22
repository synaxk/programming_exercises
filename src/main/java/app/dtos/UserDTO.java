package app.dtos;

import app.models.BattleLogEntry;
import app.models.Card;
import com.fasterxml.jackson.annotation.JsonAlias;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class UserDTO {
    @JsonAlias({"user_id"})
    UUID user_id;
    @JsonAlias({"username"})
    String username;
    @JsonAlias({"score"})
    int score;
    @JsonAlias({"stack"})
    HashMap<UUID, Card> stack;
    @JsonAlias({"deck"})
    HashMap<UUID, Card> deck;
    @JsonAlias({"BattleLog"})
    ArrayList<BattleLogEntry> battleLog;
    public UserDTO() {}
}
