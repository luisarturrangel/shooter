#include "game.h"
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
  float player_x = (SCREEN_WIDTH - game->player.rect.w) / 2.0f;
  float player_y = (SCREEN_HEIGHT - game->player.rect.h) / 2.0f;

  game->player =
      (Player){.rect = {.x = player_x, .y = player_y, .w = 100, .h = 100},
               .speed = 250.0f,
               .cooldown_timer = 0.0f};

  // bullets
  for (int i = 0; i < MAX_BULLETS; i++) {
    game->bullets[i].active = false;
  }

  // Enemy
  game->enemy.rect.x = (SCREEN_WIDTH - game->enemy.rect.w) / 2.0f;
  game->enemy.rect.y = (SCREEN_HEIGHT - game->enemy.rect.h) / 3.0f;
  game->enemy.rect.w = 100;
  game->enemy.rect.h = 100;
  game->enemy.alive = true;

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

void player_shoot(Game *game) {
  if (game->player.cooldown_timer <= 0.0f) {
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (!game->bullets[i].active) {
        game->bullets[i].active = true;

        game->bullets[i].rect.x =
            game->player.rect.x + (game->player.rect.w / 2) - 5;
        game->bullets[i].rect.y = game->player.rect.y;
        game->bullets[i].rect.w = 10;
        game->bullets[i].rect.h = 20;
        game->bullets[i].speed = 800.0f;

        game->player.cooldown_timer = 0.1f;

        SDL_Log("Bullet fired");
        return;
      }
    }
  }
}

void game_update(Game *game, float delta_time) {

  // Player movement
  const bool *keyboard_state = SDL_GetKeyboardState(NULL);

  if (keyboard_state[SDL_SCANCODE_W]) {
    game->player.rect.y -= game->player.speed * delta_time;
  }
  if (keyboard_state[SDL_SCANCODE_S]) {
    game->player.rect.y += game->player.speed * delta_time;
  }
  if (keyboard_state[SDL_SCANCODE_A]) {
    game->player.rect.x -= game->player.speed * delta_time;
  }
  if (keyboard_state[SDL_SCANCODE_D]) {
    game->player.rect.x += game->player.speed * delta_time;
  }

  // clamp player to screen
  if (game->player.rect.x < 0) {
    game->player.rect.x = 0;
  }
  if (game->player.rect.x + game->player.rect.w > SCREEN_WIDTH) {
    game->player.rect.x = SCREEN_WIDTH - game->player.rect.w;
  }
  if (game->player.rect.y < 0) {
    game->player.rect.y = 0;
  }
  if (game->player.rect.y + game->player.rect.h > SCREEN_HEIGHT) {
    game->player.rect.y = SCREEN_HEIGHT - game->player.rect.h;
  }

  if (game->enemy.rect.x < 0) {
    game->enemy.rect.x = 0;
  }
  if (game->enemy.rect.x + game->enemy.rect.w > SCREEN_WIDTH) {
    game->enemy.rect.x = SCREEN_WIDTH - game->enemy.rect.w;
  }
  if (game->enemy.rect.y < 0) {
    game->enemy.rect.y = 0;
  }
  if (game->enemy.rect.y + game->enemy.rect.h > SCREEN_HEIGHT) {
    game->enemy.rect.y = SCREEN_HEIGHT - game->enemy.rect.h;
  }

  // timer reset
  if (game->player.cooldown_timer > 0) {
    game->player.cooldown_timer -= delta_time;
    if (game->player.cooldown_timer < 0) {
      game->player.cooldown_timer = 0;
    }
  }

  // shoot
  if (keyboard_state[SDL_SCANCODE_SPACE]) {
    player_shoot(game);
  }

  // bullet update
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (game->bullets[i].active) {
      game->bullets[i].rect.y -= game->bullets[i].speed * delta_time;

      if (game->bullets[i].rect.y + game->bullets[i].rect.h < 0) {
        game->bullets[i].active = false;
      }

      // is enemy alive
      if (SDL_HasRectIntersectionFloat(&game->bullets[i].rect,
                                       &game->enemy.rect)) {
        SDL_Log("Enemy hit");
        game->enemy.alive = false;
      }
    }
  }
}

void game_render(Game *game) {

  // background
  SDL_SetRenderDrawColor(game->renderer, 0x18, 0x18, 0x18, 0xFF);
  SDL_RenderClear(game->renderer);

  // player
  SDL_SetRenderDrawColor(game->renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderFillRect(game->renderer, &game->player.rect);

  // bullets
  SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0x00, 0xFF);
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (game->bullets[i].active) {
      SDL_RenderFillRect(game->renderer, &game->bullets[i].rect);
    }
  }

  // enemy
  SDL_SetRenderDrawColor(game->renderer, 0xFF, 0x18, 0x18, 0xFF);
  if (game->enemy.alive) {
    SDL_RenderFillRect(game->renderer, &game->enemy.rect);
  }

  SDL_RenderPresent(game->renderer);
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
