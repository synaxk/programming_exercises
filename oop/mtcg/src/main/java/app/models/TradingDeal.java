package app.models;

import com.fasterxml.jackson.annotation.JsonAlias;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
@AllArgsConstructor
public class TradingDeal {
    @JsonAlias({"TradingDeal_id"})
    private UUID tradingDeal_id;
    @JsonAlias({"user_id"})
    private UUID user_id;
    @JsonAlias({"card_id"})
    private UUID card_id;
    @JsonAlias({"CardType"})
    private CardType cardType;
    @JsonAlias({"MinDamage"})
    private int minDamage;

    @JsonCreator
    public TradingDeal(@JsonProperty("TradingDeal_id") UUID tradingDeal_id,
                       @JsonProperty("Card_id") UUID card_id,
                        @JsonProperty("CardType") String cardType,
                        @JsonProperty("MinDamage") int minDamage) {
        setTradingDeal_id(tradingDeal_id);
        setCard_id(card_id);
        setCardType(CardType.parseCardName(cardType));
        setMinDamage(minDamage);
    }
}
