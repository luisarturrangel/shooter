#include "core/common.h"
#include "core/game.h"
#include <SDL3/SDL.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  Game game;

  if (game_init(&game) != 0) {
    SDL_Log("Erro ao inicializar o jogo!");
    return -1;
  }

  Uint64 last_time = SDL_GetTicks();
  const float target_dt = 1.0f / FPS;

  while (game.running) {
    Uint64 current_time = SDL_GetTicks();
    float delta_time = (current_time - last_time) / 1000.0f;
    if (delta_time > 0.1f)
      delta_time = 0.1f;
    last_time = current_time;

    game_handle_events(&game);

    game_update(&game, delta_time);

    game_render(&game);

    Uint64 frame_end_time = SDL_GetTicks();
    Uint64 elapsed = frame_end_time - current_time;
    if (elapsed < (Uint64)(target_dt * 1000.0f)) {
      SDL_Delay((Uint32)((target_dt * 1000.0f) - elapsed));
    }
  }

  game_cleanup(&game);

  return 0;
}
