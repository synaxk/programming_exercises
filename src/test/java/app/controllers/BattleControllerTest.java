package app.controllers;

import app.models.Card;
import app.repositories.CardRepository;
import app.repositories.UserRepository;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import server.Response;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

import static org.mockito.Mockito.mock;

class BattleControllerTest {

    CardRepository cardRepository;
    UserRepository userRepository;
    BattleController battleController;
    ArrayList<Card> cards;

    @BeforeEach
    void setUp(){
        userRepository = mock(UserRepository.class);
        cardRepository = mock(CardRepository.class);
        battleController = new BattleController(userRepository, cardRepository);
    }

    @Test
    void battle () {
        //arrange
        String tk1 = "altenhof-mtcgToken";
        String tk2 = "kienboec-mtcgToken";

        HashMap<UUID, Card> stack = new HashMap<>();

       // UserDTO user1 = new UserDTO(UUID.randomUUID(), "rudi", 100);

        Response rsp1 = null;
        Response rsp2 = null;



        //act
        rsp1 = battleController.battle(tk1);
        rsp2 = battleController.battle(tk2);
        //assert
    }
}