#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

typedef struct {
  SDL_FRect rect;
  float speed;
  float cooldown_timer;
} Player;

typedef struct {
  SDL_FRect rect;
  float speed;
  bool active;
} Bullet;

void player_init(Player *player);

void player_handle_input(Player *player, Bullet bullets[], int max_bullets,
                         float delta_time);

void player_update(Player *player, float delta_time);

void player_render(Player *player, SDL_Renderer *renderer);

#endif // !PLAYER_H
