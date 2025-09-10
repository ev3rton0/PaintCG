#ifndef GERENCIADOR_OBJETOS_H
#define GERENCIADOR_OBJETOS_H

#include "primitivas.h"

#define MAX_OBJETOS 100

typedef struct {
    Ponto pontos[MAX_OBJETOS];
    int numPontos;
    Reta retas[MAX_OBJETOS];
    int numRetas;
    Poligono poligonos[MAX_OBJETOS];
    int numPoligonos;
} EstadoObjetos;

void inicializar_gerenciador(void);
void adicionar_ponto(float x, float y);
void adicionar_reta(Ponto p1, Ponto p2);
void adicionar_poligono(Ponto* vertices, int numVertices);
void limpar_objetos(void);
void deselecionar_todos(void);
void deletar_objetos_selecionados(void);
void salvar_objetos(const char* filename);
void carregar_objetos(const char* filename);
EstadoObjetos* obter_estado(void);


#endif
