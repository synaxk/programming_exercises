package app.models;

import com.fasterxml.jackson.annotation.JsonAlias;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class Pack {
    @JsonAlias({"Pack_id"})
    private UUID pack_id;
    @JsonAlias({"Name"})
    private String name;
    @JsonAlias({"Price"})
    private int price;

    public Pack() {
        this.pack_id = UUID.randomUUID();
        this.name = "Pack" + pack_id.toString();
        this.price = 5;
    }
}
