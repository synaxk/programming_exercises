package app.controllers;

import app.dtos.UserDTO;
import app.models.Battle;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
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
    public static final ArrayBlockingQueue<Battle> battleQueue = new ArrayBlockingQueue<>(1);

    public BattleController(UserRepository userRepository, CardRepository cardRepository) {
        this.userRepository = userRepository;
        this.cardRepository = cardRepository;
    }

    public Response battle(String token) {
        try {
            UserDTO user = getUserRepository().getUserDTOByToken(token);

            Battle battle = new Battle(user, userRepository);
            Battle receivedBattle = null;

            while (receivedBattle == null && !battleQueue.offer(battle)) {
                receivedBattle = battleQueue.poll();
            }

            if (receivedBattle == null) {
                // battle runs in the other thread
                battle.getBattleLatch().await();
                String battleLogJson = getObjectMapper().writeValueAsString(battle.getUser1().getBattleLog());
                return new Response(HttpStatus.OK, ContentType.JSON, "{\"Data\":\""+ battleLogJson +"\", " +
                        "\"error\":\"null\"}");
            } else {
                battle = receivedBattle;
                battle.setUser2(user);
                battle.start();
                String battleLogJson = getObjectMapper().writeValueAsString(battle.getUser2().getBattleLog());
                return new Response(HttpStatus.OK, ContentType.JSON, "{\"Data\":\""+ battleLogJson +"\", " +
                        "\"error\":\"null\"}");
            }
        } catch (InterruptedException | JsonProcessingException e) {
            e.printStackTrace();
        }
        return null;
    }
}

