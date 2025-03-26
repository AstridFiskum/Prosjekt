#pragma once

#include <vector>
#include "Card.h"
#include "AnimationWindow.h"

namespace TDT4102 {

class MemoryGame {
public:
    MemoryGame();
    void initGame();
    void drawCards(AnimationWindow &window);
    void processClick(int mouseX, int mouseY);
    void update(); // Ny metode for å oppdatere kortenes tilstand etter en forsinkelse
    bool isGameOver() const;
private:
    std::vector<Card> cards;
    std::vector<int> selectedIndices;
    int gridRows;
    int gridCols;
    int cardWidth;
    int cardHeight;
    Uint32 revealTime = 0; // Tidspunkt når to kort ble valgt
    void checkMatch();
};

} // namespace TDT4102
