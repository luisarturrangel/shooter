#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "core/common.h"
#include "entities/entity.h"
#include "ui/ui.h"

typedef struct {
  int fire_rate_level;
  int damage_level;
  int move_speed_level;
  BulletType bullet_type;
} PlayerUpgrades;

typedef enum { STATE_PLAYING, STATE_UPGRADES, STATE_GAME_OVER } GameState;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool running;
  Uint64 last_frame_time;
  GameState state;

  EntityManager em;
  UIManager ui;

  int score;
  int level;
  int current_xp;
  int next_level_xp;
  PlayerUpgrades upgrades;
  int upgrade_points;
} Game;

int game_init(Game *game);
void game_handle_events(Game *game);
void game_update(Game *game, float delta_time);
void game_render(Game *game);
void game_cleanup(Game *game);

void game_add_xp(Game *game, int amount);
void game_check_levelup(Game *game);

void game_apply_upgrade(Game *game, int upgrade_choice);

#endif // GAME_H
