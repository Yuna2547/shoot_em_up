#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

// A simple button structure
struct Button {
    SDL_FRect rect;      // Position and size of button
    const char* text;    // Text label for button
    SDL_Color color;     // Button color
    SDL_Color hoverColor; // Color when mouse is over it
    bool isHovered;      // Is mouse currently over this button?
};

// Menu class to handle the main menu
class Menu {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    Button playButton;
    Button quitButton;
    Button resumeButton;  // New button for pausing
    bool isPauseMenu;     // Track if this is pause menu or main menu
    bool isVictoryMenu;
    bool isGameOverMenu;
    int windowWidth;      // Current window width
    int windowHeight;     // Current window height

    bool mouseOverButton(const Button& button, float mouseX, float mouseY);
    void drawButton(const Button& button);
    void drawText(const char* text, int x, int y, SDL_Color color);
    void updateButtonPositions(); // Recalculate button positions

public:
    Menu(SDL_Renderer* renderer, int winWidth = 800, int winHeight = 600);
    ~Menu();

    // Return : 0 = nothing, 1 = play/resume, 2 = quit
    int handleEvents(SDL_Event& event);

    // Draw menu
    void draw();

    // Update button
    void update(float mouseX, float mouseY);

    // Set whether this is a pause menu or main menu
    void setPauseMode(bool isPause);

    // Set victory menu
    void setVictoryMode(bool isVictory);

    void setGameOverMode(bool isGameOver);

    // Update window dimensions and recalculate positions
    void setWindowSize(int width, int height);
};
