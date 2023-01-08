package app.daos;

import app.controllers.Controller;
import app.models.CardType;
import app.models.TradingDeal;
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

public class TradingDao implements Dao<TradingDeal>{
    @Getter(AccessLevel.PRIVATE)
    @Setter(AccessLevel.PRIVATE)
    private Connection connection;

    public TradingDao(Connection connection) {
        this.connection = connection;
    }

    @Override
    public boolean create(TradingDeal tradingDeal) throws SQLException {
        String query = "INSERT INTO tradingdeals (tradingdeal_id, user_id, card_id, cardtype, mindamage) " +
                "Values (CAST(? AS UUID), CAST(? AS UUID), CAST(? AS UUID), ?, ?)";
        PreparedStatement stmt = connection.prepareStatement(query);
        stmt.setString(1, tradingDeal.getTradingDeal_id().toString());
        stmt.setString(2, tradingDeal.getUser_id().toString());
        stmt.setString(3, tradingDeal.getCard_id().toString());
        stmt.setInt(4, tradingDeal.getCardType().getCode());
        stmt.setInt(5, tradingDeal.getMinDamage());
        return stmt.execute();
    }

    @Override
    public HashMap<UUID, TradingDeal> read() throws SQLException {
        HashMap<UUID, TradingDeal> trades = new HashMap<>();
        String query = "SELECT * FROM tradingdeals";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        ResultSet result = stmt.executeQuery();
        while (result.next()) {
            TradingDeal trade = new TradingDeal(UUID.fromString(result.getString(1)),
                    UUID.fromString(result.getString(2)),
                    UUID.fromString(result.getString(3)),
                    CardType.parseInt(result.getInt(4)),
                    result.getInt(5));
            trades.put(trade.getTradingDeal_id(), trade);
        }
        return trades;
    }

    public TradingDeal readByID(String tradingDeal_id) throws SQLException {
        String query = "SELECT * FROM tradingdeals WHERE tradingdeal_id = CAST(? AS UUID)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, tradingDeal_id);
        ResultSet result = stmt.executeQuery();
        if (result.next()) {
            return new TradingDeal(UUID.fromString(result.getString(1)),
                    UUID.fromString(result.getString(2)),
                    UUID.fromString(result.getString(3)),
                    CardType.parseInt(result.getInt(4)), result.getInt(5));
        }
        return null;
    }

    @Override
    public void update(String uuid, Map<String, String> m) throws SQLException {

    }

    @Override
    public void delete(String uuid) throws SQLException {
        String query = "DELETE FROM tradingdeals WHERE tradingdeal_id = CAST(? AS UUID)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, uuid);
        stmt.execute();
    }

}
