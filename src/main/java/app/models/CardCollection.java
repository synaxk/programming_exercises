package app.model;

import java.util.ArrayList;

public abstract class CardCollection {
    protected ArrayList<Card> cards;
    protected int limit;

    public CardCollection() {

    }

    public CardCollection(int limit) {
        this.limit = limit;
        this.cards = new ArrayList<>(limit);
    }

    public boolean addCard(Card newCard) {
        this.cards.add(newCard);
        return true;
    }
    public void removeCard(Card card) {
        this.cards.remove(card);
    }


}
