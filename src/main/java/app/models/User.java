package app.model;

import lombok.Getter;
import lombok.Setter;

public class User {
    @Getter
    @Setter
    private int user_id;
    private String name;
    private String email;
    private String password;
    private int coins;
    private Card currentCard;
    private Deck deck;
    private Stack stack;

    public User(String name, String email) {

    }

    public Card getCurrentCard() {
        return this.currentCard;
    }

    public Card selectCard(){
        return this.currentCard = this.deck.getRandomCard();
    }

    public void removeCard() {
        this.deck.removeCard(this.currentCard);
        this.stack.removeCard(this.currentCard);
    }

    public void addCard(Card card) {
        this.stack.addCard(card);
    }

}
