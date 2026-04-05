#ifndef ENEMY_H
#define ENEMY_H

#include "entities/entity.h"

/**
 * @brief Comportamento de IA simples para os inimigos.
 * @param e Ponteiro para a entidade inimiga.
 * @param delta_time Tempo decorrido.
 */
void enemy_update(Entity *e, float delta_time);

#endif // ENEMY_H
