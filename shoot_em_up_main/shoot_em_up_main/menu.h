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
    Button resumeButton;
    Button replayButton;
    Button level1Button;  // Button to go to Level 1
    Button level2Button;  // Button to go to Level 2
    bool isPauseMenu;     // Track if this is pause menu or main menu
    bool isVictoryMenu;
    bool isGameOverMenu;
    int currentLevel;
    int windowWidth;      // Current window width
    int windowHeight;     // Current window height

    // members for title image
    SDL_Texture* titleTexture;
    int titleWidth;
    int titleHeight;

    bool mouseOverButton(const Button& button, float mouseX, float mouseY) const;
    void drawButton(const Button& button);
    void drawText(const char* text, int x, int y, SDL_Color color);
    void updateButtonPositions(); // Recalculate button positions

    void loadButtonTexture(Button& button, const char* normalPath, const char* hoverPath = nullptr);

public:
    Menu(SDL_Renderer* renderer, int winWidth = 800, int winHeight = 600);
    ~Menu();

    int handleEvents(SDL_Event& event);

    // Draw menu
    void draw();

    // Update button
    void update(float mouseX, float mouseY);
    void setPauseMode(bool isPause);
    void setVictoryMode(bool isVictory);
    void setGameOverMode(bool isGameOver);
    void setWindowSize(int width, int height);
    void setCurrentLevel(int level);
};