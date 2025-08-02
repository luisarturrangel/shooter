#include "player.h"
#include "game.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>

void player_init(Player *player) {
  player->rect.w = 100;
  player->rect.h = 100;
  player->rect.x = (SCREEN_WIDTH - player->rect.w) / 2.0f;
  player->rect.y = (SCREEN_HEIGHT - player->rect.h) / 1.5f;

  player->speed = 400.0f;
  player->cooldown_timer = 0.0f;
}

static void player_shot_logic(Player *player, Bullet bullets[],
                              int max_bullets) {
  if (player->cooldown_timer <= 0.0f) {
    for (int i = 0; i < max_bullets; i++) {
      if (!bullets[i].active) {
        bullets[i].active = true;
        bullets[i].rect.x = player->rect.x + (player->rect.w / 2) - 5;
        bullets[i].rect.y = player->rect.y;
        bullets[i].rect.w = 10;
        bullets[i].rect.h = 20;
        bullets[i].speed = 2000.0f;

        player->cooldown_timer = 0.1f;

        SDL_Log("bullet fired");
        return;
      }
    }
  }
}

void player_handle_input(Player *player, Bullet *bullets, int max_bullets,
                         float delta_time) {
  const bool *keyboard_state = SDL_GetKeyboardState(NULL);

  if (keyboard_state[SDL_SCANCODE_W]) {
    player->rect.y -= player->speed * delta_time;
  }
  if (keyboard_state[SDL_SCANCODE_S]) {
    player->rect.y += player->speed * delta_time;
  }
  if (keyboard_state[SDL_SCANCODE_A]) {
    player->rect.x -= player->speed * delta_time;
  }
  if (keyboard_state[SDL_SCANCODE_D]) {
    player->rect.x += player->speed * delta_time;
  }

  if (keyboard_state[SDL_SCANCODE_SPACE]) {
    player_shot_logic(player, bullets, max_bullets);
  }
}

void player_update(Player *player, float delta_time) {
  // clamp player to screen
  if (player->rect.x < 0) {
    player->rect.x = 0;
  }
  if (player->rect.x + player->rect.w > SCREEN_WIDTH) {
    player->rect.x = SCREEN_WIDTH - player->rect.w;
  }
  if (player->rect.y < 0) {
    player->rect.y = 0;
  }
  if (player->rect.y + player->rect.h > SCREEN_HEIGHT) {
    player->rect.y = SCREEN_HEIGHT - player->rect.h;
  }

  // timer reset
  if (player->cooldown_timer > 0) {
    player->cooldown_timer -= delta_time;
    if (player->cooldown_timer < 0) {
      player->cooldown_timer = 0;
    }
  }
}

void player_render(Player *player, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &player->rect);
}
