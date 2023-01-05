package app.repositories;

import app.dtos.ScoreEntryDTO;
import app.models.User;
import app.daos.UserDao;
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
public class UserRepository {
    UserDao userDao;
    HashMap<UUID, User> userCache = new HashMap<>();

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

    public ArrayList<ScoreEntryDTO> readScores() {
        try {
            ArrayList<User> users = this.getAll();
            ArrayList<ScoreEntryDTO> scores = new ArrayList<>();

            for (User user : users) {
                ScoreEntryDTO score = new ScoreEntryDTO(user.getUser_id(), user.getUsername(), user.getScore());
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
