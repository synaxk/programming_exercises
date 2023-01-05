package app.controllers;

import app.repositories.BattleRepository;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;
import server.Response;

public class BattleController extends Controller{

    @Getter(AccessLevel.PRIVATE)
    @Setter(AccessLevel.PRIVATE)
    BattleRepository battleRepository;

    public BattleController(BattleRepository battleRepository) {
        setBattleRepository(battleRepository);
    }


    public Response battle(String token) {
        return null;
    }
}
