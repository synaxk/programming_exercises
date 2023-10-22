package app.repositories;

import app.daos.CardDao;
import app.daos.PackDao;
import app.daos.UserDao;
import app.dtos.PackDTO;
import app.models.Card;
import app.models.Pack;
import app.models.User;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class PackRepository {
    PackDao packDao;
    CardDao cardDao;

    public PackRepository(PackDao packDao, CardDao cardDao) {
        setPackDao(packDao);
        setCardDao(cardDao);
    }

    public HashMap<UUID, PackDTO> getAll() {
        return null;
    }

    public PackDTO getPackageByUID(String uuid) {
        return null;
    }

    public void createPack(ArrayList<Card> cards) {
        try {
            Pack pack = new Pack();
            getPackDao().create(pack);
            for (Card card : cards) {
                getCardDao().create(card);
                getCardDao().addCardToPack(card.getCard_id().toString(),
                        pack.getPack_id().toString());
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public int acquirePack(User user) {
        try {
            PackDTO pack = new PackDTO();
            if ((pack = getPackDao().readFirst()) == null) {
                return -1;
            }
            //get pack
            pack.setCards(getCardDao().readCardsByPackID(pack.getPack_id().toString()));
            //delete cards from pack & add to stack
            for (UUID card_id : pack.getCards().keySet()) {
                getCardDao().deletePackCards(pack.getPack_id().toString(), card_id.toString());
                getCardDao().addCardToStack(card_id.toString(), user.getUser_id().toString());
            }
            //delete pack from db
            getPackDao().delete(pack.getPack_id().toString());
            return pack.getPrice();
        } catch (SQLException e) {
            e.printStackTrace();
            return -1;
        }
    }
}
