package app.controllers;

import app.dtos.UserDTO;
import app.models.Battle;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import server.Response;

import java.util.concurrent.ArrayBlockingQueue;
@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class BattleController extends Controller{

    private UserRepository userRepository;
    private CardRepository cardRepository;
    public static ArrayBlockingQueue<Battle> battleQueue = new ArrayBlockingQueue<>(1);

    public BattleController(UserRepository userRepository, CardRepository cardRepository) {
        this.userRepository = userRepository;
        this.cardRepository = cardRepository;
    }

    public Response battle(String token) {
        UserDTO user = getUserRepository().getUserDTOByToken(token);

        Battle battle = new Battle(user, userRepository);
        Battle receivedBattle = null;

        while( receivedBattle == null && !battleQueue.offer(battle)) {
            receivedBattle = battleQueue.poll();
        }

        if (receivedBattle == null) {
            // battle runs in the other thread
        } else {
            battle = receivedBattle;
            battle.setUser2(user);

        }
        return null;
    }
}

