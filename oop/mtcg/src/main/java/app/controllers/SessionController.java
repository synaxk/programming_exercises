package app.controllers;

import app.models.User;
import app.repositories.UserRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import org.springframework.security.crypto.bcrypt.BCrypt;
import server.Response;

public class SessionController extends Controller {

    @Setter(AccessLevel.PRIVATE)
    @Getter(AccessLevel.PRIVATE)
    private UserRepository userRepository;

    public SessionController(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    public Response login(String body) throws JsonProcessingException {

        User req_user = getObjectMapper().readValue(body, User.class);
        User user = getUserRepository().getUserByName(req_user.getUsername());

        if (req_user.getUsername().equals(user.getUsername()) &&
                BCrypt.checkpw(req_user.getPassword(), user.getPassword())) {
            return new Response(HttpStatus.OK, ContentType.JSON,
                    "{\"data\": \"User login successful, Token: " + this.generateToken(user) +
                            "\", \"error\": null}");
        }
        return new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                "{\"data\": null, \"error\": \"Invalid username/password provided\"}");
    }

    public boolean verifyAuthToken(String authToken) {
        String username = authToken.replace("-mtcgToken", "");
        User user = getUserRepository().getUserByName(username);
        return user != null;
    }

    public boolean authorize(String authToken, String user_id) {
        String username = authToken.replace("-mtcgToken","");
        User user = getUserRepository().getUserByUID(user_id);
        if (user != null && user.getUsername().equals(username)) {
            return true;
        }
        return false;
    }

    private String generateToken (User user) {
        return user.getUsername() + "-mtcgToken";
    }
}
