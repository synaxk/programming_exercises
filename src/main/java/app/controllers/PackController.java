package app.controllers;

import app.models.Card;
import app.models.User;
import app.repositories.PackRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import server.Response;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

@Setter(AccessLevel.PRIVATE)
@Getter(AccessLevel.PRIVATE)
public class PackController extends Controller {

    private PackRepository packRepository;
    private UserRepository userRepository;

    public PackController(PackRepository packRepository, UserRepository userRepository) {
        this.packRepository = packRepository;
        this.userRepository = userRepository;
    }
    public Response createPackage(String body) {
        try {
            ArrayList<Card> cards = getObjectMapper().readValue(body, new TypeReference<ArrayList<Card>>() {});
            getPackRepository().createPack(cards);
            return new Response(
                    HttpStatus.OK,
                    ContentType.JSON,
                    "{ \"data\": \"Package and cards successfully created.\", \"error\": null }"
            );
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return new Response(
                    HttpStatus.BAD_REQUEST,
                    ContentType.JSON,
                    "{ \"error\": \"At least one card in the packages already exists.\", \"data\": null }"
            );
        }
    }

    public Response acquirePackage(String token) {
        String username = token.replace("-mtcgToken","");
        User user = getUserRepository().getUserByName(username);
        int price = 0;
        if (user.getCoins() >= 5) {
            if ((price = getPackRepository().acquirePack(user)) > 0){
                user.setCoins(user.getCoins()-price);
                getUserRepository().updateUser(user.getUser_id().toString(),
                        Map.of("coins", String.valueOf(user.getCoins())));
                return new Response(
                        HttpStatus.FORBIDDEN,
                        ContentType.JSON,
                        "{\"data\":\"A package has been successfully bought.\",\"error\":\"null\"}"
                );
            }
            return new Response(
                    HttpStatus.BAD_REQUEST,
                    ContentType.JSON,
                    "{ \"error\": \"No card package available for buying.\", \"data\": null }"
            );
        } else {
            return new Response(
                    HttpStatus.BAD_REQUEST,
                    ContentType.JSON,
                    "{ \"error\": \"Not enough money for buying a card package.\", \"data\": null }"
            );
        }
    }
}
