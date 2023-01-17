package app.daos;

import app.models.User;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import org.springframework.security.crypto.bcrypt.BCrypt;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public class UserDao implements Dao<User>{
    @Getter(AccessLevel.PRIVATE)
    @Setter(AccessLevel.PRIVATE)
    private Connection connection;

    public UserDao(Connection connection) {
        setConnection(connection);
    }

    @Override
    public boolean create(User user) throws SQLException {
        String pw_hash = BCrypt.hashpw(user.getPassword(), BCrypt.gensalt());
        String query = "INSERT INTO users (user_id, username, password, score, coins) VALUES (CAST(? AS UUID), ?, ?, ?, ?)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        if (user.getUser_id() != null) {
            stmt.setString(1, user.getUser_id().toString());
        } else {
            stmt.setString(1, UUID.randomUUID().toString());
        }
        stmt.setString(2, user.getUsername());
        stmt.setString(3, pw_hash);
        stmt.setInt(4, user.getScore());
        stmt.setInt(5, user.getCoins());
        return stmt.execute();
    }

    @Override
    public HashMap<UUID,User> read() throws SQLException {
        HashMap<UUID, User> users = new HashMap<>();
        String query = "SELECT * FROM users";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        ResultSet result = stmt.executeQuery();
        while (result.next()) {
            User user = new User(UUID.fromString(result.getString(1)),
                            result.getString(2),
                            result.getString(3),
                            result.getInt(4),
                            result.getInt(5),
                            result.getString(6),
                            result.getString(7),
                            result.getInt(8),
                            result.getInt(9));
            users.put(user.getUser_id(), user);
        }
        return users;
    }


    public User readByName(String username) throws SQLException {
        String query = "SELECT * FROM users WHERE username=?";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, username);
        ResultSet result = stmt.executeQuery();
        if (result.next()) {
            return new User(UUID.fromString(result.getString(1)),
                    result.getString(2),
                    result.getString(3),
                    result.getInt(4),
                    result.getInt(5),
                    result.getString(6),
                    result.getString(7),
                    result.getInt(8),
                    result.getInt(9));
        }
        return null;
    }

    public User readByID(String user_id) throws SQLException {
        String query = "SELECT * FROM users WHERE user_id=CAST(? AS UUID)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, user_id);
        ResultSet result = stmt.executeQuery();
        if (result.next()) {
            return new User(UUID.fromString(result.getString(1)),
                    result.getString(2),
                    result.getString(3),
                    result.getInt(4),
                    result.getInt(5),
                    result.getString(6),
                    result.getString(7),
                    result.getInt(8),
                    result.getInt(9));
        }
        return null;
    }

    @Override
    public void update(String uuid, Map<String,String> props) throws SQLException {
        for (Map.Entry<String, String> ent : props.entrySet()) {
            String query = "UPDATE users SET " + ent.getKey() + " = ? WHERE user_id = CAST(? AS UUID)";
            PreparedStatement stmt = getConnection().prepareStatement(query);
            if (ent.getKey().equals("coins") || ent.getKey().equals("score")) {
                stmt.setInt(1, Integer.parseInt(ent.getValue()));
            } else {
                stmt.setString(1, ent.getValue());
            }
            stmt.setString(2, uuid);
            stmt.execute();
        }

    }

    @Override
    public void delete(String id) throws SQLException {

    }
}
