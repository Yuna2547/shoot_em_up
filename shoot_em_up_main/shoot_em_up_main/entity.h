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

// Initialise une entité
void Entity_Init(Entity* entity, float x, float y, float w, float h, float speed);

// Met à jour la position selon les touches pressées
void Entity_Update(Entity* entity, const bool* keys, float dt);

// Définit les limites de l'écran
void Entity_SetScreenBounds(Entity* entity, int width, int height);

// Dessine l'entité
void Entity_Draw(Entity* entity, SDL_Renderer* renderer);

#endif // ENTITY_H

