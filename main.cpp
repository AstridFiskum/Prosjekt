#include "AnimationWindow.h"
#include "MemoryGame.h"
#include <SDL.h>
#include <iostream>
#include <exception>

int main() {
    try {
        // Opprett et større vindu, f.eks. 800x600, slik at kortene fyller plassen
        TDT4102::AnimationWindow window(200, 100, 800, 600, "Memory Game");
        TDT4102::MemoryGame game;
        
        bool mouseWasDown = false;
        while (!window.should_close()) {
            window.pump_events();
            int mouseX = window.getMouseX();
            int mouseY = window.getMouseY();
            
            Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
            bool mouseDown = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
            if (mouseDown && !mouseWasDown) {
                game.processClick(mouseX, mouseY);
            }
            mouseWasDown = mouseDown;
            
            // Oppdater spilltilstanden (sjekk om 500 ms har passert for to valgte kort)
            game.update();
            game.drawCards(window);
            window.next_frame();
            SDL_Delay(16);
        }
        window.close();
    } catch (const std::exception &e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
