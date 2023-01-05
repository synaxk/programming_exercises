package app.daos;

import app.models.Card;
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


public class CardDao implements Dao<Card> {

    @Getter(AccessLevel.PRIVATE)
    @Setter(AccessLevel.PRIVATE)
    private Connection connection;

    public CardDao(Connection connection) {
        this.connection = connection;
    }

    @Override
    public boolean create(Card card) throws SQLException {
        String query = "INSERT INTO cards (card_id, name, damage) VALUES (CAST(? AS UUID),?,?)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, card.getCard_id().toString());
        stmt.setString(2, card.getName());
        stmt.setInt(3, card.getDamage());
        return stmt.execute();
    }

    public boolean addCardToPack(String card_id, String pack_id) throws SQLException {
        String query = "INSERT INTO packcards (card_id, pack_id) VALUES " +
                "(CAST(? AS UUID), CAST(? AS UUID))";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, card_id);
        stmt.setString(2, pack_id);
        return stmt.execute();
    }

    public boolean addCardToStack(String card_id, String user_id) throws SQLException {
        String query = "INSERT INTO stackcards (card_id, user_id) VALUES" + "" +
                "(CAST(? AS UUID), CAST(? AS UUID))";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, card_id);
        stmt.setString(2, user_id);
        return stmt.execute();
    }

    public boolean addCardToDeck(String card_id, String user_id) throws SQLException {
        String query = "INSERT INTO deckcards (card_id, user_id) VALUES" + "" +
                "(CAST(? AS UUID), CAST(? AS UUID))";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, card_id);
        stmt.setString(2, user_id);
        return stmt.execute();
    }

    @Override
    public HashMap<UUID, Card> read() throws SQLException {
        HashMap<UUID, Card> cards = new HashMap<>();
        String query = "SELECT * FROM cards";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        ResultSet result = stmt.executeQuery();
        while (result.next()) {
            Card card = new Card(UUID.fromString(result.getString(1)),
                    result.getString(2),
                    result.getInt(3));
            cards.put(card.getCard_id(), card);
        }
        return cards;
    }

    public HashMap<UUID, Card> readCardsByPackID(String package_id) throws SQLException {
        String query = "SELECT * FROM cards WHERE card_id IN (" +
                "SELECT packcards.card_id FROM packcards WHERE packcards.pack_id = CAST(? AS UUID))";
        return getCardHashMap(package_id, query);
    }

    public HashMap<UUID, Card> readCardsByUserID(String user_id) throws SQLException {
        String query = "SELECT * FROM cards WHERE card_id IN (" +
                "SELECT stackcards.card_id FROM stackcards WHERE stackcards.user_id = CAST(? AS UUID))";
        return getCardHashMap(user_id, query);
    }

    public HashMap<UUID, Card> readCardsByDeckID(String deck_id) throws SQLException {
        String query = "SELECT * FROM cards WHERE card_id IN (" +
                "SELECT deckcards.card_id FROM deckcards WHERE deckcards.user_id = CAST(? AS UUID))";
        return getCardHashMap(deck_id, query);
    }

    private HashMap<UUID, Card> getCardHashMap(String uuid, String query) throws SQLException {
        HashMap<UUID, Card> cards = new HashMap<>();
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, uuid);
        ResultSet result = stmt.executeQuery();
        while (result.next()) {
            Card card = new Card(UUID.fromString(result.getString(1)),
                    result.getString(2),
                    result.getInt(3));
            cards.put(card.getCard_id(), card);
        }
        return cards;
    }


    @Override
    public void update(String uuid, Map<String, String> m) throws SQLException {

    }

    @Override
    public void delete(String id) throws SQLException {
    }


    public void deletePackCards(String pack_id, String card_id) throws SQLException {
        String query = "DELETE FROM packcards WHERE pack_id = CAST(? AS UUID) AND card_id = CAST(? AS UUID)";
        PreparedStatement stmt = getConnection().prepareStatement(query);
        stmt.setString(1, pack_id);
        stmt.setString(2, card_id);
        stmt.executeUpdate();
    }
}
