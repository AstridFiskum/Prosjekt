#include "AnimationWindow.h"
#include "Point.h"   // For Point
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <cmath>

namespace TDT4102 {

AnimationWindow::AnimationWindow(int x, int y, int width, int height, const std::string &title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL initialization failed");
    }
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        throw std::runtime_error("TTF initialization failed");
    }
    windowHandle = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_SHOWN);
    if (!windowHandle) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Window creation failed");
    }
    rendererHandle = SDL_CreateRenderer(windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rendererHandle) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Renderer creation failed");
    }
    std::cout << "AnimationWindow created successfully." << std::endl;
    closeRequested = false;
}

AnimationWindow::~AnimationWindow() {
    if (rendererHandle) SDL_DestroyRenderer(rendererHandle);
    if (windowHandle) SDL_DestroyWindow(windowHandle);
    TTF_Quit();
    SDL_Quit();
}

void AnimationWindow::pump_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            std::cout << "SDL_QUIT received, closing window." << std::endl;
            closeRequested = true;
        }
    }
}

int AnimationWindow::getMouseX() const {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x;
}

int AnimationWindow::getMouseY() const {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y;
}

Point AnimationWindow::getWindowDimensions() const {
    int w = 0, h = 0;
    if (windowHandle) {
        SDL_GetWindowSize(windowHandle, &w, &h);
    }
    return Point(w, h);
}

void AnimationWindow::next_frame() {
    SDL_RenderPresent(rendererHandle);
    SDL_SetRenderDrawColor(rendererHandle, 0, 0, 0, 255);
    SDL_RenderClear(rendererHandle);
}

bool AnimationWindow::should_close() const {
    return closeRequested;
}

void AnimationWindow::keep_previous_frame(bool enabled) {
    keepPreviousFrame = enabled;
}

void AnimationWindow::wait_for_close() {
    while (!should_close()) {
        pump_events();
        next_frame();
        SDL_Delay(16);
    }
    close();
}

void AnimationWindow::close() {
    closeRequested = true;
}

void AnimationWindow::wait_for(double timeSeconds) {
    SDL_Delay(static_cast<Uint32>(timeSeconds * 1000));
}

// Tegnefunksjoner

void AnimationWindow::draw_circle(Point centre, int radius, Color color, Color borderColor) {
    // Stub: Implementer sirkeltegning om ønskelig
}

void AnimationWindow::draw_rectangle(Point topLeftPoint, int width, int height, Color color, Color borderColor) {
    SDL_SetRenderDrawColor(rendererHandle, color.redChannel, color.greenChannel, color.blueChannel, 255);
    SDL_Rect rect = { topLeftPoint.x, topLeftPoint.y, width, height };
    SDL_RenderFillRect(rendererHandle, &rect);
    SDL_SetRenderDrawColor(rendererHandle, borderColor.redChannel, borderColor.greenChannel, borderColor.blueChannel, 255);
    SDL_RenderDrawRect(rendererHandle, &rect);
}

void AnimationWindow::draw_rounded_rectangle(Point topLeftPoint, int width, int height, int radius, Color fillColor, Color borderColor) {
    // Tegn senterrektangelet
    SDL_Rect centerRect = { topLeftPoint.x + radius, topLeftPoint.y, width - 2 * radius, height };
    SDL_SetRenderDrawColor(rendererHandle, fillColor.redChannel, fillColor.greenChannel, fillColor.blueChannel, fillColor.alphaChannel);
    SDL_RenderFillRect(rendererHandle, &centerRect);
    
    // Tegn venstre og høyre sider
    SDL_Rect leftRect = { topLeftPoint.x, topLeftPoint.y + radius, radius, height - 2 * radius };
    SDL_Rect rightRect = { topLeftPoint.x + width - radius, topLeftPoint.y + radius, radius, height - 2 * radius };
    SDL_RenderFillRect(rendererHandle, &leftRect);
    SDL_RenderFillRect(rendererHandle, &rightRect);
    
    // Tegn fire quarter-sirkler i hjørnene
    for (int dy = 0; dy < radius; dy++) {
        for (int dx = 0; dx < radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                // Top-left
                SDL_RenderDrawPoint(rendererHandle, topLeftPoint.x + radius - dx, topLeftPoint.y + radius - dy);
                // Top-right
                SDL_RenderDrawPoint(rendererHandle, topLeftPoint.x + width - radius + dx, topLeftPoint.y + radius - dy);
                // Bottom-left
                SDL_RenderDrawPoint(rendererHandle, topLeftPoint.x + radius - dx, topLeftPoint.y + height - radius + dy);
                // Bottom-right
                SDL_RenderDrawPoint(rendererHandle, topLeftPoint.x + width - radius + dx, topLeftPoint.y + height - radius + dy);
            }
        }
    }
    // Tegn outline rundt hele rektangelet
    SDL_Rect outline = { topLeftPoint.x, topLeftPoint.y, width, height };
    SDL_SetRenderDrawColor(rendererHandle, borderColor.redChannel, borderColor.greenChannel, borderColor.blueChannel, borderColor.alphaChannel);
    SDL_RenderDrawRect(rendererHandle, &outline);
}

void AnimationWindow::draw_image(Point topLeftPoint, Image& image, int imageWidth, int imageHeight) {
    // Stub: Implementer bildevisning om ønskelig
}

void AnimationWindow::draw_text(Point bottomLeftPoint, std::string textToShow, Color color, unsigned int fontSize, Font font) {
    // Bruk SDL_ttf for å tegne tekst
    TTF_Font* sdlFont = TTF_OpenFont("arial.ttf", fontSize);
    if (!sdlFont) {
        std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Color sdlColor = { color.redChannel, color.greenChannel, color.blueChannel, color.alphaChannel };
    SDL_Surface* surface = TTF_RenderText_Solid(sdlFont, textToShow.c_str(), sdlColor);
    if (!surface) {
        std::cerr << "TTF_RenderText_Solid error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(sdlFont);
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendererHandle, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        TTF_CloseFont(sdlFont);
        return;
    }
    SDL_Rect destRect;
    destRect.x = bottomLeftPoint.x;
    // Vi tolker bottomLeftPoint som bunnen av teksten, så vi flytter opp med tekstens høyde
    destRect.y = bottomLeftPoint.y - surface->h;
    destRect.w = surface->w;
    destRect.h = surface->h;
    SDL_RenderCopy(rendererHandle, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(sdlFont);
}

void AnimationWindow::draw_line(Point start, Point end, Color color) {
    SDL_SetRenderDrawColor(rendererHandle, color.redChannel, color.greenChannel, color.blueChannel, 255);
    SDL_RenderDrawLine(rendererHandle, start.x, start.y, end.x, end.y);
}

void AnimationWindow::draw_triangle(Point vertex0, Point vertex1, Point vertex2, Color color) {
    draw_line(vertex0, vertex1, color);
    draw_line(vertex1, vertex2, color);
    draw_line(vertex2, vertex0, color);
}

void AnimationWindow::draw_quad(Point vertex0, Point vertex1, Point vertex2, Point vertex3, Color color) {
    draw_line(vertex0, vertex1, color);
    draw_line(vertex1, vertex2, color);
    draw_line(vertex2, vertex3, color);
    draw_line(vertex3, vertex0, color);
}

void AnimationWindow::draw_arc(Point center, int width, int height, int start_degree, int end_degree, Color color) {
    // Stub: Implementer tegning av bue om ønskelig
}

// Input- og GUI-funksjoner (stub-implementasjoner)
bool AnimationWindow::is_key_down(KeyboardKey key) const { return false; }
Point AnimationWindow::get_mouse_coordinates() const { return Point(getMouseX(), getMouseY()); }
bool AnimationWindow::is_left_mouse_button_down() const { return false; }
bool AnimationWindow::is_right_mouse_button_down() const { return false; }
void AnimationWindow::add(Widget& widgetToAdd) {}
void AnimationWindow::show_info_dialog(const std::string& message) const {}
void AnimationWindow::show_error_dialog(const std::string& message) const {}
int AnimationWindow::width() const {
    int w, h;
    SDL_GetWindowSize(windowHandle, &w, &h);
    return w;
}
int AnimationWindow::height() const {
    int w, h;
    SDL_GetWindowSize(windowHandle, &w, &h);
    return h;
}
void AnimationWindow::setBackgroundColor(Color newBackgroundColor) {
    backgroundColor = newBackgroundColor;
}
float AnimationWindow::get_delta_mouse_wheel() const { return deltaMouseWheel; }

} // namespace TDT4102
