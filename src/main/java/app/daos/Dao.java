package app.daos;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public interface Dao<T> {
    boolean create(T t) throws SQLException;
    HashMap<UUID, T> read() throws SQLException;
    void update(String uuid, Map<String,String> m) throws SQLException;
    void delete(String id) throws SQLException;

}
