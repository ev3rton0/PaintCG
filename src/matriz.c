#include "matriz.h"
#include <math.h>
#include <string.h>

void matriz_identidade(Matriz3x3* M) {
    memset(M->m, 0, sizeof(M->m));
    M->m[0][0] = 1.0f;
    M->m[1][1] = 1.0f;
    M->m[2][2] = 1.0f;
}

void matriz_translacao(Matriz3x3* M, float tx, float ty) {
    matriz_identidade(M);
    M->m[0][2] = tx;
    M->m[1][2] = ty;
}

void matriz_escala(Matriz3x3* M, float sx, float sy) {
    matriz_identidade(M);
    M->m[0][0] = sx;
    M->m[1][1] = sy;
}

void matriz_rotacao(Matriz3x3* M, float rad) {
    matriz_identidade(M);
    float cos_theta = cos(rad);
    float sin_theta = sin(rad);
    M->m[0][0] = cos_theta;
    M->m[0][1] = -sin_theta;
    M->m[1][0] = sin_theta;
    M->m[1][1] = cos_theta;
}

void matriz_reflexao(Matriz3x3* M, int eixoX, int eixoY) {
    matriz_identidade(M);
    if (eixoX) M->m[0][0] = -1.0f;
    if (eixoY) M->m[1][1] = -1.0f;
}

void matriz_cisalhamento(Matriz3x3* M, float shx, float shy) {
    matriz_identidade(M);
    M->m[0][1] = shx;
    M->m[1][0] = shy;
}

void matriz_multiplica(Matriz3x3* R, const Matriz3x3* A, const Matriz3x3* B) {
    Matriz3x3 temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp.m[i][j] = 0.0f;
            for (int k = 0; k < 3; k++) {
                temp.m[i][j] += A->m[i][k] * B->m[k][j];
            }
        }
    }
    *R = temp;
}

void matriz_aplica_ponto(Ponto* p_out, const Ponto* p_in, const Matriz3x3* M) {
    float in_vec[3] = { p_in->x, p_in->y, 1.0f };
    float out_vec[3] = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            out_vec[i] += M->m[i][j] * in_vec[j];
        }
    }

    if (out_vec[2] != 0 && out_vec[2] != 1) {
        p_out->x = out_vec[0] / out_vec[2];
        p_out->y = out_vec[1] / out_vec[2];
    } else {
        p_out->x = out_vec[0];
        p_out->y = out_vec[1];
    }
}
