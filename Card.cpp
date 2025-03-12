#include "Card.h"   // Inkluderer header-filen for å implementere funksjonene til Card-klassen

// Konstruktør: Initialiserer et kort med en gitt verdi, og setter standardverdier for matched og revealed
Card::Card(int value) : value(value), matched(false), revealed(false) {}

// Funksjon som setter flagget for at kortet er avslørt til true
void Card::reveal() {
    revealed = true;  // Kortet blir synlig
}

// Funksjon som returnerer om kortet er matchet eller ikke
bool Card::isMatched() const {
    return matched;  // Returnerer true hvis kortet er matchet, ellers false
}

// Funksjon som markerer kortet som matchet (setter flagget til true)
void Card::setMatched() {
    matched = true;
}

// Funksjon som returnerer kortets verdi
int Card::getValue() const {
    return value;
}

// Funksjon som sjekker om kortet er synlig (avslørt)
bool Card::isRevealed() const {
    return revealed;
}

// Funksjon for å snu kortet tilbake (skjule kortets verdi) ved å sette revealed-flagget til false
void Card::hide() {
    revealed = false;
}
