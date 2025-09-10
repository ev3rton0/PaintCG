#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H

#include <stdlib.h>

typedef struct {
    float x;
    float y;
    int selecionado;
} Ponto;

typedef struct {
    Ponto p1;
    Ponto p2;
    int selecionado;
} Reta;

typedef struct {
    Ponto* vertices;
    int numVertices;
    int selecionado;
} Poligono;

Ponto calcularCentroReta(const Reta* r);
Ponto calcularCentroPoligono(const Poligono* poligono);

void transladarPonto(Ponto* p, float tx, float ty);
void rotacionarPonto(Ponto* p, float angulo);
void refletirPonto(Ponto* p, int eixoX, int eixoY);
void cisalharPonto(Ponto* p, float shx, float shy);

void transladarReta(Reta* r, float tx, float ty);
void rotacionarReta(Reta* r, float angulo);
void escalarReta(Reta* r, float sx, float sy);
void refletirReta(Reta* r, int eixoX, int eixoY);
void cisalharReta(Reta* r, float shx, float shy);

void transladarPoligono(Poligono* poligono, float tx, float ty);
void rotacionarPoligono(Poligono* poligono, float angulo);
void escalarPoligono(Poligono* poligono, float sx, float sy);
void refletirPoligono(Poligono* poligono, int eixoX, int eixoY);
void cisalharPoligono(Poligono* poligono, float shx, float shy);

void eliminarConcavidades(Poligono* poligono);

#endif
