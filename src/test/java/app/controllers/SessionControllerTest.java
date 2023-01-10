package app.controllers;

import app.controllers.SessionController;
import app.models.User;
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

import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class SessionControllerTest {


    User user;
    UserRepository userRepository;
    SessionController sessionController;

    @BeforeEach
    void setUp(){
        user = new User(UUID.randomUUID(), "test", BCrypt.hashpw("test", BCrypt.gensalt()));
        userRepository = mock(UserRepository.class);
        sessionController = new SessionController(userRepository);
    }


    @Test
    @DisplayName("Login Success")
    void loginSuccess() {
        //arrange
        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{\"data\": \"User login successful, Token: test-mtcgToken\", \"error\": null}");

        String body = "{\"Username\":\"test\", \"Password\":\"test\"}";
        Mockito.when(userRepository.getUserByName("test")).thenReturn(user);

        //act
        Response response = null;
        try {
            response = sessionController.login(body);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Login Fail")
    void loginFail() {
        //arrange
        Response exp_response = new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                "{\"data\": null, \"error\": \"Invalid username/password provided\"}");

        String body = "{\"Username\":\"test\", \"Password\":\"fail\"}";
        Mockito.when(userRepository.getUserByName("test")).thenReturn(user);

        //act
        Response response = null;
        try {
            response = sessionController.login(body);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Verify Token Success")
    void verifyAuthTokenSuccess() {
        //arrange
        String token = "test-mtcgToken";
        Mockito.when(userRepository.getUserByName("test")).thenReturn(user);
        //act
        boolean result = sessionController.verifyAuthToken(token);
        //assert
        assertTrue(result);
    }

    @Test
    @DisplayName("Verify Token Fail")
    void verifyAuthTokenFail() {
        //arrange
        String token = "fail-mtcgToken";
        Mockito.when(userRepository.getUserByName("test")).thenReturn(user);
        //act
        boolean result = sessionController.verifyAuthToken(token);
        //assert
        assertFalse(result);
    }

    @Test
    @DisplayName("Authorize Success")
    void authorizeSuccess() {
        //arrange
        String token = "test-mtcgToken";
        String user_id = user.getUser_id().toString();
        Mockito.when(userRepository.getUserByUID(user_id)).thenReturn(user);

        //act
        boolean result = sessionController.authorize(token, user_id);
        //assert
        assertTrue(result);
    }

    @Test
    @DisplayName("Authorize Fail")
    void authorizeFail() {
        //arrange
        String token = "fail-mtcgToken";
        String user_id = user.getUser_id().toString();
        Mockito.when(userRepository.getUserByUID(user_id)).thenReturn(user);

        //act
        boolean result = sessionController.authorize(token, user_id);
        //assert
        assertFalse(result);
    }


}
