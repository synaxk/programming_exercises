package app.models;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.Random;

public class Deck {
    @Setter
    @Getter
    private int deck_id;
    private String name;
    private ArrayList<Card> cards;

    public Deck() {
    }

    public void removeCard(Card card) {
        this.cards.remove(card);
    }

    public void addCard(Card newCard) {
        this.cards.add(newCard);
    }

    public Card getRandomCard() {
        Random rnd = new Random();
        return this.cards.get(rnd.nextInt(0));
    }

}
