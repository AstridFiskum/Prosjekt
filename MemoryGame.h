#pragma once

#include <vector>
#include "Card.h"
#include "AnimationWindow.h"

namespace TDT4102 {

class MemoryGame {
public:
    MemoryGame();
    void initGame();
    void update();            // Oppdaterer spilltilstanden etter kortvalg
    void drawCards(AnimationWindow &window);
    void processClick(int mouseX, int mouseY);
    void drawFeedback(AnimationWindow &window); // Tegner live feedback nederst i vinduet
    bool isGameOver() const;
private:
    std::vector<Card> cards;
    std::vector<int> selectedIndices;
    int gridRows;
    int gridCols;
    int cardWidth;
    int cardHeight;
    Uint32 revealTime = 0;    // Tidspunkt for når to kort ble valgt
    int attempts = 0;         // Antall forsøk (hver gang to kort velges)
};

} // namespace TDT4102
