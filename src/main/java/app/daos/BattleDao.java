package app.daos;

import app.models.Battle;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public class BattleDao implements Dao<Battle> {

    @Getter(AccessLevel.PRIVATE)
    @Setter(AccessLevel.PRIVATE)
    private Connection connection;

    public BattleDao(Connection connection) {
        this.connection = connection;
    }

    @Override
    public boolean create(Battle battle) throws SQLException {
        return false;
    }

    @Override
    public HashMap<UUID, Battle> read() throws SQLException {
        return null;
    }

    @Override
    public void update(String uuid, Map<String, String> m) throws SQLException {

    }

    @Override
    public void delete(String id) throws SQLException {

    }
}
