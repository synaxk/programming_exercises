package app.daos;

import app.dtos.PackDTO;
import app.models.Pack;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public class PackDao implements Dao<Pack> {
    @Getter(AccessLevel.PRIVATE)
    @Setter(AccessLevel.PRIVATE)
    private Connection connection;

    public PackDao(Connection connection) {
        this.connection = connection;
    }

    @Override
    public boolean create(Pack pack) throws SQLException {
        String query = "INSERT INTO packs (pack_id, name, price) VALUES (CAST(? AS UUID), ?, ?)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1,pack.getPack_id().toString());
        stmt.setString(2,pack.getName());
        stmt.setInt(3, pack.getPrice());
        return stmt.execute();
    }

    @Override
    public HashMap<UUID, Pack> read() throws SQLException {

        return null;
    }

    public PackDTO readFirst() throws SQLException {
        String query = "SELECT * FROM packs";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        ResultSet result = stmt.executeQuery();
        if (result.next()) {
            return new PackDTO(UUID.fromString(result.getString(1)),
                    result.getString(2), 5, null);
        }
        return null;
    }

    @Override
    public void update(String uuid, Map<String, String> m) throws SQLException {

    }

    @Override
    public void delete(String id) throws SQLException {
        String query = "DELETE FROM packs WHERE pack_id = CAST(? AS UUID)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, id);
        stmt.executeUpdate();
    }


}
