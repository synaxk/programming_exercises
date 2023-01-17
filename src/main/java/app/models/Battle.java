package app.models;

import app.dtos.UserDTO;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.*;
import java.util.concurrent.CountDownLatch;

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
    private UserRepository userRepository;
    private CountDownLatch battleLatch = new CountDownLatch(1);
    private ObjectMapper objectMapper = new ObjectMapper();
    public Battle () {}

    @JsonCreator
    public Battle(@JsonProperty("User1") UserDTO user1, CardRepository cardRepository, UserRepository userRepository) {
        this.battle_id = UUID.randomUUID();
        this.cardRepository = cardRepository;
        this.userRepository = userRepository;
        this.user1 = user1;
        this.winner_id = null;
        this.rounds = 0;
        this.user1wins = 0;
        this.user2wins = 0;
    }

    public void start() {
        this.user1.setBattleLog(new ArrayList<>());
        this.user2.setBattleLog(new ArrayList<>());
        while ((user1.getDeck().size() > 0 && user2.getDeck().size() > 0) && this.rounds < 100) {
            this.battleRound(getRndCard(getUser1().getDeck()), getRndCard(getUser2().getDeck()));
        }
        if (this.user1wins > this.user2wins) {
            this.winner_id = user1.getUser_id();
            user1.setScore(user1.getScore() + 3);
            user2.setScore(user2.getScore() - 5);
        } else if (this.user1wins < this.user2wins) {
            this.winner_id = user2.getUser_id();
            user1.setScore(user1.getScore() - 5);
            user2.setScore(user2.getScore() + 3);
        } else {
            this.winner_id = null;
        }
        //update scores
        this.updateScore(user1);
        this.updateScore(user2);

        //update cardchanges
        this.updateCardChanges(user1);
        this.updateCardChanges(user2);

        //finish battle
        battleLatch.countDown();
    }

    private void battleRound(Card user1card, Card user2card) {
        this.rounds++;
        BattleLogEntry log = null;
        Card win_card = null;
        if ((win_card = this.compareCards(user1card, user2card)) == null) {
            // draw
            log = new BattleLogEntry(this.rounds, user1.getUser_id(), user2.getUser_id(),
                    user1card.getCard_id(), user2card.getCard_id(), 0);
            getUser1().getBattleLog().add(log);
            getUser2().getBattleLog().add(log);
        } else if (win_card.equals(user1card)) {
            //user 1 card wins round
            this.user1wins++;
            //remove card from user2
            getUser2().getDeck().remove(user2card.getCard_id());
            //add card to user1
            getUser1().getDeck().put(user2card.getCard_id(), user2card);
            //set battle log entries
            log = new BattleLogEntry(this.rounds, user1.getUser_id(), user2.getUser_id(),
                    user1card.getCard_id(), user2card.getCard_id(), 1);
            getUser1().getBattleLog().add(log);
            getUser2().getBattleLog().add(log);
        } else if (win_card.equals(user2card)) {
            this.user2wins++;
            //remove card from user1
            getUser1().getDeck().remove(user1card.getCard_id());
            //add card to user2
            getUser2().getDeck().put(user1card.getCard_id(), user1card);
            //set battle log entries
            log = new BattleLogEntry(this.rounds, user1.getUser_id(), user2.getUser_id(),
                    user1card.getCard_id(), user2card.getCard_id(), 2);
            getUser1().getBattleLog().add(log);
            getUser2().getBattleLog().add(log);
        }
        // log to console
        try {
            System.out.println(getObjectMapper().writeValueAsString(log));
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }
    }

    private Card compareCards(Card user1card, Card user2card) {
        //Pure MonsterFight
        if (user1card.getCardType().getCategory().equals("Monster") &&
                user2card.getCardType().getCategory().equals("Monster")) {
            //check for goblin / dragon -> dragon wins
            if (user1card.getName().contains("Goblin") && user2card.getName().contains("Dragon")) {
                return user2card;
            } else if (user1card.getName().contains("Dragon") && user2card.getName().contains("Goblin")) {
                return user1card;

            //check for wizard / ork -> wizard wins
            } else if (user1card.getName().equals("Wizard") && user2card.getName().equals("Ork")) {
                return user1card;
            } else if (user1card.getName().equals("Ork") && user2card.getName().equals("Wizard")) {
                return user2card;

            //check for FireElves / Dragon -> FireElf wins
            } else if (user1card.getName().equals("FireElf") && user2card.getName().contains("Dragon")) {
                return user1card;
            } else if (user1card.getName().contains("Dragon") && user2card.getName().contains("FireElf")) {
                return user2card;
            }

        // monster vs spell // spell vs spell
        } else {
            // check for Knight vs WaterSpell
            if (user1card.getName().equals("Knight") && user2card.getName().equals("WaterSpell")) {
                return user2card;
            } else if (user1card.getName().equals("WaterSpell") && user2card.getName().equals("Knight")) {
                return user1card;
            }

            // handle element effectiveness
            // water -> fire
            if (user1card.getCardType().getElement().equals("water") && user2card.getCardType().getElement().equals("fire")) {
                user1card.setDamage(user1card.getDamage() * 2);
                user2card.setDamage(user2card.getDamage() / 2);
            } else if (user1card.getCardType().getElement().equals("fire") && user2card.getCardType().getElement().equals("water")) {
                user1card.setDamage(user1card.getDamage() / 2);
                user2card.setDamage(user2card.getDamage() * 2);

            // fire -> normal
            } else if (user1card.getCardType().getElement().equals("fire") && user2card.getCardType().getElement().equals("normal")) {
                user1card.setDamage(user1card.getDamage() * 2);
                user2card.setDamage(user2card.getDamage() / 2);
            } else if (user1card.getCardType().getElement().equals("normal") && user2card.getCardType().getElement().equals("fire")) {
                user1card.setDamage(user1card.getDamage() / 2);
                user2card.setDamage(user2card.getDamage() * 2);

            // normal -> water
            } else if (user1card.getCardType().getElement().equals("normal") && user2card.getCardType().getElement().equals("water")) {
                user1card.setDamage(user1card.getDamage() * 2);
                user2card.setDamage(user2card.getDamage() / 2);
            } else if (user1card.getCardType().getElement().equals("water") && user2card.getCardType().getElement().equals("normal")) {
                user1card.setDamage(user1card.getDamage() / 2);
                user2card.setDamage(user2card.getDamage() * 2);
            }
        }
        // compare card damage
        if (user1card.getDamage() > user2card.getDamage()) {
            return user1card;
        } else if (user1card.getDamage() < user2card.getDamage()) {
            return user2card;
        }
        return null;
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

    private void updateScore(UserDTO user) {
        getUserRepository().updateUser(user.getUser_id().toString(), Map.of("score", String.valueOf(user.getScore())));
    }
    private void removeCard(UserDTO user, Card card) {
    }


}
