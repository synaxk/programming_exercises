package app.controllers;

import app.models.Card;
import app.models.CardType;
import app.models.User;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
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
        ArrayList<Card> cards = getCardRepository().getStackList(token);
        String cardJson = getObjectMapper().writeValueAsString(cards);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + cardJson + ", \"error\": null }");
    }

    public Response getDeck(String token, String format) throws JsonProcessingException {
        ArrayList<Card> cards = getCardRepository().getDeckList(token);
        String cardJson = getObjectMapper().writeValueAsString(cards);
        ContentType contentType = ContentType.JSON;
        if (format.contains("plain")) {
            contentType = ContentType.TEXT;
        }
        return new Response(HttpStatus.OK, contentType,
                "{ \"data\": " + cardJson + ", \"error\": null }");
    }

    public Response setDeck(String token, String body) throws JsonProcessingException {
        ArrayList<String> cards = getObjectMapper().readValue(body, ArrayList.class);
        if (cards.size() != 4) {
            return new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                    "{ \"data\": null, \"error\": \"The provided deck did not " +
                            "include the required amount of cards.\" }");
        }
        if (getCardRepository().setDeck(token, cards)) {
            return new Response(HttpStatus.OK, ContentType.JSON,
                    "{ \"data\": \"The deck has been successfully configured.\", \"error\": null }");
        }
        return new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                "{ \"data\": null, \"error\": \"At least one of the provided cards does not belong to " +
                        "the user or is not available\" }");
    }
}
