#include "entities/entity.h"
#include "core/game.h" // Necessário para acessar o estado do game
#include "entities/enemy.h"
#include "entities/player.h"
#include <string.h>

void entities_init(EntityManager *em, int *score_ptr) {
  memset(em->pool, 0, sizeof(Entity) * MAX_ENTITIES);
  em->score_ref = score_ptr;
}

Entity *entity_spawn(EntityManager *em, EntityKind kind) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (!em->pool[i].active) {
      Entity *e = &em->pool[i];
      memset(e, 0, sizeof(Entity));
      e->active = true;
      e->kind = kind;
      return e;
    }
  }
  return NULL;
}

void entities_update(EntityManager *em, float delta_time) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity *ent = &em->pool[i];
    if (!ent->active)
      continue;

    switch (ent->kind) {
    case EK_PLAYER:
      player_handle_input(ent);
      player_update(ent, em, delta_time);
      break;

    case EK_ENEMY:
      enemy_update(ent, delta_time);
      break;

    case EK_BULLET:
      ent->rect.x += ent->velocity_x * delta_time;
      ent->rect.y += ent->velocity_y * delta_time;

      if (ent->rect.y + ent->rect.h < -50)
        ent->active = false;

      // Colisão Bala -> Inimigo
      for (int j = 0; j < MAX_ENTITIES; j++) {
        Entity *target = &em->pool[j];
        if (target->active && target->kind == EK_ENEMY) {
          if (SDL_HasRectIntersectionFloat(&ent->rect, &target->rect)) {

            // Lógica de Pontuação Dinâmica baseada no "nível"
            // Se você tiver uma referência ao game/level:
            // int points = 10 * current_level;
            if (em->score_ref)
              (*em->score_ref) += 10;

            target->active = false;

            if (ent->data.bullet.type != BT_PIERCING) {
              ent->active = false;
              break;
            }
          }
        }
      }
      break;
    }
  }
}

void entities_render(EntityManager *em, SDL_Renderer *renderer) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity *ent = &em->pool[i];
    if (!ent->active)
      continue;

    switch (ent->kind) {
    case EK_PLAYER:
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      break;
    case EK_ENEMY:
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      break;
    case EK_BULLET:
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      break;
    }
    SDL_RenderFillRect(renderer, &ent->rect);
  }
}
