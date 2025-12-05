#include "entity.h"

Entity::Entity(float x, float y, float w, float h, float speed)
    : speed(speed), screen_width(800), screen_height(600) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void Entity::update(const bool* keys, float dt) {
    // Vertical controls
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
        rect.y -= speed * dt;
    }
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
        rect.y += speed * dt;
    }

    // Horizontal controls
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        rect.x -= speed * dt;
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        rect.x += speed * dt;
    }

    // Screen limits
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x + rect.w > screen_width) rect.x = screen_width - rect.w;
    if (rect.y + rect.h > screen_height) rect.y = screen_height - rect.h;
}

void Entity::setScreenBounds(int width, int height) {
    screen_width = width;
    screen_height = height;
}

void Entity::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
