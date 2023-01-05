package app.models;


import com.fasterxml.jackson.annotation.JsonAlias;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class Card {
    @JsonAlias({"Card_id"})
    private UUID card_id;
    @JsonAlias({"Name"})
    private String name;
    @JsonAlias({"Damage"})
    int damage;

    public Card() {}

}
