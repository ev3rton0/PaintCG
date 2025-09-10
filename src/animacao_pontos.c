#include "animacao_pontos.h"
#include "ui.h"
#include <stdlib.h>
#include <stdio.h>

static int animacao_ativa = 0;
static Ponto* backup_pontos = NULL;
static int num_backup_pontos = 0;
static float* velocidades = NULL;

void animacao_pontos_iniciar(EstadoObjetos* estado) {
    if (animacao_ativa) return;
    if (estado->numPontos == 0) {
        printf("Nao ha pontos na tela para animar!\n");
        return;
    }

    printf("Iniciando animacao de pontos...\n");
    animacao_ativa = 1;
    num_backup_pontos = estado->numPontos;


    backup_pontos = (Ponto*)malloc(num_backup_pontos * sizeof(Ponto));
    velocidades = (float*)malloc(num_backup_pontos * sizeof(float));

    if (!backup_pontos || !velocidades) {
        printf("Erro de alocacao de memoria para animacao!\n");
        animacao_ativa = 0;
        return;
    }


    for (int i = 0; i < num_backup_pontos; i++) {
        backup_pontos[i] = estado->pontos[i];
        velocidades[i] = 1.0f + ((float)rand() / RAND_MAX) * 2.0f;
    }
}

void animacao_pontos_parar(EstadoObjetos* estado) {
    if (!animacao_ativa) return;

    printf("Parando animacao de pontos.\n");
    animacao_ativa = 0;


    if (estado->numPontos == num_backup_pontos) {
        for (int i = 0; i < num_backup_pontos; i++) {
            estado->pontos[i] = backup_pontos[i];
        }
    }


    free(backup_pontos);
    free(velocidades);
    backup_pontos = NULL;
    velocidades = NULL;
    num_backup_pontos = 0;
}

void animacao_pontos_atualizar(EstadoObjetos* estado) {
    if (!animacao_ativa) return;


    if (estado->numPontos != num_backup_pontos) {
        animacao_pontos_parar(estado);
        return;
    }


    for (int i = 0; i < estado->numPontos; i++) {
        estado->pontos[i].y -= velocidades[i];

        if (estado->pontos[i].y < 0) {
            estado->pontos[i].y = ALTURA_JANELA;
            estado->pontos[i].x = backup_pontos[i].x;
        }
    }
}

int is_animacao_pontos_ativa() {
    return animacao_ativa;
}
