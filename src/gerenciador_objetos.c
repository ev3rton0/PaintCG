#include "gerenciador_objetos.h"
#include <stdio.h>

static EstadoObjetos estado;

void inicializar_gerenciador(void) {
    estado.numPontos = 0;
    estado.numRetas = 0;
    estado.numPoligonos = 0;
}

void adicionar_ponto(float x, float y) {
    if (estado.numPontos < MAX_OBJETOS) {
        estado.pontos[estado.numPontos].x = x;
        estado.pontos[estado.numPontos].y = y;
        estado.pontos[estado.numPontos].selecionado = 0;
        estado.numPontos++;
    }
}

void adicionar_reta(Ponto p1, Ponto p2) {
    if (estado.numRetas < MAX_OBJETOS) {
        estado.retas[estado.numRetas].p1 = p1;
        estado.retas[estado.numRetas].p2 = p2;
        estado.retas[estado.numRetas].selecionado = 0;
        estado.numRetas++;
    }
}

void adicionar_poligono(Ponto* vertices, int numVertices) {
    if (estado.numPoligonos < MAX_OBJETOS) {
        Poligono* poligono = &estado.poligonos[estado.numPoligonos];
        poligono->numVertices = numVertices;
        poligono->vertices = (Ponto*)malloc(numVertices * sizeof(Ponto));
        for (int i = 0; i < numVertices; i++) {
            poligono->vertices[i] = vertices[i];
        }
        poligono->selecionado = 0;
        estado.numPoligonos++;
    }
}

void limpar_objetos(void) {
    for (int i = 0; i < estado.numPoligonos; i++) {
        free(estado.poligonos[i].vertices);
    }
    inicializar_gerenciador();
}

void deselecionar_todos(void) {
    for (int i = 0; i < estado.numPontos; i++) estado.pontos[i].selecionado = 0;
    for (int i = 0; i < estado.numRetas; i++) estado.retas[i].selecionado = 0;
    for (int i = 0; i < estado.numPoligonos; i++) estado.poligonos[i].selecionado = 0;
}

EstadoObjetos* obter_estado(void) {
    return &estado;
}

void salvar_objetos(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Erro ao salvar no arquivo!\n");
        return;
    }

    fprintf(f, "PONTOS %d\n", estado.numPontos);
    for (int i = 0; i < estado.numPontos; i++) {
        fprintf(f, "%f %f\n", estado.pontos[i].x, estado.pontos[i].y);
    }

    fprintf(f, "RETAS %d\n", estado.numRetas);
    for (int i = 0; i < estado.numRetas; i++) {
        fprintf(f, "%f %f %f %f\n",
            estado.retas[i].p1.x, estado.retas[i].p1.y,
            estado.retas[i].p2.x, estado.retas[i].p2.y);
    }

    fprintf(f, "POLIGONOS %d\n", estado.numPoligonos);
    for (int i = 0; i < estado.numPoligonos; i++) {
        fprintf(f, "%d\n", estado.poligonos[i].numVertices);
        for (int j = 0; j < estado.poligonos[i].numVertices; j++) {
            fprintf(f, "%f %f\n",
                estado.poligonos[i].vertices[j].x,
                estado.poligonos[i].vertices[j].y);
        }
    }

    fclose(f);
    printf("Objetos salvos em %s\n", filename);
}
void carregar_objetos(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Erro ao abrir arquivo para carregar!\n");
        return;
    }

    limpar_objetos();

    int n, numVertices;
    float x, y;

    fscanf(f, "PONTOS %d\n", &n);
    for (int i = 0; i < n; i++) {
        fscanf(f, "%f %f\n", &x, &y);
        adicionar_ponto(x, y);
    }

    fscanf(f, "RETAS %d\n", &n);
    for (int i = 0; i < n; i++) {
        Ponto p1, p2;
        fscanf(f, "%f %f %f %f\n", &p1.x, &p1.y, &p2.x, &p2.y);
        adicionar_reta(p1, p2);
    }

    fscanf(f, "POLIGONOS %d\n", &n);
    for (int i = 0; i < n; i++) {
        fscanf(f, "%d\n", &numVertices);
        Ponto* vertices = malloc(numVertices * sizeof(Ponto));
        for (int j = 0; j < numVertices; j++) {
            fscanf(f, "%f %f\n", &vertices[j].x, &vertices[j].y);
        }
        adicionar_poligono(vertices, numVertices);
        free(vertices);
    }

    fclose(f);
    printf("Objetos carregados de %s\n", filename);
}

void deletar_objetos_selecionados(void) {
    for (int i = 0; i < estado.numPontos; i++) {
        if (estado.pontos[i].selecionado) {
            for (int j = i; j < estado.numPontos - 1; j++) {
                estado.pontos[j] = estado.pontos[j + 1];
            }
            estado.numPontos--;
            i--;
        }
    }

    for (int i = 0; i < estado.numRetas; i++) {
        if (estado.retas[i].selecionado) {
            for (int j = i; j < estado.numRetas - 1; j++) {
                estado.retas[j] = estado.retas[j + 1];
            }
            estado.numRetas--;
            i--;
        }
    }

    for (int i = 0; i < estado.numPoligonos; i++) {
        if (estado.poligonos[i].selecionado) {
            free(estado.poligonos[i].vertices);
            for (int j = i; j < estado.numPoligonos - 1; j++) {
                estado.poligonos[j] = estado.poligonos[j + 1];
            }
            estado.numPoligonos--;
            i--;
        }
    }
}
