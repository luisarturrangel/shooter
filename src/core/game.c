#include "core/game.h"
#include "core/common.h"
#include "entities/entity.h"
#include "ui/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para calcular recompensa offline
void game_calculate_offline_progress(Game *game) {
  time_t now = time(NULL);
  time_t last_save = (time_t)SDL_GetNumberProperty(
      SDL_GetGlobalProperties(), "last_save_time", (Sint64)now);
  double diff_seconds = difftime(now, last_save);

  if (diff_seconds > 60) {
    float dps_estimado =
        (float)(game->upgrades.damage_level * game->upgrades.fire_rate_level) *
        0.2f;
    int reward = (int)(dps_estimado * diff_seconds * 0.7f);

    if (reward > 0) {
      game->score += reward;
      game->current_xp += reward;
      SDL_Log("Offline: Coletados %d recursos.", reward);
    }
  }
}

int game_init(Game *game) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    return -1;

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                                   &game->window, &game->renderer))
    return -1;

  entities_init(&game->em, &game->score);
  ui_init(&game->ui,
          "assets/fonts/DepartureMono/DepartureMonoNerdFont-Regular.otf", 22);

  game->running = true;
  game->last_frame_time = SDL_GetTicks();
  game->state = STATE_PLAYING;

  game->score = 0;
  game->level = 1;
  game->current_xp = 0;
  game->next_level_xp = 100;
  game->upgrade_points = 0;

  game->upgrades.fire_rate_level = 1;
  game->upgrades.damage_level = 1;
  game->upgrades.move_speed_level = 1;
  game->upgrades.bullet_type = BT_NORMAL;

  game_calculate_offline_progress(game);

  Entity *p = entity_spawn(&game->em, EK_PLAYER);
  if (p) {
    p->rect =
        (SDL_FRect){(SCREEN_WIDTH / 2.0f) - 40, SCREEN_HEIGHT - 120, 80, 80};
    p->speed = 350.0f;
    p->data.player.cooldown = 0.0f;
  }

  return 0;
}

void game_handle_events(Game *game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      SDL_SetNumberProperty(SDL_GetGlobalProperties(), "last_save_time",
                            (Sint64)time(NULL));
      game->running = false;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
      if (event.key.key == SDLK_ESCAPE || event.key.key == SDLK_U) {
        if (game->state == STATE_PLAYING)
          game->state = STATE_UPGRADES;
        else if (game->state == STATE_UPGRADES)
          game->state = STATE_PLAYING;
      }

      // Lógica de compra no menu de upgrades usando Score (Recursos)
      if (game->state == STATE_UPGRADES) {
        // Cálculo de preço linear/escalonável
        int damage_cost = 50 + (game->upgrades.damage_level * 25);
        int fire_rate_cost = 50 + (game->upgrades.fire_rate_level * 30);

        if (event.key.key == SDLK_1 && game->score >= damage_cost) {
          game->score -= damage_cost;
          game->upgrades.damage_level++;
          SDL_Log("Upgrade: Dano Nivel %d comprado!",
                  game->upgrades.damage_level);
        }
        if (event.key.key == SDLK_2 && game->score >= fire_rate_cost) {
          game->score -= fire_rate_cost;
          game->upgrades.fire_rate_level++;
          SDL_Log("Upgrade: Cadencia Nivel %d comprada!",
                  game->upgrades.fire_rate_level);
        }
      }
    }
  }
}

void game_spawn_bullet(Game *game, float start_x, float start_y) {
  Entity *b = entity_spawn(&game->em, EK_BULLET);
  if (b) {
    b->rect = (SDL_FRect){start_x - 5, start_y - 20, 10, 25};
    b->velocity_x = 0;
    b->velocity_y = -800.0f;
    b->data.bullet.type = game->upgrades.bullet_type;
  }
}

void game_update(Game *game, float delta_time) {
  if (!game->running)
    return;

  game_handle_events(game);

  if (game->state != STATE_PLAYING)
    return;

  static bool space_was_held = false; // Controle rigoroso de semi-automático
  static float respawn_timer = 0.0f;

  entities_update(&game->em, delta_time);

  // 1. Lógica do Jogador
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity *e = &game->em.pool[i];
    if (e->active && e->kind == EK_PLAYER) {
      const bool *keys = SDL_GetKeyboardState(NULL);

      float step = e->speed * delta_time;
      if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        e->rect.x -= step;
      if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        e->rect.x += step;

      if (e->rect.x < 0)
        e->rect.x = 0;
      if (e->rect.x + e->rect.w > SCREEN_WIDTH)
        e->rect.x = SCREEN_WIDTH - e->rect.w;

      if (e->data.player.cooldown > 0)
        e->data.player.cooldown -= delta_time;

      // Lógica de tiro com trava de repetição
      bool space_is_down = keys[SDL_SCANCODE_SPACE];

      if (game->upgrades.fire_rate_level < 10) {
        // Modo Semi-Automático: Só atira se pressionar (down) e não estava
        // segurando antes
        if (space_is_down && !space_was_held && e->data.player.cooldown <= 0) {
          game_spawn_bullet(game, e->rect.x + e->rect.w / 2, e->rect.y);
          e->data.player.cooldown =
              0.5f / (float)game->upgrades.fire_rate_level;
        }
      } else {
        // Modo Automático: Desbloqueia em níveis altos de upgrade
        if (space_is_down && e->data.player.cooldown <= 0) {
          game_spawn_bullet(game, e->rect.x + e->rect.w / 2, e->rect.y);
          e->data.player.cooldown =
              0.5f / (float)game->upgrades.fire_rate_level;
        }
      }
      space_was_held = space_is_down;
    }
  }

  if (game->current_xp >= game->next_level_xp) {
    game->level++;
    game->current_xp = 0;
    game->next_level_xp = 100 + (game->level * 150);
    SDL_Log("Evolucao Cosmica! Nivel: %d", game->level);
  }

  int target_count = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (game->em.pool[i].active && game->em.pool[i].kind == EK_ENEMY)
      target_count++;
  }

  if (target_count == 0) {
    respawn_timer += delta_time;
    float current_respawn_delay = 1.5f;

    if (respawn_timer >= current_respawn_delay) {
      Entity *target = entity_spawn(&game->em, EK_ENEMY);
      if (target) {
        float size = 120.0f + (game->level * 5);
        target->rect =
            (SDL_FRect){(SCREEN_WIDTH - size) / 2.0f, 60, size, size};
        target->speed = 0;
        // Inimigo com muito mais vida para não morrer com 1 tiro
        // Multiplicamos por 10 para garantir resistência
        target->data.enemy.health = 10 + (game->level * 10);
      }
      respawn_timer = 0;
    }
  }
}

void game_render(Game *game) {
  SDL_SetRenderDrawColor(game->renderer, 10, 10, 25, 255);
  SDL_RenderClear(game->renderer);

  entities_render(&game->em, game->renderer);

  char res[64], lvl[64], costs[128];
  snprintf(res, 64, "RECURSOS: %d", game->score);
  snprintf(lvl, 64, "ESCALA: %d (XP: %d/%d)", game->level, game->current_xp,
           game->next_level_xp);

  ui_draw_text(&game->ui, game->renderer, res, 20, 20, game->ui.color_white);
  ui_draw_text(&game->ui, game->renderer, lvl, 20, 50, game->ui.color_green);

  if (game->state == STATE_UPGRADES) {
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
    SDL_FRect overlay = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    SDL_RenderFillRect(game->renderer, &overlay);

    int damage_cost = 50 + (game->upgrades.damage_level * 25);
    int fire_rate_cost = 50 + (game->upgrades.fire_rate_level * 30);

    ui_draw_text(&game->ui, game->renderer, "MENU DE EVOLUCAO",
                 SCREEN_WIDTH / 2 - 120, 150, game->ui.color_white);

    snprintf(costs, 128, "[1] Dano (Nivel %d) - Custo: %d",
             game->upgrades.damage_level, damage_cost);
    ui_draw_text(&game->ui, game->renderer, costs, SCREEN_WIDTH / 2 - 180, 220,
                 game->ui.color_white);

    snprintf(costs, 128, "[2] Cadencia (Nivel %d) - Custo: %d",
             game->upgrades.fire_rate_level, fire_rate_cost);
    ui_draw_text(&game->ui, game->renderer, costs, SCREEN_WIDTH / 2 - 180, 260,
                 game->ui.color_white);

    ui_draw_text(&game->ui, game->renderer, "Pressione ESC para voltar",
                 SCREEN_WIDTH / 2 - 140, 400, game->ui.color_green);
  }

  SDL_RenderPresent(game->renderer);
}

void game_cleanup(Game *game) {
  ui_cleanup(&game->ui);
  if (game->renderer)
    SDL_DestroyRenderer(game->renderer);
  if (game->window)
    SDL_DestroyWindow(game->window);
  SDL_Quit();
}
