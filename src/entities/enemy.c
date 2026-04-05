#include "entities/enemy.h"
#include "core/common.h"

void enemy_update(Entity *e, float delta_time) {
  if (!e)
    return;

  // Movimento vertical constante
  e->rect.y += e->speed * delta_time;

  // Se sair do ecrã, desativar para o Manager reutilizar o slot
  if (e->rect.y > SCREEN_HEIGHT) {
    e->active = false;
  }

  // Aqui poderias adicionar movimento em ziguezague usando e->data.enemy
  // Exemplo: e->rect.x += sinf(SDL_GetTicks() * 0.005f) * 2.0f;
}
