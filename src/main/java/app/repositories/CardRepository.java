package app.repositories;

import app.daos.CardDao;
import app.daos.UserDao;
import app.models.Card;
import app.models.User;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class CardRepository {

    CardDao cardDao;
    UserDao userDao;

    public CardRepository(CardDao cardDao, UserDao userDao) {
        setCardDao(cardDao);
        setUserDao(userDao);
    }

    public ArrayList<Card> getStackList(String token) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            return new ArrayList<>(getCardDao().readStackCardsByUserID(
                    user.getUser_id().toString()).values());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }
    public HashMap<UUID,Card> getStack(String token) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            return getCardDao().readStackCardsByUserID(user.getUser_id().toString());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }

    public ArrayList<Card> getDeckList(String token) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            return new ArrayList<>(getCardDao().readDeckCardsByUserID(
                    user.getUser_id().toString()).values());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }
    public HashMap<UUID, Card> getDeck(String token) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            return getCardDao().readDeckCardsByUserID(user.getUser_id().toString());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }

    public HashMap<UUID, Card> getDeckByID(String user_id) {
        try {
            return getCardDao().readDeckCardsByUserID(user_id);
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }


    public void addCardToStack(String card_id, String user_id) {
        try {
            getCardDao().addCardToStack(card_id, user_id);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    public void removeCardFromStack(String card_id, String user_id) {
        try {
            getCardDao().removeCardFromStack(card_id, user_id);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    public void removeCardFromDeck(String card_id, String user_id) {
        try {
            getCardDao().removeCardFromDeck(card_id, user_id);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public boolean setDeck(String token, ArrayList<String> cards) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken",""));
            HashMap<UUID, Card> currentDeck = getDeck(token);
            HashMap<UUID, Card> stack = getStack(token);

            for (String card_id : cards) {
                if (!stack.containsKey(UUID.fromString(card_id))) {
                    return false;
                }
                getCardDao().addCardToDeck(card_id, user.getUser_id().toString());
            }
            // clear current deck
            if (!currentDeck.isEmpty()) {
                for (Map.Entry<UUID, Card> ent : currentDeck.entrySet()) {
                    this.removeCardFromDeck(ent.getKey().toString(), user.getUser_id().toString());
                }
            }
            return true;

        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }


}
