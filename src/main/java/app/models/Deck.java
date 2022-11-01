package app.model;

import lombok.Getter;
import lombok.Setter;

import java.util.Random;

public class Deck extends CardCollection {
    @Setter
    @Getter
    private int deck_id;
    private String name;

    public Deck() {
        super(4);
    }

    public void removeCard(Card card) {
        this.cards.remove(card);
    }

    public boolean addCard(Card newCard) {
        if (cards.size() <= limit) {
            this.cards.add(newCard);
            return true;
        }
        return false;
    }

    public Card getRandomCard() {
        Random rnd = new Random();
        return this.cards.get(rnd.nextInt(0,limit));
    }

}
