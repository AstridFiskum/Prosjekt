#include "MemoryGame.h"
#include <algorithm>
#include <random>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <SDL.h>

namespace TDT4102 {

MemoryGame::MemoryGame() 
    : gridRows(2), gridCols(4), cardWidth(100), cardHeight(100), revealTime(0), attempts(0)
{
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
    int margin = 20; // Margin mellom kortene
    int availableWidth = window.width() - (gridCols + 1) * margin;
    int availableHeight = window.height() - (gridRows + 60); // Reservér 60 piksler for feedback-tekst
    int cardW = availableWidth / gridCols;
    int cardH = availableHeight / gridRows;
    
    for (int i = 0; i < gridRows; ++i) {
        for (int j = 0; j < gridCols; ++j) {
            int index = i * gridCols + j;
            int x = margin + j * (cardW + margin);
            int y = margin + i * (cardH + margin);
            if (cards[index].isRevealed() || cards[index].isMatched()) {
                int value = cards[index].getValue();
                Color cardColor;
                // Velg farge basert på kortets verdi:
                // 0 -> blå, 1 -> rød, 2 -> grønn, 3 -> rosa
                if (value == 0)
                    cardColor = Color(0, 0, 255);
                else if (value == 1)
                    cardColor = Color(255, 0, 0);
                else if (value == 2)
                    cardColor = Color(0, 255, 0);
                else if (value == 3)
                    cardColor = Color(255, 192, 203);
                window.draw_rounded_rectangle(Point(x, y), cardW, cardH, 20, cardColor, Color(0, 0, 0));
            } else {
                window.draw_rounded_rectangle(Point(x, y), cardW, cardH, 20, Color(100, 100, 100), Color(0, 0, 0));
            }
        }
    }
}

void MemoryGame::processClick(int mouseX, int mouseY) {
    int margin = 20;
    int availableWidth = 800 - (gridCols + 1) * margin;  // Antatt vindusbredde (brukes kun for beregning)
    int availableHeight = 600 - (gridRows + 60);         // Antatt vindushøyde, med plass til feedback
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
         attempts++;  // Øk forsøk når to kort er valgt
    }
}

bool MemoryGame::isGameOver() const {
    for (const auto &card : cards) {
        if (!card.isMatched())
            return false;
    }
    return true;
}

void MemoryGame::drawFeedback(AnimationWindow &window) {
    std::string feedback;
    if (isGameOver()) {
        feedback = "Gratulerer, du klarte det paa " + std::to_string(attempts) + " forsok!";
    } else {
        feedback = "Antall forsok: " + std::to_string(attempts);
    }
    // Tegn feedback-teksten nederst i vinduet, med litt margin fra bunnen
    window.draw_text(Point(20, window.height() - 20), feedback, Color(255, 255, 255), 24, Font::arial);
}

} // namespace TDT4102
