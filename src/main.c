#include "game.h"

int main() {
  Game game;

  if (game_init(&game) != 0) {
    return 1;
  }

  while (game.running) {
    game_handle_events(&game);

    Uint64 current_time = SDL_GetTicks();
    float delta_time = (float)(current_time - game.last_frame_time) / 1000.0f;
    game.last_frame_time = current_time;

    game_update(&game, delta_time);
    game_cleanup_entities(&game);
    game_render(&game);

    Uint64 frame_time = SDL_GetTicks() - current_time;
    Uint64 delay_time_ms = 1000 / FPS;

    if (frame_time < delay_time_ms) {
      SDL_Delay(delay_time_ms - frame_time);
    }
  }
  game_cleanup(&game);
  return 0;
}
