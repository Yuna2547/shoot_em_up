#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <SDL3/SDL.h>

typedef struct {
    SDL_FRect rect;
    float speed;
    int screen_width;
    int screen_height;
} Entity;

// Entities' creation
void Entity_Init(Entity* entity, float x, float y, float w, float h, float speed);

// Update position 
void Entity_Update(Entity* entity, const bool* keys, float dt);

// Define screen's limits
void Entity_SetScreenBounds(Entity* entity, int width, int height);

// Draw entity
void Entity_Draw(Entity* entity, SDL_Renderer* renderer);

#endif // ENTITY_H

