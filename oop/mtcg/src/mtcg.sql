-- Create the Database for MonsterTradingCardsGame

CREATE DATABASE mtcg;
\c mtcg;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE Cards (
    card_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    name VARCHAR (64) NOT NULL,
    damage INT NOT NULL, -- wird noch erweitert
    cardType INT NOT NULL,
    PRIMARY KEY(card_id)
);

CREATE TABLE Users (
    user_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    username VARCHAR(64) UNIQUE NOT NULL,
    password VARCHAR(64) NOT NULL,
    --score, coins
    score INT NOT NULL,
    coins INT NOT NULL,
    bio VARCHAR(255) DEFAULT '',
    image VARCHAR(64) DEFAULT '',
    wins INT,
    losses INT,
    PRIMARY KEY(user_id)
);

CREATE TABLE Packs (
    pack_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    name VARCHAR(64),
    price INT,
    PRIMARY KEY(pack_id)
);

CREATE TABLE StackCards(
    stackCard_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    user_id UUID NOT NULL,
    card_id UUID NOT NULL,
    PRIMARY KEY (stackCard_id),
    CONSTRAINT fk_user FOREIGN KEY(user_id) REFERENCES users(user_id),
    CONSTRAINT fk_card FOREIGN KEY(card_id) REFERENCES cards(card_id)
);

CREATE TABLE DeckCards (
    deckCard_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    user_id UUID NOT NULL,
    card_id UUID NOT NULL,
    PRIMARY KEY (deckCard_id),
    CONSTRAINT fk_user FOREIGN KEY(user_id) REFERENCES users(user_id),
    CONSTRAINT fk_card FOREIGN KEY(card_id) REFERENCES cards(card_id)
);

CREATE TABLE PackCards (
    packageCard_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    pack_id UUID NOT NULL,
    card_id UUID NOT NULL,
    PRIMARY KEY (packageCard_id),
    CONSTRAINT fk_pack FOREIGN KEY(pack_id) REFERENCES packs(pack_id),
    CONSTRAINT fk_card FOREIGN KEY(card_id) REFERENCES cards(card_id)
);

CREATE TABLE TradingDeals (
    tradingDeal_id UUID NOT NULL DEFAULT uuid_generate_v4(),
    user_id UUID NOT NULL,
    card_id UUID NOT NULL,
    cardType int,
    minDamage int,
    PRIMARY KEY (tradingDeal_id),
    CONSTRAINT fk_user FOREIGN KEY(user_id) REFERENCES users(user_id),
    CONSTRAINT fk_card FOREIGN KEY(card_id) REFERENCES cards(card_id)
)
