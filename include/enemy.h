#ifndef ENEMY_H
#define ENEMY_H

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>

typedef struct {
  float speed;
  SDL_FRect rect;
  bool alive;
} Enemy;

void enemy_spawn(Enemy enemy[], int count, int max_enemies_cap,
                 int *enemy_count_ptr);

void enemy_render(Enemy *enemy, SDL_Renderer *renderer, int enemy_count);

void enemy_update_path(Enemy *enemy, double delta_time);

#endif // !ENEMY_H
