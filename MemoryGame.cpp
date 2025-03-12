#include "AnimationWindow.h"  // Inkluderer den fulle definisjonen av AnimationWindow
#include "MemoryGame.h"       // Inkluderer header-filen for MemoryGame-klassen

#include <algorithm>          // For std::shuffle
#include <random>             // For tilfeldighetsgenerator
#include <fstream>            // For filhåndtering
#include <iostream>           // For input/output
#include <stdexcept>          // For unntakshåndtering

// Konstruktør for MemoryGame, initialiserer grid-størrelse, kortdimensjoner og kaller initGame og loadGameData
MemoryGame::MemoryGame() : gridRows(4), gridCols(4), cardWidth(100), cardHeight(100) {
    initGame();        // Initialiserer spillbrettet
    loadGameData();    // Leser evt. spilldata fra fil
}

// Funksjon for å initialisere eller tilbakestille spillet
void MemoryGame::initGame() {
    cards.clear();  // Fjerner eksisterende kort, hvis noen
    
    // Oppretter 8 par (16 kort totalt) med verdier 0 til 7
    for (int i = 0; i < (gridRows * gridCols) / 2; ++i) {
        cards.push_back(Card(i));  // Legger til første kort i paret
        cards.push_back(Card(i));  // Legger til andre kort i paret
    }
    
    // Stokker kortene for å plassere dem tilfeldig i rutenettet
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

// Funksjon for filhåndtering. Leser spilldata fra "game_data.txt" om filen finnes.
void MemoryGame::loadGameData() {
    std::ifstream infile("game_data.txt");
    if (!infile) {
        std::cerr << "Warning: game_data.txt ikke funnet. Bruker standardinnstillinger." << std::endl;
        return;
    }
    int dummy;
    infile >> dummy;
    if (infile.fail()) {
        throw std::runtime_error("Feil ved lesing av game_data.txt");
    }
}

// Funksjon for å tegne kortene i vinduet
void MemoryGame::drawCards(AnimationWindow &window) {
    // Går gjennom alle rader og kolonner i rutenettet
    for (int i = 0; i < gridRows; ++i) {
        for (int j = 0; j < gridCols; ++j) {
            int index = i * gridCols + j;  // Beregner indeks for kortet
            int x = j * cardWidth;         // Kalkulerer x-koordinat for kortet
            int y = i * cardHeight;        // Kalkulerer y-koordinat for kortet
            
            // Dersom kortet er avslørt eller allerede matchet, skal kortets verdi vises med en farge
            if (cards[index].isRevealed() || cards[index].isMatched()) {
                int value = cards[index].getValue();
                // Kalkulerer en farge basert på kortets verdi
                int r = (value * 50) % 256;
                int g = (value * 80) % 256;
                int b = (value * 110) % 256;
                // Tegn et fylt rektangel med beregnet farge og en svart kant
                window.draw_rectangle(TDT4102::Point(x, y), cardWidth, cardHeight,
                                      TDT4102::Color(r, g, b), TDT4102::Color(0, 0, 0));
            } else {
                // Dersom kortet er skjult, tegnes et rektangel med grå farge og svart kant
                window.draw_rectangle(TDT4102::Point(x, y), cardWidth, cardHeight,
                                      TDT4102::Color(100, 100, 100), TDT4102::Color(0, 0, 0));
            }
        }
    }
}

// Funksjon for å håndtere museklikk. Den finner hvilket kort som er klikket på og håndterer logikken.
void MemoryGame::processClick(int mouseX, int mouseY) {
    int col = mouseX / cardWidth;
    int row = mouseY / cardHeight;
    
    // Sjekk at museklikket er innenfor rutenettet
    if (col >= gridCols || row >= gridRows)
        return;
    
    int index = row * gridCols + col;
    // Ignorer klikk hvis kortet allerede er avslørt eller matchet
    if (cards[index].isRevealed() || cards[index].isMatched())
        return;
    
    cards[index].reveal();
    selectedIndices.push_back(index);
    
    // Hvis to kort er valgt, sjekk om de matcher
    if (selectedIndices.size() == 2) {
        checkMatch();
    }
}

// Privat funksjon for å sjekke om de to valgte kortene matcher
void MemoryGame::checkMatch() {
    int firstIndex = selectedIndices[0];
    int secondIndex = selectedIndices[1];
    
    if (cards[firstIndex].getValue() == cards[secondIndex].getValue()) {
        cards[firstIndex].setMatched();
        cards[secondIndex].setMatched();
    } else {
        cards[firstIndex].hide();
        cards[secondIndex].hide();
    }
    selectedIndices.clear();
}

// Funksjon for å sjekke om spillet er over (alle kort er matchet)
bool MemoryGame::isGameOver() const {
    for (const auto &card : cards) {
        if (!card.isMatched())
            return false;
    }
    return true;
}
