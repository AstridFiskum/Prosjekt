#pragma once

#include <vector>
#include "Card.h"
#include "AnimationWindow.h"  // Inkluderer full definisjon av AnimationWindow (definert i namespace TDT4102)

// Ved å bruke "using" får vi lov til å skrive AnimationWindow direkte istedenfor TDT4102::AnimationWindow
using TDT4102::AnimationWindow;

/// <summary>
/// Klassen MemoryGame håndterer logikken for memory-spillet.
/// Den oppretter, stokker og tegner kortene, samt håndterer brukerinteraksjon og sjekker for match.
/// </summary>
class MemoryGame {
public:
    // Konstruktør: Initialiserer spillet med et 4x4 rutenett og laster evt. spilldata
    MemoryGame();
    
    // Funksjon for å initialisere eller tilbakestille spillet
    void initGame();
    
    // Funksjon for å tegne kortene i vinduet.
    // Parameter: window - referanse til AnimationWindow hvor kortene tegnes
    void drawCards(AnimationWindow &window);
    
    // Funksjon for å håndtere museklikk.
    // Parameter: mouseX og mouseY - koordinatene til museklikket
    void processClick(int mouseX, int mouseY);
    
    // Funksjon for å sjekke om spillet er over (alle kort er matchet).
    // Returnerer: true dersom alle kort er matchet, ellers false.
    bool isGameOver() const;

private:
    std::vector<Card> cards;         // Vektor som lagrer alle kortene i spillet
    std::vector<int> selectedIndices; // Vektor som lagrer indeksene til kort som er midlertidig valgt
    int gridRows;                    // Antall rader i rutenettet (her 4)
    int gridCols;                    // Antall kolonner i rutenettet (her 4)
    int cardWidth;                   // Bredden på hvert kort (i piksler)
    int cardHeight;                  // Høyden på hvert kort (i piksler)
    
    // Privat funksjon for å sjekke om to valgte kort matcher.
    // Hvis kortene har samme verdi, markeres de som matchet.
    void checkMatch();
    
    // Privat funksjon for filhåndtering. Her leses spilldata (f.eks. konfigurasjon) fra en fil.
    void loadGameData();
};
