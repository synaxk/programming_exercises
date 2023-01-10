package app.controllers;

import app.models.TradingDeal;
import app.repositories.TradingRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import server.Response;

import java.util.ArrayList;

public class TradingController extends Controller{

    @Setter(AccessLevel.PRIVATE)
    @Getter(AccessLevel.PRIVATE)
    private TradingRepository tradingRepository;

    public TradingController(TradingRepository tradingRepository) {
        setTradingRepository(tradingRepository);
    }


    //get trades

    public Response getTradingDeals() {
        try {
            ArrayList<TradingDeal> trades = getTradingRepository().getTradingDeals();
            String tradesJson = getObjectMapper().writeValueAsString(trades);
            if (trades.isEmpty()) {
                return new Response(HttpStatus.OK, ContentType.JSON,
                        "{ \"data\": " + tradesJson + ", \"error\": null }");
            }
            return new Response(HttpStatus.OK, ContentType.JSON,
                    "{ \"data\": " + tradesJson + ", \"error\": null }");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return new Response(
                    HttpStatus.INTERNAL_SERVER_ERROR,
                    ContentType.JSON,
                    "{ \"error\": \"Internal Server Error\", \"data\": null }");
        }
    }

    //create tradingdeal -- move card from userstack to trades
    public Response createDeal(String token, String body){
        try {
            TradingDeal trade = getObjectMapper().readValue(body, TradingDeal.class);
            ArrayList<TradingDeal> trades = getTradingRepository().getTradingDeals();

            for (TradingDeal tr : trades) {
                if (tr.getTradingDeal_id().equals(trade.getTradingDeal_id())) {
                    return new Response(HttpStatus.ALREADY_EXISTS, ContentType.JSON,
                            "{\"data\":\"\",\"error\":\"A deal with this deal ID already exists\"}");
                }
            }

            if (!getTradingRepository().createTradingDeal(token, trade)) {
                return new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                        "{ \"data\": \"The deal contains a card that is not owned by " +
                                "the user or locked in the deck\", \"error\": null }");
            }
            return new Response(HttpStatus.CREATED, ContentType.JSON,
                    "{ \"data\": \"Trading deal successfully created.\", \"error\": null }");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        return null;
    }

    //Post accept trading deal
    public Response acceptDeal(String token, String trade_id, String body) {
        try {
            String card_id = getObjectMapper().readValue(body, String.class);
            if (getTradingRepository().acceptTradingDeal(token, trade_id, card_id)) {
                return new Response(HttpStatus.OK, ContentType.JSON,
                        "{ \"data\": \"Trading deal successfully executed.\", \"error\": null }");
            }
            return new Response(HttpStatus.OK, ContentType.JSON, "{\"data\":\"null\",\"error\":\"The offered " +
                    "card is not owned by the user, or the requirements are not met (Type, MinimumDamage), " +
                    "or the offered card is locked in the deck, or the user tries to trade with self\"}");

        } catch (JsonProcessingException e){
            e.printStackTrace();
            return new Response(
                    HttpStatus.INTERNAL_SERVER_ERROR,
                    ContentType.JSON,
                    "{ \"error\": \"Internal Server Error\", \"data\": null }");
        }
    }

    //del delete trading deal
    public Response deleteDeal(String token, String trade_id) {
        if (getTradingRepository().deleteTradingDeal(token, trade_id)) {
            return new Response(HttpStatus.OK, ContentType.JSON,
                    "{ \"data\": \"Trading deal successfully deleted.\", \"error\": null }");
        }
        return new Response(
                HttpStatus.INTERNAL_SERVER_ERROR,
                ContentType.JSON,
                "{ \"error\": \"Internal Server Error\", \"data\": null }");

    }
}
