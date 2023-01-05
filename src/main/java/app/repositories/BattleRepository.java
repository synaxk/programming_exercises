package app.repositories;

import app.App;
import app.daos.BattleDao;
import app.daos.CardDao;
import app.daos.UserDao;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class BattleRepository {
    BattleDao battleDao;
    UserDao userDao;
    CardDao cardDao;

    public BattleRepository(BattleDao battleDao, UserDao userDao, CardDao cardDao){
        setBattleDao(battleDao);
        setUserDao(userDao);
        setCardDao(cardDao);
    }
}
