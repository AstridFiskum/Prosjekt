#include "MemoryGame.h"
#include <algorithm>
#include <random>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <SDL.h>

namespace TDT4102 {

MemoryGame::MemoryGame() 
    : gridRows(2), gridCols(4), cardWidth(100), cardHeight(100) {
    initGame();
}

void MemoryGame::initGame() {
    cards.clear();
    // Oppretter 4 par (8 kort totalt) med verdier 0 til 3
    for (int i = 0; i < (gridRows * gridCols) / 2; ++i) {
        cards.push_back(Card(i));
        cards.push_back(Card(i));
    }
    // Stokker kortene
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

void MemoryGame::update() {
    // Hvis to kort er valgt og vi har startet en timer
    if (selectedIndices.size() == 2 && revealTime != 0) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - revealTime >= 500) {
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
            revealTime = 0;
        }
    }
}

void MemoryGame::drawCards(AnimationWindow &window) {
    // Beregn kortstørrelse dynamisk slik at de fyller vinduet
    int margin = 20;
    int availableWidth = window.width() - (gridCols + 1) * margin;
    int availableHeight = window.height() - (gridRows + 1) * margin;
    int cardW = availableWidth / gridCols;
    int cardH = availableHeight / gridRows;
    for (int i = 0; i < gridRows; ++i) {
        for (int j = 0; j < gridCols; ++j) {
            int index = i * gridCols + j;
            int x = margin + j * (cardW + margin);
            int y = margin + i * (cardH + margin);
            if (cards[index].isRevealed() || cards[index].isMatched()) {
                int value = cards[index].getValue();
                TDT4102::Color cardColor;
                // Bestem farge basert på kortets verdi:
                // 0 -> blå, 1 -> rød, 2 -> grønn, 3 -> rosa
                if (value == 0)
                    cardColor = TDT4102::Color(0, 0, 255);
                else if (value == 1)
                    cardColor = TDT4102::Color(255, 0, 0);
                else if (value == 2)
                    cardColor = TDT4102::Color(0, 255, 0);
                else if (value == 3)
                    cardColor = TDT4102::Color(255, 192, 203);
                window.draw_rounded_rectangle(Point(x, y), cardW, cardH, 20, cardColor, TDT4102::Color(0, 0, 0));
            } else {
                window.draw_rounded_rectangle(Point(x, y), cardW, cardH, 20, TDT4102::Color(100, 100, 100), TDT4102::Color(0, 0, 0));
            }
        }
    }
}

void MemoryGame::processClick(int mouseX, int mouseY) {
    int margin = 20;
    int availableWidth = 800 - (gridCols + 1) * margin; // assuming window width 800 (but we recalc in drawCards)
    int availableHeight = 600 - (gridRows + 1) * margin; // assuming window height 600
    int cardW = availableWidth / gridCols;
    int cardH = availableHeight / gridRows;
    int col = (mouseX - margin) / (cardW + margin);
    int row = (mouseY - margin) / (cardH + margin);
    if (col < 0 || col >= gridCols || row < 0 || row >= gridRows)
        return;
    int index = row * gridCols + col;
    if (cards[index].isRevealed() || cards[index].isMatched())
        return;
    cards[index].reveal();
    selectedIndices.push_back(index);
    if (selectedIndices.size() == 2 && revealTime == 0) {
         revealTime = SDL_GetTicks();
    }
}

bool MemoryGame::isGameOver() const {
    for (const auto &card : cards) {
        if (!card.isMatched())
            return false;
    }
    return true;
}

void MemoryGame::checkMatch() {
    // Denne funksjonen blir nå ikke brukt, da oppdatering skjer i update()
}

} // namespace TDT4102
