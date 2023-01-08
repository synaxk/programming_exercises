package app.models;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

public enum CardType {
    MONSTER(0,"monster", "Monster"),
    DRAGON(1, "Dragon", "Monster"),
    FIREELF(2, "FireElf", "Monster"),
    KNIGHT(3, "Knight", "Monster"),
    ORK(4, "Ork", "Monster"),
    WATERGOBLIN(5, "WaterGoblin", "Monster"),

    SPELL(10, "spell", "Spell"),
    FIRESPELL(11, "FireSpell", "Spell"),
    WATERSPELL(12,"WaterSpell", "Spell"),
    REGULARSPELL(13, "RegularSpell", "Spell");

    @Getter
    @Setter(AccessLevel.PRIVATE)
    private int code;
    @Getter
    @Setter(AccessLevel.PRIVATE)
    private String name;

    @Getter
    @Setter(AccessLevel.PRIVATE)
    private String category;
    CardType(int code, String name, String category){
        this.code = code;
        this.name = name;
        this.category = category;
    }

    public static CardType parseCardName(String name) {
        for (CardType t : CardType.values()) {
            if (t.getName().equals(name)) {
                return t;
            }
        }
        return null;
    }

    public static CardType parseCardType(String type) {
        for (CardType t : CardType.values()) {
            if (t.getCategory().equals(type)) {
                return t;
            }
        }
        return null;
    }

    public static CardType parseInt(int value) {
        for (CardType t : CardType.values()) {
            if (t.getCode() == value) {
                return t;
            }
        }
        return null;
    }
}
