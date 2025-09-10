#ifndef MATRIZ_H
#define MATRIZ_H

#include "primitivas.h"

typedef struct {
    float m[3][3];
} Matriz3x3;

void matriz_identidade(Matriz3x3* M);
void matriz_translacao(Matriz3x3* M, float tx, float ty);
void matriz_escala(Matriz3x3* M, float sx, float sy);
void matriz_rotacao(Matriz3x3* M, float rad);
void matriz_reflexao(Matriz3x3* M, int eixoX, int eixoY);
void matriz_cisalhamento(Matriz3x3* M, float shx, float shy);
void matriz_multiplica(Matriz3x3* R, const Matriz3x3* A, const Matriz3x3* B);
void matriz_aplica_ponto(Ponto* p_out, const Ponto* p_in, const Matriz3x3* M);

#endif
