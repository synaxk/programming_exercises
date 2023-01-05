package app.models;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class Stack {
    private UUID stack_id;
    private UUID card_id;
    public Stack() {}

}
