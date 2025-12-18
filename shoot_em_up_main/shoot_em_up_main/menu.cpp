#include "menu.h"
#include <SDL3_image/SDL_image.h>

Menu::Menu(SDL_Renderer* renderer, int winWidth, int winHeight)
    : renderer(renderer), font(nullptr), isPauseMenu(false), isVictoryMenu(false), isGameOverMenu(false), windowWidth(winWidth), windowHeight(winHeight) {

    
    if (TTF_Init() == -1) // Initialize SDL_ttf
        SDL_Log("TTF_Init Error: ", SDL_GetError());
    else {
        font = TTF_OpenFont("arial.ttf", 32);
        if (!font) {
            font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 32);
            if (!font) {
                font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 32);
                if (!font) 
                    SDL_Log("Failed to load font: ", SDL_GetError());
            }
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


    updateButtonPositions();
}

Menu::~Menu() {     //destory the textures when the menu is closed
    if (playButton.texture) 
        SDL_DestroyTexture(playButton.texture);
    if (playButton.hoverTexture) 
        SDL_DestroyTexture(playButton.hoverTexture);

    if (resumeButton.texture) 
        SDL_DestroyTexture(resumeButton.texture);
    if (resumeButton.hoverTexture) 
        SDL_DestroyTexture(resumeButton.hoverTexture);

    if (replayButton.texture) 
        SDL_DestroyTexture(replayButton.texture);
    if (replayButton.hoverTexture)
        SDL_DestroyTexture(replayButton.hoverTexture);

    if (quitButton.texture) 
        SDL_DestroyTexture(quitButton.texture);
    if (quitButton.hoverTexture) 
        SDL_DestroyTexture(quitButton.hoverTexture);


    if (font) 
        TTF_CloseFont(font);
    TTF_Quit();

}

void Menu::loadButtonTexture(Button& button, const char* normalPath, const char* hoverPath) {       //load every texture needed for the menu
    SDL_Surface* surface = IMG_Load(normalPath);
    if (surface) {
        button.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        if (!button.texture) 
            SDL_Log("Failed to create texture from ", normalPath," : ", SDL_GetError());
        else 
            SDL_Log("Successfully loaded texture: ", normalPath);
    }
    else 
        SDL_Log("Failed to load image ", normalPath," : ", SDL_GetError());

    if (hoverPath) {        //load texture for the hover button
        surface = IMG_Load(hoverPath);
        if (surface) {
            button.hoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            if (!button.hoverTexture) 
                SDL_Log("Failed to create hover texture from ", hoverPath, " : ", SDL_GetError());
            else 
                SDL_Log("Successfully loaded hover texture: ", hoverPath);
        }
        else 
            SDL_Log("Failed to load hover image ", hoverPath, " : ", SDL_GetError());
    }
}


void Menu::setPauseMode(bool isPause) {     //trigger pause menu
    isPauseMenu = isPause;
    if (isPause) {
        isVictoryMenu = false;
        isGameOverMenu = false;
    }
}

void Menu::setVictoryMode(bool isVictory){      //trigger victory menu
    isVictoryMenu = isVictory;
    if (isVictory) {
        isPauseMenu = true;  
        isGameOverMenu = false;
    }
}

void Menu::setGameOverMode(bool isGameOver) {       //trigger game over menu
    isGameOverMenu = isGameOver;
    if (isGameOver) {
        isPauseMenu = true; 
        isVictoryMenu = false;
    }
}

void Menu::setWindowSize(int width, int height) {       //adjusts to the window's size
    windowWidth = width;
    windowHeight = height;
    updateButtonPositions();
}

void Menu::updateButtonPositions() {        //adjust the button's position
    float buttonWidth = 155.0f;
    float buttonHeight = 75.0f;
    float buttonSpacing = 40.0f;

    float centerX = (windowWidth - buttonWidth) / 2.0f;
    float startY = (windowHeight - (buttonHeight * 2 + buttonSpacing)) / 2.0f;

    playButton.rect = { centerX, startY, buttonWidth, buttonHeight };
    resumeButton.rect = { centerX, startY, buttonWidth, buttonHeight };
    replayButton.rect = { centerX, startY, buttonWidth, buttonHeight }; 
    quitButton.rect = { centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
}

bool Menu::mouseOverButton(const Button& button, float mouseX, float mouseY) const {        //detects if the mouse passes over the button for the hover texture
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w && mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}

int Menu::handleEvents(SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX = event.button.x;
        float mouseY = event.button.y;

        if (isPauseMenu) {
            if (isVictoryMenu || isGameOverMenu) {      //if victory of game over
                if (mouseOverButton(replayButton, mouseX, mouseY))      //toggle replay button for the first button
                    return 1; 
            }
            else {          //if paused menu
                if (mouseOverButton(resumeButton, mouseX, mouseY))      //toggle resume button 
                    return 1; 
            }
        }
        else {          //else means start menu
            if (mouseOverButton(playButton, mouseX, mouseY))        //toggle play button
                return 1; 
        }

        if (mouseOverButton(quitButton, mouseX, mouseY))        //toggle quit button everytime for the second one
            return 2; 
    }
    return 0; 
}

void Menu::update(float mouseX, float mouseY) {         //updates mostly for hovered buttons and change of texture
    playButton.isHovered = false;
    resumeButton.isHovered = false;
    replayButton.isHovered = false;
    quitButton.isHovered = false;

    if (isPauseMenu) {
        if (isVictoryMenu || isGameOverMenu) 
            replayButton.isHovered = mouseOverButton(replayButton, mouseX, mouseY);     //hovered texture for replay button
        else 
            resumeButton.isHovered = mouseOverButton(resumeButton, mouseX, mouseY);     //hovered texture for resume button
    }
    else 
        playButton.isHovered = mouseOverButton(playButton, mouseX, mouseY);         //hovered texture for play button
    quitButton.isHovered = mouseOverButton(quitButton, mouseX, mouseY);         //hovered texture for guit button
}

void Menu::drawText(const char* text, int x, int y, SDL_Color color) {          //draw every text needed
    if (!font) 
        return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (!surface) {
        SDL_Log("Failed to render text: ", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: ", SDL_GetError());
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

    if (font) {         //draws titles according to each menu
        SDL_Color titleColor = { 255, 255, 255, 255 };
        const char* title;

        if (isPauseMenu) {
            if (isVictoryMenu) 
                title = "VICTORY!";
            else if (isGameOverMenu) 
                title = "GAME OVER";
            else 
                title = "PAUSED";
        }
        else 
            title = "Shoot 'Em Up";

        SDL_Surface* tempSurface = TTF_RenderText_Blended(font, title, 0, titleColor);
        if (tempSurface) {
            int titleW = tempSurface->w;
            SDL_DestroySurface(tempSurface);
            int titleY = (int)(playButton.rect.y - 100);
            drawText(title, (windowWidth - titleW) / 2, titleY, titleColor);
        }
    }

    if (isPauseMenu) {
        if (isVictoryMenu || isGameOverMenu) 
            drawButton(replayButton);
        else 
            drawButton(resumeButton); 
    }
    else 
        drawButton(playButton); 

    drawButton(quitButton);
}
