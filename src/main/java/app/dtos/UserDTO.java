package app.dtos;

import app.models.Deck;
import app.models.Stack;
import com.fasterxml.jackson.annotation.JsonAlias;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class UserDTO {
    @JsonAlias({"user_id"})
    UUID user_id;
    @JsonAlias({"username"})
    String username;
    @JsonAlias({"password"})
    String password;
    @JsonAlias({"coins"})
    int coins;
    @JsonAlias({"stack"})
    Stack stack;
    @JsonAlias({"deck"})
    Deck deck;

    public UserDTO() {}
}
