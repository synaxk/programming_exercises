package app.models;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

public enum CardType {
    MONSTER(0,"monster", "Monster"),
    DRAGON(1, "Dragon", "Monster","normal"),
    FIREELF(2, "FireElf", "Monster", "fire"),
    KNIGHT(3, "Knight", "Monster","normal"),
    ORK(4, "Ork", "Monster", "normal"),
    WATERGOBLIN(5, "WaterGoblin", "Monster","water"),

    SPELL(10, "spell", "Spell"),
    FIRESPELL(11, "FireSpell", "Spell", "fire"),
    WATERSPELL(12,"WaterSpell", "Spell", "water"),
    REGULARSPELL(13, "RegularSpell", "Spell", "normal");

    @Getter
    @Setter(AccessLevel.PRIVATE)
    private int code;
    @Getter
    @Setter(AccessLevel.PRIVATE)
    private String name;
    @Getter
    @Setter(AccessLevel.PRIVATE)
    private String category;
    @Getter
    @Setter(AccessLevel.PRIVATE)
    private String element;
    CardType(int code, String name, String category){
        this.code = code;
        this.name = name;
        this.category = category;
    }
    CardType(int code, String name, String category, String element){
        this.code = code;
        this.name = name;
        this.category = category;
        this.element = element;
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
