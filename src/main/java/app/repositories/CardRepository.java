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

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class CardRepository {

    CardDao cardDao;
    UserDao userDao;

    public CardRepository(CardDao cardDao, UserDao userDao) {
        setCardDao(cardDao);
        setUserDao(userDao);
    }

    public ArrayList<Card> getStack(String token) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            return new ArrayList<>(getCardDao().readCardsByUserID(
                    user.getUser_id().toString()).values());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }

    public ArrayList<Card> getDeck(String token) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            return new ArrayList<>(getCardDao().readCardsByDeckID(
                    user.getUser_id().toString()).values());
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }

    public boolean setDeck(String token, ArrayList<String> cards) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken",""));
            if (cards.size() == 4) {
                for (String card_id : cards) {
                    getCardDao().addCardToDeck(card_id, user.getUser_id().toString());
                }
                return true;
            }
            return false;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

}
