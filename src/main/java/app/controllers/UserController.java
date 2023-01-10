package app.controllers;

import app.dtos.ScoreEntryDTO;
import app.models.User;
import app.repositories.UserRepository;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import org.springframework.security.crypto.bcrypt.BCrypt;
import server.Response;
import com.fasterxml.jackson.core.JsonProcessingException;


import java.util.*;

public class UserController extends Controller {
    @Setter(AccessLevel.PRIVATE)
    @Getter(AccessLevel.PRIVATE)
    private UserRepository userRepository;

    public UserController(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    public Response getUsers() throws JsonProcessingException {
        ArrayList<User> users = getUserRepository().getAll();
        String usersJSON = getObjectMapper().writeValueAsString(users);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + usersJSON + ", \"error\": null }");
    }

    public Response getUserByID(String uuid) throws JsonProcessingException {
        User user = getUserRepository().getUserByUID(uuid);
        String userJSON = getObjectMapper().writeValueAsString(user);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + userJSON + ", \"error\": null }");
    }


    public Response createUser(String body) {
        User user = null;
        try {
            user = getObjectMapper().readValue(body, User.class);
            getUserRepository().createUser(user);
            return new Response(
                    HttpStatus.OK,
                    ContentType.JSON,
                    "{ \"data\": \"User successfully created.\", \"error\": null }"
            );
        } catch (RuntimeException e) {
            e.printStackTrace();
            return new Response(
                    HttpStatus.BAD_REQUEST,
                    ContentType.JSON,
                    "{ \"error\": \"User with same username already registered.\", \"data\": null }"
            );
        } catch (JsonProcessingException e) {
            e.printStackTrace();

            return new Response(
                    HttpStatus.INTERNAL_SERVER_ERROR,
                    ContentType.JSON,
                    "{ \"error\": \"Internal Server Error\", \"data\": null }"
            );
        }
    }

    public Response updateUser(String uuid, String body) {
        try {
            User user = null;
            User req_user = getObjectMapper().readValue(body, User.class);
            req_user.setUser_id(UUID.fromString(uuid));
            if ((user = getUserRepository().getUserByUID(req_user.getUser_id().toString())) != null) {
                Map<String, String> req_props = getObjectMapper().convertValue(req_user, Map.class);
                Map<String, String> props = getObjectMapper().convertValue(user, Map.class);
                for (Map.Entry<String, String> ent : req_props.entrySet()) {
                    // prohibit change of id, name, score and coins
                    if (ent.getKey().equals("user_id") || ent.getKey().equals("username")
                            || ent.getKey().equals("score") || ent.getKey().equals("coins")
                            || ent.getKey().equals("wins") || ent.getKey().equals("losses")) {
                        props.remove(ent.getKey());
                        continue;
                    }
                    // password
                    if (ent.getKey().equals("password")) {
                        if (ent.getValue() != null && !ent.getValue().isEmpty()
                                //compare new password to old hash
                                && !BCrypt.checkpw(ent.getValue(), user.getPassword())) {
                            //hash new pw and set hash as value
                            props.replace(ent.getKey(), BCrypt.hashpw(ent.getValue(), BCrypt.gensalt()));
                        } else {
                            props.remove(ent.getKey());
                        }
                        continue;
                    }
                    if (!ent.getValue().equals(props.get(ent.getKey()))) {
                        props.replace(ent.getKey(), String.valueOf(ent.getValue()));
                    } else {
                        props.remove(ent.getKey());
                    }

                }
                getUserRepository().updateUser(user.getUser_id().toString(), props);
                return new Response(
                        HttpStatus.OK,
                        ContentType.JSON,
                        "{ \"data\": \"Success\", \"error\": null }"
                );
            }
            return new Response(
                    HttpStatus.BAD_REQUEST,
                    ContentType.JSON,
                    "{ \"data\": \"\", \"error\": Bad Request}"
            );
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return new Response(
                    HttpStatus.INTERNAL_SERVER_ERROR,
                    ContentType.JSON,
                    "{ \"error\": \"Internal Server Error\", \"data\": null }"
            );
        }
    }

    public Response getScoreBoard() throws JsonProcessingException {
        ArrayList<ScoreEntryDTO> scores = getUserRepository().readScores();
        scores.sort((o1, o2) -> Integer.compare(o2.getScore(), o1.getScore()));
        String scoreJson = getObjectMapper().writeValueAsString(scores);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + scoreJson + ", \"error\": null }");
    }

    public Response getScore(String token) throws JsonProcessingException {
        ScoreEntryDTO score = getUserRepository().readScore(token);
        String scoreJson = getObjectMapper().writeValueAsString(score);
        return new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": " + scoreJson + ", \"error\": null }");
    }
}