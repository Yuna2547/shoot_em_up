#include "entity.h"
#include "sprite.h"   // make sure the header name matches your actual file

void Entity_Init(Entity* entity, float x, float y, float w, float h, float speed, SDL_Renderer* renderer) {
    entity->rect.x = x;
    entity->rect.y = y;
    entity->rect.w = w;
    entity->rect.h = h;
    entity->speed = speed;
    entity->screen_width = 800;
    entity->screen_height = 600;

    // Load your PNG sprite instead of BMP
    entity->sprite = Sprite_Load(renderer, "player.png");
}

void Entity_Update(Entity* entity, const bool* keys, float dt) {
    // Vertical controls
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
        entity->rect.y -= entity->speed * dt;
    }
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
        entity->rect.y += entity->speed * dt;
    }

    // Horizontal controls
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        entity->rect.x -= entity->speed * dt;
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        entity->rect.x += entity->speed * dt;
    }

    // Screen's limits
    if (entity->rect.x < 0)
        entity->rect.x = 0;
    if (entity->rect.y < 0)
        entity->rect.y = 0;
    if (entity->rect.x + entity->rect.w > entity->screen_width)
        entity->rect.x = entity->screen_width - entity->rect.w;
    if (entity->rect.y + entity->rect.h > entity->screen_height)
        entity->rect.y = entity->screen_height - entity->rect.h;
}

void Entity_Draw(Entity* entity, SDL_Renderer* renderer) {
    if (entity->sprite) {
        Sprite_Draw(entity->sprite, renderer, &entity->rect);
    }
}

void Entity_SetScreenBounds(Entity* entity, int width, int height) {
    entity->screen_width = width;
    entity->screen_height = height;
}

void Entity_Destroy(Entity* entity) {
    if (entity->sprite) {
        Sprite_Destroy(entity->sprite);
        entity->sprite = NULL;
    }
}
