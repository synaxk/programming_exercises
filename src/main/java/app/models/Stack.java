package app.model;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;

public class Stack extends CardCollection {
    @Getter
    @Setter
    private int stack_id;

    public Stack() {
        this.cards = new ArrayList<>();
    }

}
