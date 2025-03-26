#pragma once

class Card {
public:
    Card(int value);
    void reveal();
    void hide();
    bool isRevealed() const;
    bool isMatched() const;
    void setMatched();
    int getValue() const;
private:
    int value;
    bool revealed;
    bool matched;
};
