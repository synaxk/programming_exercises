package app.dtos;

import com.fasterxml.jackson.annotation.JsonAlias;
import lombok.AccessLevel;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter(AccessLevel.PRIVATE)
@AllArgsConstructor
public class ScoreEntryDTO {
    @JsonAlias({"User_id"})
    private UUID user_id;
    @JsonAlias({"Username"})
    private String username;
    @JsonAlias({"Score"})
    private int score;
    @JsonAlias({"Wins"})
    private int wins;
    @JsonAlias({"Losses"})
    private int losses;

    public ScoreEntryDTO() {}
}
