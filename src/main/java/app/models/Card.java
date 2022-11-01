package app.model;


import lombok.Getter;
import lombok.Setter;
@Getter
@Setter
public abstract class Card {

    private int card_id;
    private String name;
    int damage;

}
