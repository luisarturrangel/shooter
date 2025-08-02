#include "enemy.h"
#include "game.h"

void enemy_spawn(Enemy enemy[], int count, int max_enemies_cap,
                 int *alive_enemies_count_ptr) {
  int spawned_this_call = 0;

  for (int i = 0; i < max_enemies_cap && spawned_this_call < count; i++) {
    if (!enemy[i].alive) {
      enemy[i].rect.x = (float)(i * 150.0f) + 50.0f;
      enemy[i].rect.y = (SCREEN_HEIGHT / 4.0f);

      enemy[i].rect.w = 100;
      enemy[i].rect.h = 100;
      enemy[i].alive = true;

      (*alive_enemies_count_ptr)++;
      spawned_this_call++;

      SDL_Log("Spawned enemy at index %d (%.2f, %.2f). Total active: %d", i,
              enemy[i].rect.x, enemy[i].rect.y, *alive_enemies_count_ptr);
    }
  }
}

void enemy_render(Enemy enemy[], SDL_Renderer *renderer, int enemy_count) {
  for (int i = 0; i < enemy_count; i++) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x18, 0x18, 0xFF);
    if (enemy[i].alive) {
      SDL_RenderFillRect(renderer, &enemy[i].rect);
    }
  }
}
