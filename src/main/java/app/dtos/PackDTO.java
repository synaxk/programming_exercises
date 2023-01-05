package app.dtos;


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
public class PackDTO {
    @JsonAlias({"Pack_id"})
    private UUID pack_id;
    @JsonAlias({"Name"})
    private String name;
    @JsonAlias({"Price"})
    private int price;
    @JsonAlias({"Cards"})
    private HashMap<UUID, Card> cards;

    public PackDTO() {}
}
