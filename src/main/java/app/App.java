package app;

import app.controllers.*;
import app.daos.BattleDao;
import app.daos.CardDao;
import app.daos.PackDao;
import app.daos.UserDao;
import app.repositories.BattleRepository;
import app.repositories.CardRepository;
import app.repositories.PackRepository;
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
        BattleDao battleDao = new BattleDao(connection);


        setUserController(new UserController(new UserRepository(userDao)));
        setSessionController(new SessionController(new UserRepository(userDao)));
        setPackController(new PackController(new PackRepository(packDao, cardDao), new UserRepository(userDao)));
        setCardController(new CardController(new CardRepository(cardDao, userDao)));
        setBattleController(new BattleController(new BattleRepository(battleDao, userDao, cardDao)));
    }

    //routes
    // users || POST
    // users/USERNAME || GET, PUT
    // sessions || POST
    // packages || POST
    // transactions
    // transactions/packages || POST
    // cards || GET
    // decks, var format=plain || GET, PUT
    // stats || GET
    // battles || POST
    // score || GET
    // tradings || GET, POST
    // tradings/TRADEID || DELETE || POST


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
                            return getCardController().getDeck(request.getAuthToken());
                        } else if (Pattern.matches("/scores",request.getPathname())) {
                            return getUserController().getScoreBoard();
                        }
                        break;
                    case PUT:
                        if (Pattern.matches(".*/users/.*",request.getPathname())) {
                            if (getSessionController().authorize(request.getAuthToken(), request.getPathVar())) {
                                return getUserController().updateUser(request.getPathVar(), request.getBody());
                            }
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
                        } else if (Pattern.matches("/battle", request.getPathname())) {
                        /*        Thread.currentThread().getThreadGroup().list();

                                try {
                                    Thread.currentThread().wait();
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }*/

                        }
                        //acquire packages
                        if (Pattern.matches("/transactions/packages", request.getPathname())) {
                            return packController.acquirePackage(request.getAuthToken());
                        }
                        break;
                    case DELETE:
                        break;
                    default:
                        break;
                }
            }
        }


        return new Response(HttpStatus.OK, ContentType.JSON, "asdfasdf");
    }

}


