package app.controllers;

import app.models.Card;
import app.models.CardType;
import app.models.User;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.springframework.security.crypto.bcrypt.BCrypt;
import server.Response;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class CardControllerTest {

    CardRepository cardRepository;
    CardController cardController;

    ArrayList<Card> cards;

    @BeforeEach
    void setUp(){
        cardRepository = mock(CardRepository.class);
        cardController = new CardController(cardRepository);
        cards = new ArrayList<>();
        cards.add(new Card(UUID.randomUUID(), "Test", 10, CardType.DRAGON));
    }

    @Test
    @DisplayName("Get Stack")
    void getStack() {
        //arrange
        String token = "test-mtcgToken";
        Mockito.when(cardRepository.getStackList(token)).thenReturn(cards);
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": [{\"card_id\":\""+ cards.get(0).getCard_id().toString()
                        +"\",\"name\":\"Test\",\"damage\":10,\"cardType\":\"DRAGON\"}],"
                        + " \"error\": null }");
        //act
        Response response = null;
        try{
            response = cardController.getStack(token);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContent(), response.getContent());
        assertEquals(exp_response.getContentType(), response.getContentType());
    }

    @Test
    @DisplayName("Get Deck Json")
    void getDeckJson() {
        //arrange
        String token = "test-mtcgToken";
        Mockito.when(cardRepository.getDeckList(token)).thenReturn(cards);
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": [{\"card_id\":\""+ cards.get(0).getCard_id().toString()
                        +"\",\"name\":\"Test\",\"damage\":10,\"cardType\":\"DRAGON\"}],"
                        + " \"error\": null }");
        //act
        Response response = null;
        try{
            response = cardController.getDeck(token, "format=json");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContent(), response.getContent());
        assertEquals(exp_response.getContentType(), response.getContentType());
    }

    @Test
    @DisplayName("Get Deck Plaintext")
    void getDeckPlain() {
        //arrange
        String token = "test-mtcgToken";
        Mockito.when(cardRepository.getDeckList(token)).thenReturn(cards);
        Response exp_response = new Response(HttpStatus.OK, ContentType.TEXT,
                "{ \"data\": [{\"card_id\":\""+ cards.get(0).getCard_id().toString()
                        +"\",\"name\":\"Test\",\"damage\":10,\"cardType\":\"DRAGON\"}],"
                        + " \"error\": null }");
        //act
        Response response = null;
        try{
            response = cardController.getDeck(token, "format=plain");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContent(), response.getContent());
        assertEquals(exp_response.getContentType(), response.getContentType());
    }

    @Test
    @DisplayName("Set Deck Success")
    void setDeckSuccess() {
        //arrange
        String token = "test-mtcgToken";
        String body = "[\"845f0dc7-37d0-426e-994e-43fc3ac83c08\"," +
                " \"99f8f8dc-e25e-4a95-aa2c-782823f36e2a\", "
                +"\"e85e3976-7c86-4d06-9a80-641c2019a79f\", " +
                "\"171f6076-4eb5-4a7d-b3f2-2d650cc3d237\"]";

        ArrayList<String> deck = new ArrayList<>();
        deck.add("845f0dc7-37d0-426e-994e-43fc3ac83c08");
        deck.add("99f8f8dc-e25e-4a95-aa2c-782823f36e2a");
        deck.add("e85e3976-7c86-4d06-9a80-641c2019a79f");
        deck.add("171f6076-4eb5-4a7d-b3f2-2d650cc3d237");
        Mockito.when(cardRepository.setDeck(token, deck)).thenReturn(true);

        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": \"The deck has been successfully configured.\", \"error\": null }");

        Response response = null;

        //act
        try {
            response = cardController.setDeck(token, body);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Set Deck Size Fail")
    void setDeckSizeFail() {
        //arrange
        String token = "test-mtcgToken";
        String body = "[\"845f0dc7-37d0-426e-994e-43fc3ac83c08\"," +
                " \"99f8f8dc-e25e-4a95-aa2c-782823f36e2a\", "
                +"\"e85e3976-7c86-4d06-9a80-641c2019a79f\"]";

        ArrayList<String> deck = new ArrayList<>();
        deck.add("845f0dc7-37d0-426e-994e-43fc3ac83c08");
        deck.add("99f8f8dc-e25e-4a95-aa2c-782823f36e2a");
        deck.add("e85e3976-7c86-4d06-9a80-641c2019a79f");
        Mockito.when(cardRepository.setDeck(token, deck)).thenReturn(true);

        Response exp_response = new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                "{ \"data\": null, \"error\": \"The provided deck did not " +
                        "include the required amount of cards.\" }");

        Response response = null;

        //act
        try {
            response = cardController.setDeck(token, body);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Set Deck Size Fail")
    void setDeckAuthFail() {
        //arrange
        String token = "test-mtcgToken";
        String body = "[\"845f0dc7-37d0-426e-994e-43fc3ac83c08\"," +
                " \"99f8f8dc-e25e-4a95-aa2c-782823f36e2a\", "
                +"\"e85e3976-7c86-4d06-9a80-641c2019a79f\", " +
                "\"171f6076-4eb5-4a7d-b3f2-2d650cc3d237\"]";

        ArrayList<String> deck = new ArrayList<>();
        deck.add("845f0dc7-37d0-426e-994e-43fc3ac83c08");
        deck.add("99f8f8dc-e25e-4a95-aa2c-782823f36e2a");
        deck.add("e85e3976-7c86-4d06-9a80-641c2019a79f");
        deck.add("171f6076-4eb5-4a7d-b3f2-2d650cc3d237");
        Mockito.when(cardRepository.setDeck(token, deck)).thenReturn(false);

        Response exp_response = new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                "{ \"data\": null, \"error\": \"At least one of the provided cards does not belong to " +
                        "the user or is not available\" }");

        Response response = null;

        //act
        try {
            response = cardController.setDeck(token, body);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }


}
