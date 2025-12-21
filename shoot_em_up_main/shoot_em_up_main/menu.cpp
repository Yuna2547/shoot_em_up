#include "menu.h"
#include <SDL3_image/SDL_image.h>

Menu::Menu(SDL_Renderer* renderer, int winWidth, int winHeight)
    : renderer(renderer), font(nullptr), isPauseMenu(false), isVictoryMenu(false), isGameOverMenu(false), currentLevel(1), windowWidth(winWidth), windowHeight(winHeight) {
    
    if (!TTF_Init()) {
        printf("TTF_Init failed");
    }

    font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);
        if (!font) {
            font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 32);
            if (!font)
                printf("Failed to load font");
        }
    }

    //define every buttons needed with their assets 
    playButton.text = "Play";
    playButton.color = { 0, 180, 0, 255 };
    playButton.hoverColor = { 0, 220, 0, 255 };
    playButton.isHovered = false;
    playButton.texture = nullptr;
    playButton.hoverTexture = nullptr;

    resumeButton.text = "Resume";
    resumeButton.color = { 0, 180, 0, 255 };
    resumeButton.hoverColor = { 0, 220, 0, 255 };
    resumeButton.isHovered = false;
    resumeButton.texture = nullptr;
    resumeButton.hoverTexture = nullptr;

    replayButton.text = "Replay";
    replayButton.color = { 0, 150, 200, 255 };
    replayButton.hoverColor = { 0, 180, 230, 255 };
    replayButton.isHovered = false;
    replayButton.texture = nullptr;
    replayButton.hoverTexture = nullptr;

    level1Button.text = "Level 1";
    level1Button.color = { 0, 200, 100, 255 };
    level1Button.hoverColor = { 0, 230, 140, 255 };
    level1Button.isHovered = false;
    level1Button.texture = nullptr;
    level1Button.hoverTexture = nullptr;

    level2Button.text = "Level 2";
    level2Button.color = { 100, 200, 0, 255 };
    level2Button.hoverColor = { 140, 230, 0, 255 };
    level2Button.isHovered = false;
    level2Button.texture = nullptr;
    level2Button.hoverTexture = nullptr;

    quitButton.text = "Quit";
    quitButton.color = { 180, 0, 0, 255 };
    quitButton.hoverColor = { 220, 0, 0, 255 };
    quitButton.isHovered = false;
    quitButton.texture = nullptr;
    quitButton.hoverTexture = nullptr;

    loadButtonTexture(playButton, "assets/play.png", "assets/play_hover.png");
    loadButtonTexture(resumeButton, "assets/resume.png", "assets/resume_hover.png");
    loadButtonTexture(replayButton, "assets/replay.png", "assets/replay_hover.png");
	loadButtonTexture(level1Button, "assets/level1.png", "assets/level1_hover.png");
	loadButtonTexture(level2Button, "assets/level2.png", "assets/level2_hover.png");
    loadButtonTexture(quitButton, "assets/stop.png", "assets/stop_hover.png");


    updateButtonPositions();
    {
        SDL_Surface* surface = IMG_Load("assets/title.png");
        if (surface) {
            titleTexture = SDL_CreateTextureFromSurface(renderer, surface);
            if (titleTexture) {
                titleWidth = surface->w;
                titleHeight = surface->h;
            }
            SDL_DestroySurface(surface);
        }
    }
}

Menu::~Menu() {     //destory the textures when the menu is closed
    SDL_DestroyTexture(playButton.texture);
    SDL_DestroyTexture(playButton.hoverTexture);

    SDL_DestroyTexture(resumeButton.texture);
    SDL_DestroyTexture(resumeButton.hoverTexture);

    SDL_DestroyTexture(replayButton.texture);
    SDL_DestroyTexture(replayButton.hoverTexture);

    SDL_DestroyTexture(quitButton.texture);
    SDL_DestroyTexture(quitButton.hoverTexture);

    SDL_DestroyTexture(titleTexture);
    TTF_CloseFont(font);
    TTF_Quit();

}

void Menu::loadButtonTexture(Button& button, const char* normalPath, const char* hoverPath) {       //load every texture needed for the menu
    SDL_Surface* surface = IMG_Load(normalPath);
    if (surface) {
        button.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        
    }
    else
        printf("Failed to load image");

    if (hoverPath) {        //load texture for the hover button
        surface = IMG_Load(hoverPath);
        if (surface) {
            button.hoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
        }
        else
            printf("Failed to load hover image ");
    }
}

void Menu::setPauseMode(bool isPause) {     //trigger pause menu
    isPauseMenu = isPause;
    if (isPause) {
        isVictoryMenu = false;
        isGameOverMenu = false;
    }
    updateButtonPositions();
}

void Menu::setVictoryMode(bool isVictory) {      //trigger victory menu
    isVictoryMenu = isVictory;
    if (isVictory) {
        isPauseMenu = true;
        isGameOverMenu = false;
    }
    updateButtonPositions(); 
}

void Menu::setGameOverMode(bool isGameOver) {       //trigger game over menu
    isGameOverMenu = isGameOver;
    if (isGameOver) {
        isPauseMenu = true;
        isVictoryMenu = false;
    }
    updateButtonPositions();
}

void Menu::setWindowSize(int width, int height) {       //adjusts to the window's size
    windowWidth = width;
    windowHeight = height;
    updateButtonPositions();
}

void Menu::updateButtonPositions() {
    float buttonWidth = 155.0f;
    float buttonHeight = 75.0f;
    float buttonSpacing = 40.0f;

    float centerX = (windowWidth - buttonWidth) / 2.0f;

    if (isPauseMenu && isVictoryMenu) {
        float startY = (windowHeight - (buttonHeight * 3 + buttonSpacing * 2)) / 2.0f;
        replayButton.rect = { centerX, startY, buttonWidth, buttonHeight };
        level1Button.rect = { centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
        level2Button.rect = { centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
        quitButton.rect = { centerX, startY + (buttonHeight + buttonSpacing) * 2, buttonWidth, buttonHeight };
        
    }
    else {
        float startY = (windowHeight - (buttonHeight * 2 + buttonSpacing)) / 2.0f;
        playButton.rect = { centerX, startY, buttonWidth, buttonHeight };
        resumeButton.rect = { centerX, startY, buttonWidth, buttonHeight };
        replayButton.rect = { centerX, startY, buttonWidth, buttonHeight };
        level1Button.rect = { centerX, startY, buttonWidth, buttonHeight };
        level2Button.rect = { centerX, startY, buttonWidth, buttonHeight };
        quitButton.rect = { centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
    }
}

bool Menu::mouseOverButton(const Button& button, float mouseX, float mouseY) const {        //detects if the mouse passes over the button for the hover texture
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w && mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}
int Menu::handleEvents(SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX = event.button.x;
        float mouseY = event.button.y;

        if (isPauseMenu) {
            if (isVictoryMenu || isGameOverMenu) {
                if (mouseOverButton(replayButton, mouseX, mouseY)) {
                    return 1; 
                }
                // choose which level button to show 
                if (isVictoryMenu) {
                    if (currentLevel == 1 && mouseOverButton(level2Button, mouseX, mouseY)) {
                        return 2;
                    }
                    else if (currentLevel == 2 && mouseOverButton(level1Button, mouseX, mouseY)) {
                        return 3;
                    }
                }
            }
            else {
                if (mouseOverButton(resumeButton, mouseX, mouseY)) {
                    return 1;
                }
            }
            if (mouseOverButton(quitButton, mouseX, mouseY)) {
                return 4;
            }
        }
        else {
            if (mouseOverButton(playButton, mouseX, mouseY)) {
                return 1; 
            }
            if (mouseOverButton(quitButton, mouseX, mouseY)) {
                return 4;
            }
        }
    }
    return 0;
}

void Menu::update(float mouseX, float mouseY) {
    playButton.isHovered = false;
    resumeButton.isHovered = false;
    replayButton.isHovered = false;
    level1Button.isHovered = false;
    level2Button.isHovered = false;
    quitButton.isHovered = false;

	if (isPauseMenu) { //change hover states based on menu type
        if (isVictoryMenu || isGameOverMenu) {
            replayButton.isHovered = mouseOverButton(replayButton, mouseX, mouseY);
            if (isVictoryMenu) {
                if (currentLevel == 1)
                    level2Button.isHovered = mouseOverButton(level2Button, mouseX, mouseY);
                else if (currentLevel == 2)
                    level1Button.isHovered = mouseOverButton(level1Button, mouseX, mouseY);
            }
        }
        else {
            resumeButton.isHovered = mouseOverButton(resumeButton, mouseX, mouseY);
        }

        quitButton.isHovered = mouseOverButton(quitButton, mouseX, mouseY);
    }
    else {
        playButton.isHovered = mouseOverButton(playButton, mouseX, mouseY);
        quitButton.isHovered = mouseOverButton(quitButton, mouseX, mouseY);
    }
}

void Menu::drawText(const char* text, int x, int y, SDL_Color color) {          //draw every text needed
    if (!font)
        return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (!surface) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
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

void Menu::drawButton(const Button& button) {           //draws evey button for the menu
    SDL_Texture* currentTexture = button.isHovered && button.hoverTexture ? button.hoverTexture : button.texture;

    if (currentTexture)
        SDL_RenderTexture(renderer, currentTexture, nullptr, &button.rect);
    else {
        if (button.isHovered)
            SDL_SetRenderDrawColor(renderer, button.hoverColor.r, button.hoverColor.g, button.hoverColor.b, button.hoverColor.a);
        else
            SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
        SDL_RenderFillRect(renderer, &button.rect);

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

void Menu::draw() {         //draws the menu
    if (isPauseMenu) {      //sets background semi transparent if paused
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_FRect fullScreen = { 0, 0, (float)windowWidth, (float)windowHeight };
        SDL_RenderFillRect(renderer, &fullScreen);
    }
    else {              //sets background to grey not to see through
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
    }

    if (isPauseMenu) {
        if (font) {
            SDL_Color titleColor = { 255, 255, 255, 255 };
            const char* title;

            if (isVictoryMenu)
                title = "VICTORY!";
            else if (isGameOverMenu)
                title = "GAME OVER";
            else
                title = "PAUSED";

            SDL_Surface* tempSurface = TTF_RenderText_Blended(font, title, 0, titleColor);
            if (tempSurface) {
                int titleW = tempSurface->w;
                SDL_DestroySurface(tempSurface);
                int titleY = (int)(playButton.rect.y - 100);
                drawText(title, (windowWidth - titleW) / 2, titleY, titleColor);
            }
        }
    }
    else {

        if (titleTexture) {
            SDL_FRect titleRect;
            titleRect.w = 256.0f;
            titleRect.h = 128.0f;
            titleRect.x = (windowWidth - titleRect.w) / 2.0f;
            titleRect.y = (float)((int)(playButton.rect.y - 200));
            SDL_RenderTexture(renderer, titleTexture, nullptr, &titleRect);
        }
        else {
            if (font) {
                SDL_Color titleColor = { 255, 255, 255, 255 };
                const char* title = "BURGIRL";
                SDL_Surface* tempSurface = TTF_RenderText_Blended(font, title, 0, titleColor);
                if (tempSurface) {
                    int titleW = tempSurface->w;
                    SDL_DestroySurface(tempSurface);
                    int titleY = (int)(playButton.rect.y - 100);
                    drawText(title, (windowWidth - titleW) / 2, titleY, titleColor);
                }
            }
        }
    }

    if (isPauseMenu) {
        if (isVictoryMenu || isGameOverMenu) {
            drawButton(replayButton);
			if (isVictoryMenu) {    // show right buttons according to state
                if (currentLevel == 1)
                    drawButton(level2Button);  
                else if (currentLevel == 2)
                    drawButton(level1Button);  
            }
        }
        else
            drawButton(resumeButton);
    }
    else
        drawButton(playButton);

    drawButton(quitButton);
}

void Menu::setCurrentLevel(int level) {
    currentLevel = level;
    updateButtonPositions();
}
