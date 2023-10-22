package app.models;

import com.fasterxml.jackson.annotation.JsonAlias;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class User {
    @JsonAlias({"User_id"})
    private UUID user_id;
    @JsonAlias({"Username"})
    private String username;
    @JsonAlias({"Password"})
    private String password;
    @JsonAlias({"Score"})
    private int score;
    @JsonAlias({"Coins"})
    private int coins;
    @JsonAlias({"Bio"})
    private String bio;
    @JsonAlias({"Image"})
    private String image;

    @JsonAlias({"Wins"})
    private int wins;
    @JsonAlias({"Losses"})
    private int losses;
    @JsonCreator
    public User(@JsonProperty("Username") String name, @JsonProperty("Password") String password) {
        this.username = name;
        this.password = password;
        this.coins = 20; // check default
        this.score = 100;
    }

    public User(UUID user_id,String name, String password) {
        this.user_id = user_id;
        this.username = name;
        this.password = password;
    }

}

