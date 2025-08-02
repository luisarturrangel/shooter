#ifndef GAME_H_
#define GAME_H_

#include "enemy.h"
#include "player.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>

#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 1152
#define FPS 60

#define MAX_BULLETS 10
#define MAX_ENEMIES 20

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int running;
  Uint64 last_frame_time;
  Player player;
  Bullet bullets[MAX_BULLETS];
  Enemy enemy[MAX_ENEMIES];
  int alive_enemies_count;
} Game;

/**
 * @brief Initializes the game state and SDL subsystems
 * @param game Pointer to Game structure
 * @return int 0 on success, -1 on failure
 */
int game_init(Game *game);

/**
 * @brief handles SDL events (keyboard, mouse, etc.)
 * @param game Pointer to Game structure
 */
void game_handle_events(Game *game);

/**
 * @brief Updates game logic
 * @param game Pointer to Game structure
 * @param delta_time Time since last frame in seconds
 */
void game_update(Game *game, float delta_time);

/**
 * @brief Renders the current game state
 * @param game Pointer to Game structure
 */
void game_render(Game *game);

/**
 * @brief Clean up entities not used
 * @param game Pointer to Game structure
 */
void game_cleanup_entities(Game *game);

/**
 * @brief Cleans up resources and shuts down SDL subsystems
 * @param game Pointer to Game structure
 */
void game_cleanup(Game *game);

#endif // GAME_H_
