package app.controllers;

import app.models.User;
import app.repositories.PackRepository;
import app.repositories.UserRepository;
import http.ContentType;
import http.HttpStatus;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import server.Response;

import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;

class PackControllerTest {

    PackController packController;
    PackRepository packRepository;
    UserRepository userRepository;

    @BeforeEach
    void setUp(){
        packRepository = mock(PackRepository.class);
        userRepository = mock(UserRepository.class);
        packController = new PackController(packRepository, userRepository);
    }

    @Test
    @DisplayName("Create Package Success")
    void createPackageSuccess() {
        String body = "[{\"Card_id\":\"67f9048f-99b8-4ae4-b866-d8008d00c53d\", \"Name\":\"WaterGoblin\", " +
                "\"Damage\": 10.0}, {\"Card_id\":\"aa9999a0-734c-49c6-8f4a-651864b14e62\", \"Name\":\"RegularSpell\", " +
                "\"Damage\": 50.0}, {\"Card_id\":\"d6e9c720-9b5a-40c7-a6b2-bc34752e3463\", \"Name\":\"Knight\", " +
                "\"Damage\": 20.0}, {\"Card_id\":\"02a9c76e-b17d-427f-9240-2dd49b0d3bfd\", \"Name\":\"RegularSpell\", " +
                "\"Damage\": 45.0}, {\"Card_id\":\"2508bf5c-20d7-43b4-8c77-bc677decadef\", \"Name\":\"FireElf\", " +
                "\"Damage\": 25.0}]";

        Response exp_response = new Response(HttpStatus.OK, ContentType.JSON,
                "{ \"data\": \"Package and cards successfully created.\", \"error\": null }"
        );

        Response response = packController.createPackage(body);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Create Package Fail")
    void createPackageFail() {
        String body = "[{\"Card_id\":\"67f9048f-99b8-4ae4-b866-d8008d00c53d\", \"Name\":\"WaterGoblin\", " +
                "\"Damage\": ";

        Response exp_response = new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                "{ \"error\": \"At least one card in the packages already exists.\", \"data\": null }"
        );

        Response response = packController.createPackage(body);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Acquire Package Success")
    void acquirePackageSuccess() {
        User user = mock(User.class);
        String token = "test-mtcgToken";
        user.setUser_id(UUID.randomUUID());
        Mockito.when(userRepository.getUserByName(any(String.class))).thenReturn(user);
        Mockito.when(user.getCoins()).thenReturn(10);
        Mockito.when(packRepository.acquirePack(any(User.class))).thenReturn(5);
        Mockito.when(user.getUser_id()).thenReturn(UUID.randomUUID());
        Response exp_response = new Response(HttpStatus.FORBIDDEN, ContentType.JSON,
                "{\"data\":\"A package has been successfully bought.\",\"error\":\"null\"}"
        );


        Response response = packController.acquirePackage(token);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Acquire Package No Money")
    void acquirePackageNoMoney() {
        User user = mock(User.class);
        String token = "test-mtcgToken";
        Mockito.when(userRepository.getUserByName(any(String.class))).thenReturn(user);
        Mockito.when(user.getCoins()).thenReturn(0);
        Response exp_response = new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                "{ \"error\": \"Not enough money for buying a card package.\", \"data\": null }"
        );


        Response response = packController.acquirePackage(token);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }

    @Test
    @DisplayName("Acquire Package No Package")
    void acquirePackageNoPackage() {
        User user = mock(User.class);
        String token = "test-mtcgToken";
        Mockito.when(userRepository.getUserByName(any(String.class))).thenReturn(user);
        Mockito.when(user.getCoins()).thenReturn(10);
        Mockito.when(packRepository.acquirePack(any(User.class))).thenReturn(0);
        Response exp_response = new Response(HttpStatus.BAD_REQUEST, ContentType.JSON,
                "{ \"error\": \"No card package available for buying.\", \"data\": null }"
        );


        Response response = packController.acquirePackage(token);

        //assert
        assertEquals(exp_response.getStatusCode(), response.getStatusCode());
        assertEquals(exp_response.getContentType(), response.getContentType());
        assertEquals(exp_response.getContent(), response.getContent());
    }
}