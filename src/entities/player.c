#include "entities/player.h"
#include "core/common.h"
#include "entities/entity.h"

void player_handle_input(Entity *p) {
  if (!p)
    return;

  const bool *state = SDL_GetKeyboardState(NULL);
  float move_x = 0;
  float move_y = 0;

  if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
    move_y -= 1;
  if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
    move_y += 1;
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    move_x -= 1;
  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    move_x += 1;

  p->velocity_x = move_x * p->speed;
  p->velocity_y = move_y * p->speed;
}

void player_update(Entity *p, EntityManager *em, float delta_time) {
  if (!p)
    return;

  // Aplicar movimento
  p->rect.x += p->velocity_x * delta_time;
  p->rect.y += p->velocity_y * delta_time;

  // Limites do ecrã
  if (p->rect.x < 0)
    p->rect.x = 0;
  if (p->rect.x + p->rect.w > SCREEN_WIDTH)
    p->rect.x = SCREEN_WIDTH - p->rect.w;
  if (p->rect.y < 0)
    p->rect.y = 0;
  if (p->rect.y + p->rect.h > SCREEN_HEIGHT)
    p->rect.y = SCREEN_HEIGHT - p->rect.h;

  // Lógica de Tiro
  const bool *state = SDL_GetKeyboardState(NULL);
  if (p->data.player.cooldown > 0) {
    p->data.player.cooldown -= delta_time;
  }

  if (state[SDL_SCANCODE_SPACE] && p->data.player.cooldown <= 0) {
    // Spawn de bala usando o Manager
    Entity *b = entity_spawn(em, EK_BULLET);
    if (b) {
      b->rect.w = 8;
      b->rect.h = 16;
      b->rect.x = p->rect.x + (p->rect.w / 2) - (b->rect.w / 2);
      b->rect.y = p->rect.y - b->rect.h;
      b->speed = 600.0f;
      b->velocity_y = -b->speed;
    }
    p->data.player.cooldown = 0.15f; // Taxa de tiro
  }
}
