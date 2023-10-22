package app.controllers;

import app.dtos.UserDTO;
import app.models.Battle;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import server.Response;

import java.util.UUID;
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

            Battle battle = new Battle(user, cardRepository, userRepository);
            Battle receivedBattle = null;

            while (receivedBattle == null && !battleQueue.offer(battle)) {
                receivedBattle = battleQueue.poll();
            }

            if (receivedBattle == null) {
                // battle runs in the other thread
                battle.getBattleLatch().await();
                UUID winner = battle.getWinner_id();
                if (winner == null) {
                    return new Response(HttpStatus.OK, ContentType.JSON, "{\"Data\":\"Battle successfully completed."
                            + "Draw angainst " + battle.getUser2().getUsername() + " in " + battle.getRounds() +" rounds.\", " +
                            "\"error\":\"null\"}");
                }
                return new Response(HttpStatus.OK, ContentType.JSON, "{\"Data\":\"Battle successfully completed."
                        + "You "+ (winner.equals(user.getUser_id()) ? "won" : "lost") +" against " +
                        battle.getUser2().getUsername() + " in " + battle.getRounds() +" rounds.\", " +
                        "\"error\":\"null\"}");

            } else {
                battle = receivedBattle;
                battle.setUser2(user);
                battle.start();
                UUID winner = battle.getWinner_id();
                if (winner == null) {
                    return new Response(HttpStatus.OK, ContentType.JSON, "{\"Data\":\"Battle successfully completed."
                            + "Draw angainst " + battle.getUser1().getUsername() + " in " + battle.getRounds() +" rounds.\", " +
                            "\"error\":\"null\"}");
                }
                return new Response(HttpStatus.OK, ContentType.JSON, "{\"Data\":\"Battle successfully completed."
                        + "You "+ (winner.equals(user.getUser_id()) ? "won" : "lost") +" against " +
                        battle.getUser1().getUsername() + " in " + battle.getRounds() +" rounds.\", " +
                        "\"error\":\"null\"}");
            }
        } catch (InterruptedException e) {
            return new Response(
                    HttpStatus.INTERNAL_SERVER_ERROR,
                    ContentType.JSON,
                    "{ \"error\": \"Internal Server Error\", \"data\": null }"
            );
        }
    }
}

