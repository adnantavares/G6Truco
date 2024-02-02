#ifndef CARD_H
#define CARD_H

class Card {
public:
    enum Suit { HEARTS, SPADES, DIAMONDS, CLUBS };
    enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

private:
    Suit suit;
    Rank rank;

public:
    Card();
    Card(Suit suit, Rank rank);

#pragma region Getters and Setter
    Suit GetSuit() const;
    Rank GetRank() const;
#pragma endregion

};
#endif //CARD_H