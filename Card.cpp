#include "Card.h"

Card::Card(int value) : value(value), revealed(false), matched(false) {}

void Card::reveal() {
    revealed = true;
}

void Card::hide() {
    revealed = false;
}

bool Card::isRevealed() const {
    return revealed;
}

bool Card::isMatched() const {
    return matched;
}

void Card::setMatched() {
    matched = true;
}

int Card::getValue() const {
    return value;
}
