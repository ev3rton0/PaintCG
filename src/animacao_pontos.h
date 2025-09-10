#ifndef ANIMACAO_PONTOS_H
#define ANIMACAO_PONTOS_H

#include "gerenciador_objetos.h"


void animacao_pontos_iniciar(EstadoObjetos* estado);


void animacao_pontos_parar(EstadoObjetos* estado);


void animacao_pontos_atualizar(EstadoObjetos* estado);


int is_animacao_pontos_ativa();

#endif
