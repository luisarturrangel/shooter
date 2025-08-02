#include "game.h"
#include "player.h"

void player_shoot(Game *game);

int game_init(Game *game) {

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return -1;
  }

  if (!SDL_CreateWindowAndRenderer("Hello", SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                                   &game->window, &game->renderer)) {
    SDL_Log("Window creation failed: %s", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  game->running = true;
  game->last_frame_time = SDL_GetTicks();

  // player
  player_init(&game->player);

  // bullets
  for (int i = 0; i < MAX_BULLETS; i++) {
    game->bullets[i].active = false;
  }

  // Enemy
  game->alive_enemies_count = 0;
  int enemy_count = 5;
  enemy_spawn(game->enemy, enemy_count, MAX_ENEMIES,
              &game->alive_enemies_count);

  return 0;
}

void game_handle_events(Game *game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    switch (event.type) {

    case SDL_EVENT_QUIT:
      game->running = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      switch (event.key.key) {
      case SDLK_ESCAPE:
        SDL_Log("Escape key pressed! Quitting.");
        game->running = false;
        break;
      default:
        break;
      }
    }
  }
}

void game_update(Game *game, float delta_time) {

  // Player movement & shoot
  player_handle_input(&game->player, game->bullets, MAX_BULLETS, delta_time);

  player_update(&game->player, delta_time);

  // bullet update
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (game->bullets[i].active) {
      game->bullets[i].rect.y -= game->bullets[i].speed * delta_time;

      if (game->bullets[i].rect.y + game->bullets[i].rect.h < 0) {
        game->bullets[i].active = false;
      }

      for (int j = 0; j < MAX_ENEMIES; j++) {
        if (game->enemy[j].alive) {
          if (SDL_HasRectIntersectionFloat(&game->bullets[i].rect,
                                           &game->enemy[j].rect)) {
            SDL_Log("Enemy hit [%d]", j);
            game->enemy[j].alive = false;
            break;
          }
        }
      }
      // is enemy alive
    }
  }
}

void game_render(Game *game) {

  // background
  SDL_SetRenderDrawColor(game->renderer, 0x18, 0x18, 0x18, 0xFF);
  SDL_RenderClear(game->renderer);

  // player
  player_render(&game->player, game->renderer);
  // bullets
  SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0x00, 0xFF);
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (game->bullets[i].active) {
      SDL_RenderFillRect(game->renderer, &game->bullets[i].rect);
    }
  }

  // enemy
  enemy_render(game->enemy, game->renderer, game->alive_enemies_count);

  SDL_RenderPresent(game->renderer);
}

void game_cleanup_entities(Game *game) {
  int write_index = 0;
  for (int read_index = 0; read_index < MAX_ENEMIES; read_index++) {
    if (game->enemy[read_index].alive) {
      if (read_index != write_index) {
        game->enemy[write_index] = game->enemy[read_index];
      }
      write_index++;
    }
  }
  game->alive_enemies_count = write_index;
}

void game_cleanup(Game *game) {
  if (game->renderer) {
    SDL_DestroyRenderer(game->renderer);
  }
  if (game->window) {
    SDL_DestroyWindow(game->window);
  }
  SDL_Quit();
}
