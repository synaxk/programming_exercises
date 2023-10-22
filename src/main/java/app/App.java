package app;

import app.controllers.*;
import app.daos.CardDao;
import app.daos.PackDao;
import app.daos.TradingDao;
import app.daos.UserDao;
import app.repositories.CardRepository;
import app.repositories.PackRepository;
import app.repositories.TradingRepository;
import app.repositories.UserRepository;
import app.services.DatabaseService;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
import http.Method;
import lombok.Getter;
import server.*;
import lombok.AccessLevel;
import lombok.Setter;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.regex.Pattern;

@Setter(AccessLevel.PRIVATE)
@Getter(AccessLevel.PRIVATE)
public class App implements ServerApp {

    private UserController userController;
    private SessionController sessionController;
    private PackController packController;
    private CardController cardController;
    private BattleController battleController;
    private TradingController tradingController;
    private Connection connection;


    public App() {
        try {
            setConnection(new DatabaseService().getConnection());
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        UserDao userDao = new UserDao(connection);
        PackDao packDao = new PackDao(connection);
        CardDao cardDao = new CardDao(connection);
        TradingDao tradingDao = new TradingDao(connection);


        setUserController(new UserController(new UserRepository(userDao)));
        setSessionController(new SessionController(new UserRepository(userDao)));
        setPackController(new PackController(new PackRepository(packDao, cardDao), new UserRepository(userDao)));
        setCardController(new CardController(new CardRepository(cardDao, userDao)));
        setBattleController(new BattleController(new UserRepository(userDao, cardDao), new CardRepository(cardDao, userDao)));
        setTradingController(new TradingController(new TradingRepository(tradingDao, userDao, cardDao)));
    }

    public Response handleRequest(Request request) throws JsonProcessingException {

        // only allow login and registration for requests without token
        if (request.getAuthToken().isEmpty()) {
            if (request.getMethod() == Method.POST) {
                if (Pattern.matches(".*/sessions", request.getPathname())) {
                    return getSessionController().login(request.getBody());
                } else if (Pattern.matches(".*/users", request.getPathname())) {
                    return getUserController().createUser(request.getBody());
                }
            }
            return new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                    "{\"data\": null, \"error\": \"Bad Request.\"}");

        } else { /*verify the authentication token for further processing*/
            if (!getSessionController().verifyAuthToken(request.getAuthToken())) {
                return new Response(HttpStatus.UNAUTHORIZED, ContentType.JSON,
                        "{\"data\": null, \"error\": \"Unauthorized: Invalid Token.\"}");
            } else {
                // authenticated requests
                switch (request.getMethod()) {
                    case GET:
                        if (request.getPathname().equals("/users")) {
                            return getUserController().getUsers();
                        } else if (Pattern.matches(".*/users/.*",request.getPathname())) {
                            if (getSessionController().authorize(request.getAuthToken(), request.getPathVar())) {
                                return getUserController().getUserByID(request.getPathVar());
                            }
                            return new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                                    "{\"data\": null, \"error\": \"Forbidden.\"}");
                        } else if (Pattern.matches("/cards", request.getPathname())) {
                            return getCardController().getStack(request.getAuthToken());
                        } else if (Pattern.matches("/decks", request.getPathname())) {
                            return getCardController().getDeck(request.getAuthToken(), request.getParams());
                        } else if (Pattern.matches("/scores",request.getPathname())) {
                            return getUserController().getScoreBoard();
                        } else if (Pattern.matches("/stats", request.getPathname())) {
                            return getUserController().getScore(request.getAuthToken());
                        } else if (Pattern.matches("/tradings", request.getPathname())) {
                            return getTradingController().getTradingDeals();
                        }
                        break;
                    case PUT:
                        if (Pattern.matches(".*/users/.*",request.getPathname())) {
                            if (getSessionController().authorize(request.getAuthToken(), request.getPathVar())) {
                                return getUserController().updateUser(request.getPathVar(), request.getBody());
                            }
                            return new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                                    "{\"data\": null, \"error\": \"Forbidden.\"}");
                        } else if (Pattern.matches("/decks", request.getPathname())) {
                            return getCardController().setDeck(request.getAuthToken(), request.getBody());
                        }
                        break;
                    case POST:
                        // create new packages
                        if (Pattern.matches("^((?!transaction).)*\\/packages$",request.getPathname())) {
                            if (request.getAuthToken().contains("admin-mtcgToken")) {
                                return getPackController().createPackage(request.getBody());
                            }
                            return new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                                    "{\"data\": null, \"error\": \"Forbidden.\"}");
                        } else if (Pattern.matches("/battles", request.getPathname())) {
                            return getBattleController().battle(request.getAuthToken());
                        } else if (Pattern.matches("/transactions/packages", request.getPathname())) {
                            //acquire packages
                            return getPackController().acquirePackage(request.getAuthToken());
                        } else if (Pattern.matches("/tradings/.*", request.getPathname())) {
                            // accept trade deal with card
                            return getTradingController().acceptDeal(request.getAuthToken(), request.getPathVar(), request.getBody());
                        } else if (Pattern.matches("/tradings", request.getPathname())) {
                            // create trading deal
                            return getTradingController().createDeal(request.getAuthToken(), request.getBody());
                        }
                        break;
                    case DELETE:
                        if (Pattern.matches("/tradings/.*", request.getPathname())) {
                            return getTradingController().deleteDeal(request.getAuthToken(), request.getPathVar());
                        }
                        break;
                    default:
                        break;
                }
            }
        }


        return new Response(HttpStatus.OK, ContentType.JSON, "asdfasdf");
    }

}


