#include "menu.h"
#include <SDL3_image/SDL_image.h>

Menu::Menu(SDL_Renderer* renderer, int winWidth, int winHeight)
    : renderer(renderer), font(nullptr), isPauseMenu(false), isVictoryMenu(false),
    isGameOverMenu(false), windowWidth(winWidth), windowHeight(winHeight) {

    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init Error: %s", SDL_GetError());
    }
    else {
        // Try to load a font - adjust path as needed for your system
        font = TTF_OpenFont("arial.ttf", 32);
        if (!font) {
            // Try alternative common font locations
            font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);
            if (!font) {
                font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 32);
                if (!font) {
                    SDL_Log("Failed to load font: %s", SDL_GetError());
                }
            }
        }
    }


    // Initialize Play button
    playButton.text = "Play";
    playButton.color = { 0, 180, 0, 255 };        // Green
    playButton.hoverColor = { 0, 220, 0, 255 };   // Brighter green
    playButton.isHovered = false;
    playButton.texture = nullptr;
    playButton.hoverTexture = nullptr;

    // Initialize Resume button
    resumeButton.text = "Resume";
    resumeButton.color = { 0, 180, 0, 255 };
    resumeButton.hoverColor = { 0, 220, 0, 255 };
    resumeButton.isHovered = false;
    resumeButton.texture = nullptr;
    resumeButton.hoverTexture = nullptr;

    // Initialize Replay button
    replayButton.text = "Replay";
    replayButton.color = { 0, 150, 200, 255 };
    replayButton.hoverColor = { 0, 180, 230, 255 };
    replayButton.isHovered = false;
    replayButton.texture = nullptr;
    replayButton.hoverTexture = nullptr;

    // Initialize Quit button
    quitButton.text = "Quit";
    quitButton.color = { 180, 0, 0, 255 };
    quitButton.hoverColor = { 220, 0, 0, 255 };
    quitButton.isHovered = false;
    quitButton.texture = nullptr;
    quitButton.hoverTexture = nullptr;

    loadButtonTexture(playButton, "assets/play.png", "assets/play_hover.png");
    loadButtonTexture(resumeButton, "assets/resume.png", "assets/resume_hover.png");
    loadButtonTexture(replayButton, "assets/replay.png", "assets/replay_hover.png");
    loadButtonTexture(quitButton, "assets/stop.png", "assets/stop_hover.png");



    // Set initial positions
    updateButtonPositions();
}

Menu::~Menu() {
    // Destroy button textures
    if (playButton.texture) SDL_DestroyTexture(playButton.texture);
    if (playButton.hoverTexture) SDL_DestroyTexture(playButton.hoverTexture);

    if (resumeButton.texture) SDL_DestroyTexture(resumeButton.texture);
    if (resumeButton.hoverTexture) SDL_DestroyTexture(resumeButton.hoverTexture);

    if (replayButton.texture) SDL_DestroyTexture(replayButton.texture);
    if (replayButton.hoverTexture) SDL_DestroyTexture(replayButton.hoverTexture);

    if (quitButton.texture) SDL_DestroyTexture(quitButton.texture);
    if (quitButton.hoverTexture) SDL_DestroyTexture(quitButton.hoverTexture);


    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();

}

void Menu::loadButtonTexture(Button& button, const char* normalPath, const char* hoverPath) {
    // Load normal texture (SDL3 method)
    SDL_Surface* surface = IMG_Load(normalPath);
    if (surface) {
        button.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        if (!button.texture) {
            SDL_Log("Failed to create texture from %s: %s", normalPath, SDL_GetError());
        }
        else {
            SDL_Log("Successfully loaded texture: %s", normalPath);
        }
    }
    else {
        SDL_Log("Failed to load image %s: %s", normalPath, SDL_GetError());
    }

    if (hoverPath) {
        surface = IMG_Load(hoverPath);
        if (surface) {
            button.hoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            if (!button.hoverTexture) {
                SDL_Log("Failed to create hover texture from %s: %s", hoverPath, SDL_GetError());
            }
            else {
                SDL_Log("Successfully loaded hover texture: %s", hoverPath);
            }
        }
        else {
            SDL_Log("Failed to load hover image %s: %s", hoverPath, SDL_GetError());
        }
    }
}


void Menu::setPauseMode(bool isPause) {
    isPauseMenu = isPause;
    if (isPause) {
        isVictoryMenu = false;
        isGameOverMenu = false;
    }
}

void Menu::setVictoryMode(bool isVictory){
    isVictoryMenu = isVictory;
    if (isVictory) {
        isPauseMenu = true;  // Victory menu is a type of pause menu
        isGameOverMenu = false;
    }
}

void Menu::setGameOverMode(bool isGameOver) {
    isGameOverMenu = isGameOver;
    if (isGameOver) {
        isPauseMenu = true;  // Game over menu is a type of pause menu
        isVictoryMenu = false;
    }
}

void Menu::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    updateButtonPositions();
}

void Menu::updateButtonPositions() {
    // Button dimensions
    float buttonWidth = 155.0f;
    float buttonHeight = 75.0f;
    float buttonSpacing = 40.0f;

    // Center horizontally
    float centerX = (windowWidth - buttonWidth) / 2.0f;

    // Position vertically (centered with spacing)
    float startY = (windowHeight - (buttonHeight * 2 + buttonSpacing)) / 2.0f;

    // Play / Resume / Replay button(first button) - ALL THREE NEED THE SAME POSITION
        playButton.rect = { centerX, startY, buttonWidth, buttonHeight };
    resumeButton.rect = { centerX, startY, buttonWidth, buttonHeight };
    replayButton.rect = { centerX, startY, buttonWidth, buttonHeight };  // THIS WAS MISSING!

    // Quit button (second button)
    quitButton.rect = { centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
}

bool Menu::mouseOverButton(const Button& button, float mouseX, float mouseY) {
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
        mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}

int Menu::handleEvents(SDL_Event& event) {
    //check mouse clicks
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX = event.button.x;
        float mouseY = event.button.y;

        if (isPauseMenu) {
            // Victory or Game Over: show replay button
            if (isVictoryMenu || isGameOverMenu) {
                if (mouseOverButton(replayButton, mouseX, mouseY)) {
                    return 1; // replay clicked
                }
            }
            // Just paused: show resume button
            else {
                if (mouseOverButton(resumeButton, mouseX, mouseY)) {
                    return 1; // resume clicked
                }
            }
        }
        else {
            // Main menu: show play button
            if (mouseOverButton(playButton, mouseX, mouseY)) {
                return 1; // play clicked
            }
        }

        if (mouseOverButton(quitButton, mouseX, mouseY)) {
            return 2; // quit clicked
        }
    }
    return 0; // still in menu
}

void Menu::update(float mouseX, float mouseY) {
    // Reset all hover states first
    playButton.isHovered = false;
    resumeButton.isHovered = false;
    replayButton.isHovered = false;
    quitButton.isHovered = false;

    if (isPauseMenu) {
        // Victory or Game Over: update replay button hover
        if (isVictoryMenu || isGameOverMenu) {
            replayButton.isHovered = mouseOverButton(replayButton, mouseX, mouseY);
     
        }
        // Just paused: update resume button hover
        else {
            resumeButton.isHovered = mouseOverButton(resumeButton, mouseX, mouseY);
        }
    }
    else {
        // Main menu: update play button hover
        playButton.isHovered = mouseOverButton(playButton, mouseX, mouseY);
      
    }
    quitButton.isHovered = mouseOverButton(quitButton, mouseX, mouseY);
}

void Menu::drawText(const char* text, int x, int y, SDL_Color color) {
    if (!font) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (!surface) {
        SDL_Log("Failed to render text: %s", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect destRect;
    destRect.x = (float)x;
    destRect.y = (float)y;
    destRect.w = (float)surface->w;
    destRect.h = (float)surface->h;

    SDL_RenderTexture(renderer, texture, NULL, &destRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void Menu::drawButton(const Button& button) {
    // Choose which texture to use based on hover state
    SDL_Texture* currentTexture = button.isHovered && button.hoverTexture
        ? button.hoverTexture
        : button.texture;

    if (currentTexture) {
        // Render the PNG texture
        SDL_RenderTexture(renderer, currentTexture, nullptr, &button.rect);
    }
    else {
        // Fallback: draw colored rectangle if texture not available
        if (button.isHovered) {
            SDL_SetRenderDrawColor(renderer,
                button.hoverColor.r,
                button.hoverColor.g,
                button.hoverColor.b,
                button.hoverColor.a);
        }
        else {
            SDL_SetRenderDrawColor(renderer,
                button.color.r,
                button.color.g,
                button.color.b,
                button.color.a);
        }
        SDL_RenderFillRect(renderer, &button.rect);

        // Draw text centered in button (fallback mode)
        if (font) {
            SDL_Color tempColor = { 255, 255, 255, 255 };
            SDL_Surface* tempSurface = TTF_RenderText_Blended(font, button.text, 0, tempColor);
            if (tempSurface) {
                int textW = tempSurface->w;
                int textH = tempSurface->h;
                SDL_DestroySurface(tempSurface);

                int textX = (int)(button.rect.x + (button.rect.w - textW) / 2);
                int textY = (int)(button.rect.y + (button.rect.h - textH) / 2);
                SDL_Color textColor = { 255, 255, 255, 255 };
                drawText(button.text, textX, textY, textColor);
            }
        }
    }
}

void Menu::draw() {
    // Create semi-transparent background for pause menu
    if (isPauseMenu) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_FRect fullScreen = { 0, 0, (float)windowWidth, (float)windowHeight };
        SDL_RenderFillRect(renderer, &fullScreen);
    }
    else {
        // Solid background for main menu
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
    }

    // Draw title
    if (font) {
        SDL_Color titleColor = { 255, 255, 255, 255 };
        const char* title;

        if (isPauseMenu) {
            if (isVictoryMenu) {
                title = "VICTORY!";
            }
            else if (isGameOverMenu) {
                title = "GAME OVER";
            }
            else {
                title = "PAUSED";
            }
        }
        else {
            title = "Shoot 'Em Up";
        }

        // Get title dimensions
        SDL_Surface* tempSurface = TTF_RenderText_Blended(font, title, 0, titleColor);
        if (tempSurface) {
            int titleW = tempSurface->w;
            SDL_DestroySurface(tempSurface);
            int titleY = (int)(playButton.rect.y - 100);
            drawText(title, (windowWidth - titleW) / 2, titleY, titleColor);
        }
    }

    if (isPauseMenu) {
        if (isVictoryMenu || isGameOverMenu) {
            drawButton(replayButton);  // Show replay for victory/game over
        }
        else {
            drawButton(resumeButton);  // Show resume for pause
        }
    }
    else {
        drawButton(playButton);  // Show play for main menu
    }

    // Draw quit button
    drawButton(quitButton);

    // Display on screen
    SDL_RenderPresent(renderer);
}
