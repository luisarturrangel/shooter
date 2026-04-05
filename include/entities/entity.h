#ifndef ENTITY_H
#define ENTITY_H

#include "core/common.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef enum { EK_PLAYER, EK_ENEMY, EK_BULLET } EntityKind;

typedef enum { BT_NORMAL, BT_PIERCING, BT_RICOCHET } BulletType;

typedef struct {
  EntityKind kind;
  bool active;
  SDL_FRect rect;
  float speed;
  float velocity_x;
  float velocity_y;

  union {
    struct {
      float cooldown;
      int lives;
    } player;
    struct {
      float health;
      float amplitude;
    } enemy;
    struct {
      float damage;
      BulletType type;
    } bullet;
  } data;
} Entity;

typedef struct EntityManager {
  Entity pool[MAX_ENTITIES];
  int *score_ref;
} EntityManager;

void entities_init(EntityManager *em, int *score_pointer);
Entity *entity_spawn(EntityManager *em, EntityKind kind);
void entities_update(EntityManager *em, float delta_time);
void entities_render(EntityManager *em, SDL_Renderer *renderer);

#endif // ENTITY_H
