#ifndef PLAYER_H
#define PLAYER_H

#include "entities/entity.h"

/**
 * @brief Processa a entrada do teclado para a entidade do jogador.
 * @param p Ponteiro para a entidade (deve ser do tipo EK_PLAYER).
 */
void player_handle_input(Entity *p);

/**
 * @brief Atualiza a lógica específica do jogador (cooldown, limites de ecrã).
 * @param p Ponteiro para a entidade.
 * @param em Ponteiro para o EntityManager (necessário para dar spawn a balas).
 * @param delta_time Tempo decorrido.
 */
void player_update(Entity *p, EntityManager *em, float delta_time);

#endif // PLAYER_H
