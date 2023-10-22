package app.controllers;

import app.models.CardType;
import app.models.TradingDeal;
import app.repositories.TradingRepository;
import http.ContentType;
import http.HttpStatus;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import server.Response;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;

class TradingControllerTest {

    TradingRepository tradingRepository;
    TradingController tradingController;


    @BeforeEach
    void setUp(){
        tradingRepository = mock(TradingRepository.class);
        tradingController = new TradingController(tradingRepository);
    }
    @Test
    @DisplayName("Get Trading Deals ")
    void getTradingDeals() {
        //arrange
        TradingDeal trade = new TradingDeal(UUID.randomUUID(),UUID.randomUUID(),UUID.randomUUID(), CardType.MONSTER, 10);
        Mockito.when(tradingRepository.getTradingDeals()).thenReturn(new ArrayList<>(List.of(trade)));
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": [{\"tradingDeal_id\":\""+trade.getTradingDeal_id().toString()+"\"," +
                        "\"user_id\":\""+trade.getUser_id().toString()+"\"," +
                        "\"card_id\":\""+trade.getCard_id().toString()+"\",\"cardType\":\"MONSTER\"," +
                        "\"minDamage\":10}], \"error\": null }");
        //act
        Response response = tradingController.getTradingDeals();

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());

    }

    @Test
    @DisplayName("Get Trading Deals Empty")
    void getTradingDealsEmpty() {
        //arrange

        Mockito.when(tradingRepository.getTradingDeals()).thenReturn(new ArrayList<>());
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": [], \"error\": null }");
        //act
        Response response = tradingController.getTradingDeals();

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Create Deal Success")
    void createDealSuccess() {
        //arrange
        String token = "test-mtcgToken";
        String body = "{\"TradingDeal_id\": \"6cd85277-4590-49d4-b0cf-ba0a921faad0\", \"Card_id\": " +
                "\"1cb6ab86-bdb2-47e5-b6e4-68c5ab389334\", \"CardType\": \"monster\", \"MinDamage\": 15}";

        Mockito.when(tradingRepository.createTradingDeal(any(), any())).thenReturn(true);
        Response exp_response = new Response(HttpStatus.CREATED, ContentType.JSON,
                "{ \"data\": \"Trading deal successfully created.\", \"error\": null }");

        //act
        Response response = tradingController.createDeal(token, body);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Create Deal Exists")
    void createDealExists() {
        //arrange
        String token = "test-mtcgToken";
        String body = "{\"TradingDeal_id\": \"6cd85277-4590-49d4-b0cf-ba0a921faad0\", \"Card_id\": " +
                "\"1cb6ab86-bdb2-47e5-b6e4-68c5ab389334\", \"CardType\": \"monster\", \"MinDamage\": 15}";

        TradingDeal trade = new TradingDeal(UUID.fromString("6cd85277-4590-49d4-b0cf-ba0a921faad0"),
                UUID.fromString("1cb6ab86-bdb2-47e5-b6e4-68c5ab389334"), "monster", 15);

        ArrayList<TradingDeal> trades = new ArrayList<>();
        trades.add(trade);

        Mockito.when(tradingRepository.getTradingDeals()).thenReturn(trades);
        Response exp_response = new Response(HttpStatus.ALREADY_EXISTS, ContentType.JSON,
                "{\"data\":\"\",\"error\":\"A deal with this deal ID already exists\"}");

        //act
        Response response = tradingController.createDeal(token, body);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Create Deal Forbidden")
    void createDealForbidden() {
        //arrange
        String token = "test-mtcgToken";
        String body = "{\"TradingDeal_id\": \"6cd85277-4590-49d4-b0cf-ba0a921faad0\", \"Card_id\": " +
                "\"1cb6ab86-bdb2-47e5-b6e4-68c5ab389334\", \"CardType\": \"monster\", \"MinDamage\": 15}";

        TradingDeal trade = new TradingDeal(UUID.fromString("6cd85277-4590-49d4-b0cf-ba0a921faad0"),
                UUID.fromString("1cb6ab86-bdb2-47e5-b6e4-68c5ab389334"), "monster", 15);

        ArrayList<TradingDeal> trades = new ArrayList<>();
        trades.add(trade);

        Mockito.when(tradingRepository.createTradingDeal(any(), any())).thenReturn(false);
        Response exp_response = new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                "{ \"data\": \"The deal contains a card that is not owned by " +
                        "the user or locked in the deck\", \"error\": null }");

        //act
        Response response = tradingController.createDeal(token, body);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Accept Deal Success")
    void acceptDeal() {
        //arrange
        String body = "\"" + UUID.randomUUID() + "\"";
        String trade_id = UUID.randomUUID().toString();
        String token = "test-mtcgToken";
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": \"Trading deal successfully executed.\", \"error\": null }");
        Mockito.when(tradingRepository.acceptTradingDeal(any(), any(), any(String.class))).thenReturn(true);
        //act
        Response response = tradingController.acceptDeal(token, trade_id, body);
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Accept Deal Fail")
    void acceptFail() {
        //arrange
        String body = "\"" + UUID.randomUUID() + "\"";
        String trade_id = UUID.randomUUID().toString();
        String token = "test-mtcgToken";
        Response exp_response = new Response(HttpStatus.FORBIDDEN, ContentType.JSON, "{\"data\":\"null\",\"error\":\"The offered " +
                "card is not owned by the user, or the requirements are not met (Type, MinimumDamage), " +
                "or the offered card is locked in the deck, or the user tries to trade with self\"}");
        Mockito.when(tradingRepository.acceptTradingDeal(any(), any(), any(String.class))).thenReturn(false);
        //act
        Response response = tradingController.acceptDeal(token, trade_id, body);
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Delete Deal Success")
    void deleteDealSuccess() {
        //arrange
        String token = "test-mtcgToken";
        String trade_id = UUID.randomUUID().toString();

        Mockito.when(tradingRepository.deleteTradingDeal(token, trade_id)).thenReturn(true);
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": \"Trading deal successfully deleted.\", \"error\": null }");

        // act
        Response response = tradingController.deleteDeal(token, trade_id);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());

    }

    @Test
    @DisplayName("Delete Deal Fail")
    void deleteDealFail() {
        //arrange
        String token = "test-mtcgToken";
        String trade_id = UUID.randomUUID().toString();

        Mockito.when(tradingRepository.deleteTradingDeal(token, trade_id)).thenReturn(false);
        Response exp_response = new Response(HttpStatus.INTERNAL_SERVER_ERROR, ContentType.JSON,
                "{ \"error\": \"Internal Server Error\", \"data\": null }");

        // act
        Response response = tradingController.deleteDeal(token, trade_id);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());

    }
}