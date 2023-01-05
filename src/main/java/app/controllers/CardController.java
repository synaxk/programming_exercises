package app.controllers;

import app.models.Card;
import app.models.User;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import server.Request;
import server.Response;

import java.util.ArrayList;

public class CardController extends Controller {

    @Setter(AccessLevel.PRIVATE)
    @Getter(AccessLevel.PRIVATE)
    private CardRepository cardRepository;

    public CardController(CardRepository cardRepository) {
        this.cardRepository = cardRepository;
    }

    public Response getStack(String token) throws JsonProcessingException {
        ArrayList<Card> cards = getCardRepository().getStack(token);
        String cardJson = getObjectMapper().writeValueAsString(cards);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + cardJson + ", \"error\": null }");
    }

    public Response getDeck(String token) throws JsonProcessingException {
        ArrayList<Card> cards = getCardRepository().getDeck(token);
        String cardJson = getObjectMapper().writeValueAsString(cards);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + cardJson + ", \"error\": null }");
    }

    public Response setDeck(String token, String body) throws JsonProcessingException {
        ArrayList<String> cards = getObjectMapper().readValue(body, ArrayList.class);
        if (getCardRepository().setDeck(token, cards)) {
            return new Response(HttpStatus.OK, ContentType.JSON,
                    "{ \"data\": \"Success\", \"error\": null }");
        }
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": null, \"error\": \"Error\" }");
    }
}
