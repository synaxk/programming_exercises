package app.repositories;

import app.daos.CardDao;
import app.dtos.ScoreEntryDTO;
import app.dtos.UserDTO;
import app.models.Card;
import app.models.User;
import app.daos.UserDao;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.sql.SQLException;
import java.util.*;

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class UserRepository {
    UserDao userDao;
    CardDao cardDao;
    HashMap<UUID, User> userCache = new HashMap<>();

    public UserRepository(UserDao userDao, CardDao cardDao) {
        setUserDao(userDao);
        setCardDao(cardDao);
    }

    public UserRepository(UserDao userDao) {
        setUserDao(userDao);
    }

    public ArrayList<User> getAll() {
        if (userCache.isEmpty()) {
            try {
                setUserCache(getUserDao().read());
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
        }
        return new ArrayList<>(userCache.values());
    }

    public User getUserByUID(String uuid) {
        if (userCache.isEmpty()) {
            getAll();
        }
        return userCache.get(UUID.fromString(uuid));
    }

    public User getUserByName(String name) {
        try {
            return getUserDao().readByName(name);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public User getUserByToken(String token) {
        try {
            String username = token.replace("-mtcgToken","");
            return getUserDao().readByName(username);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public UserDTO getUserDTOByToken(String token) {
        try {
            String username = token.replace("-mtcgToken","");
            User user = getUserDao().readByName(username);
            HashMap<UUID, Card> stack = getCardDao().readStackCardsByUserID(
                    user.getUser_id().toString());
            HashMap<UUID, Card> deck = getCardDao().readDeckCardsByUserID(
                    user.getUser_id().toString());
            return new UserDTO(user.getUser_id(), user.getUsername(), user.getScore(), stack, deck, null);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public void createUser(User user) {
        try {
            getUserDao().create(user);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public void updateUser(String uuid, Map<String, String> props) {
        try {
            getUserDao().update(uuid, props);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public ScoreEntryDTO readScore(String token) {
        User user = this.getUserByToken(token);
        return new ScoreEntryDTO(user.getUser_id(), user.getUsername(), user.getScore(),
                user.getWins(), user.getLosses());
    }

    public ArrayList<ScoreEntryDTO> readScores() {
        try {
            ArrayList<User> users = this.getAll();
            ArrayList<ScoreEntryDTO> scores = new ArrayList<>();

            for (User user : users) {
                ScoreEntryDTO score = new ScoreEntryDTO(user.getUser_id(), user.getUsername(),
                        user.getScore(), user.getWins(), user.getLosses());
                scores.add(score);
            }
            return scores;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

   /* private User searchUser(String uuid) {
        for (User user : userCache) {
            if (user.getUser_id() == UUID.fromString(uuid)) {
                return user;
            }
        }
        return null;
    }*/
}
