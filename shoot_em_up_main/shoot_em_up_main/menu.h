#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <string>

enum class Button_Type {
    Play,
    Resume,
    Stop,
    Replay
};



// A simple button structure
struct Button {
    SDL_FRect rect;      // Position and size of button
    const char* text;    // Text label for button
    SDL_Color color;     // Button color
    SDL_Color hoverColor; // Color when mouse is over it
    bool isHovered;      // Is mouse currently over this button?
    SDL_Texture* texture;
    SDL_Texture* hoverTexture;
    

};

// Menu class to handle the main menu
class Menu {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    Button playButton;
    Button quitButton;
    Button resumeButton;  // New button for pausing
    Button replayButton;
    bool isPauseMenu;     // Track if this is pause menu or main menu
    bool isVictoryMenu;
    bool isGameOverMenu;
    int windowWidth;      // Current window width
    int windowHeight;     // Current window height
   

    bool mouseOverButton(const Button& button, float mouseX, float mouseY) const;
    void drawButton(const Button& button);
    void drawText(const char* text, int x, int y, SDL_Color color);
    void updateButtonPositions(); // Recalculate button positions

    void loadButtonTexture(Button& button, const char* normalPath, const char* hoverPath = nullptr);

public:
    Menu(SDL_Renderer* renderer, int winWidth = 800, int winHeight = 600);
    ~Menu();

    // Return : 0 = nothing, 1 = play/resume, 2 = quit
    int handleEvents(SDL_Event& event);

    // Draw menu
    void draw();

    // Update button
    void update(float mouseX, float mouseY);
    void setPauseMode(bool isPause);
    void setVictoryMode(bool isVictory);
    void setGameOverMode(bool isGameOver);
    void setWindowSize(int width, int height);
};
