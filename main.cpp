#include "AnimationWindow.h"   // Inkluderer definisjonen av TDT4102::AnimationWindow
#include "MemoryGame.h"        // Inkluderer spill-logikken for MemoryGame
#include <SDL.h>               // For SDL_Delay
#include <iostream>
#include <exception>

int main() {
    try {
        // Opprett et vindu med posisjon (300, 100), størrelse (500 x 500) og tittel "Memory Game"
        TDT4102::AnimationWindow window(300, 100, 500, 500, "Memory Game");

        // Opprett spillobjektet uten namespace-prefiks (siden MemoryGame er i global namespace)
        MemoryGame game;

        // Hovedløkke: kjør til brukeren lukker vinduet.
        while (!window.should_close()) {
            // Pump SDL-hendelser slik at lukkeknappen og andre input fanges opp
            window.pump_events();

            // Hent musekoordinatene
            int mouseX = window.getMouseX();
            int mouseY = window.getMouseY();

            // Send musekoordinatene til spillobjektet for å håndtere klikk
            game.processClick(mouseX, mouseY);

            // Tegn spillbrettet (kortene) i vinduet
            game.drawCards(window);

            // Oppdater vinduet slik at tegningen vises
            window.next_frame();

            // En liten forsinkelse for å begrense framerate (ca 60 FPS)
            SDL_Delay(16);
        }

        // Når brukeren lukker vinduet, kalles close() for opprydding
        window.close();
    } catch (const std::exception &e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
