package app.models;

import app.daos.UserDao;
import app.dtos.UserDTO;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.*;

@Getter
@Setter
@AllArgsConstructor
public class Battle {
    private UUID battle_id;
    private UserDTO user1;
    private UserDTO user2;
    private UUID winner_id;
    private int rounds;
    private int user1wins;
    private int user2wins;
    private CardRepository cardRepository;

    public Battle () {}

    @JsonCreator
    public Battle(@JsonProperty("User1") UserDTO user1, UserRepository userRepository) {
        this.battle_id = UUID.randomUUID();
        this.user1 = user1;
        this.winner_id = null;
        this.rounds = 0;
        this.user1wins = 0;
        this.user2wins = 0;
    }

    public void start() {
        this.user1.setBattleLog(new ArrayList<>());
        this.user2.setBattleLog(new ArrayList<>());
        while ((user1.getDeck().size() > 1 && user2.getDeck().size() > 1) && this.rounds < 150) {
            this.battleRound(getRndCard(getUser1().getDeck()), getRndCard(getUser2().getDeck()));
        }
    }

    private void battleRound(Card user1card, Card user2card) {
        this.rounds++;
        BattleLogEntry log;
        if (user1card.getDamage() > user2card.getDamage()) {
            //user 1 card wins round
            this.user1wins++;
            //remove card from user2
            getUser2().getDeck().remove(user2card.getCard_id());
            //add card to user1
            getUser1().getDeck().put(user2card.getCard_id(), user2card);
            //set battle log entries
            log = new BattleLogEntry(this.rounds, user1.getUser_id(),user2.getUser_id(),
                    user1card.getCard_id(),user2card.getCard_id(),1);
            getUser1().getBattleLog().add(log);
            getUser2().getBattleLog().add(log);

        } else if (user1card.getDamage() < user2card.getDamage()) {
            //user 2 card wins round
            this.user2wins++;
            //remove card from user1
            getUser1().getDeck().remove(user1card.getCard_id());
            //add card to user2
            getUser2().getDeck().put(user1card.getCard_id(), user1card);
            //set battle log entries
            log = new BattleLogEntry(this.rounds, user1.getUser_id(),user2.getUser_id(),
                    user1card.getCard_id(),user2card.getCard_id(),2);
            getUser1().getBattleLog().add(log);
            getUser2().getBattleLog().add(log);

        }
        //else draw
        log = new BattleLogEntry(this.rounds, user1.getUser_id(),user2.getUser_id(),
                user1card.getCard_id(),user2card.getCard_id(),0);
        getUser1().getBattleLog().add(log);
        getUser2().getBattleLog().add(log);
    }


    private Card getRndCard(HashMap<UUID, Card> map) {
        Random rnd = new Random();
        List<UUID> keys = new ArrayList<>(map.keySet());
        return map.get(keys.get(rnd.nextInt(keys.size())));
    }


    //add / remove the new deck cards to/from stack and reset decks
    private void updateCardChanges(UserDTO user) {
        HashMap<UUID, Card> oldDeck = getCardRepository().getDeckByID(user.getUser_id().toString());
        //check if cards of current deck are in old deck -> add, if not
        for (Map.Entry<UUID, Card> ent : user.getDeck().entrySet()) {
            if (!oldDeck.containsKey(ent.getKey())){
                //new card -> add to Stack
                getCardRepository().addCardToStack(ent.getKey().toString(),
                        user.getUser_id().toString());
            }
        }
        //check if cards of old deck are still in new deck, remove if not
        for (Map.Entry<UUID, Card> ent : oldDeck.entrySet()) {
            if (!user.getDeck().containsKey(ent.getKey())) {
                //card lost, remove from stack
                getCardRepository().removeCardFromStack(ent.getKey().toString(),
                        user.getUser_id().toString());
            }
        }

    }
    private void removeCard(UserDTO user, Card card) {
    }


}
