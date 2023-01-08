package app.repositories;


import app.daos.CardDao;
import app.daos.TradingDao;
import app.daos.UserDao;
import app.models.Card;
import app.models.TradingDeal;
import app.models.User;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.sql.SQLException;
import java.util.ArrayList;

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class TradingRepository {

    TradingDao tradingDao;
    UserDao userDao;
    CardDao cardDao;

    public TradingRepository(TradingDao tradingDao, UserDao userDao, CardDao cardDao) {
        setTradingDao(tradingDao);
        setUserDao(userDao);
        setCardDao(cardDao);
    }

    public ArrayList<TradingDeal> getTradingDeals() {
        try {
            return new ArrayList<>(getTradingDao().read().values());
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public boolean createTradingDeal(String token, TradingDeal trade) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            trade.setUser_id(user.getUser_id());

            if (!getCardDao().userStackContainsCard(user.getUser_id().toString(), trade.getCard_id().toString()) ||
                    getCardDao().readDeckCardsByUserID(user.getUser_id().toString()).containsKey(trade.getCard_id())) {
                return false;
            }
            getTradingDao().create(trade);
            getCardDao().removeCardFromStack(trade.getCard_id().toString(), user.getUser_id().toString());
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean acceptTradingDeal(String token, String tradingDeal_id, String card_id) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            Card card = getCardDao().readByID(card_id);
            TradingDeal trade = getTradingDao().readByID(tradingDeal_id);

            if (getCardDao().userStackContainsCard(user.getUser_id().toString(), card_id)
                    && !user.getUser_id().equals(trade.getUser_id()) && card.getDamage() >= trade.getMinDamage()
                    && trade.getCardType().getCategory().equals(card.getCardType().getCategory())) {
                //remove new card from accepting user
                getCardDao().removeCardFromStack(card.getCard_id().toString(), user.getUser_id().toString());
                //give new card to trade user
                getCardDao().addCardToStack(card.getCard_id().toString(), trade.getUser_id().toString());
                //give trade card to accepting user
                getCardDao().addCardToStack(trade.getCard_id().toString(), user.getUser_id().toString());
                //remove deal
                getTradingDao().delete(tradingDeal_id);
                return true;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean deleteTradingDeal(String token, String tradingDeal_id) {
        try {
            User user = getUserDao().readByName(token.replace("-mtcgToken", ""));
            TradingDeal trade = getTradingDao().readByID(tradingDeal_id);
            if (user.getUser_id().equals(trade.getUser_id())) {
                //give card back to user
                getCardDao().addCardToStack(trade.getCard_id().toString(), trade.getUser_id().toString());
                //
                getTradingDao().delete(tradingDeal_id);
                return true;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }

}
