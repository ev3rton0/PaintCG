#include "primitivas.h"
#include "matriz.h"
#include <math.h>
#include <stdio.h>

Ponto calcularCentroReta(const Reta* r) {
    Ponto centro;
    centro.x = (r->p1.x + r->p2.x) / 2.0f;
    centro.y = (r->p1.y + r->p2.y) / 2.0f;
    return centro;
}

Ponto calcularCentroPoligono(const Poligono* poligono) {
    Ponto centro = {0.0, 0.0};
    if (poligono == NULL || poligono->numVertices == 0) return centro;
    for (int i = 0; i < poligono->numVertices; i++) {
        centro.x += poligono->vertices[i].x;
        centro.y += poligono->vertices[i].y;
    }
    centro.x /= poligono->numVertices;
    centro.y /= poligono->numVertices;
    return centro;
}

void transladarPonto(Ponto* p, float tx, float ty) {
    if (p == NULL) return;
    Matriz3x3 T;
    matriz_translacao(&T, tx, ty);
    matriz_aplica_ponto(p, p, &T);
}

void rotacionarPonto(Ponto* p, float angulo) {
    if (p == NULL) return;
    float rad = angulo * M_PI / 180.0;

    Matriz3x3 R;
    matriz_rotacao(&R, rad);
    matriz_aplica_ponto(p, p, &R);
}

void refletirPonto(Ponto* p, int eixoX, int eixoY) {
    if (p == NULL) return;
    Matriz3x3 R;
    matriz_reflexao(&R, eixoX, eixoY);
    matriz_aplica_ponto(p, p, &R);
}

void cisalharPonto(Ponto* p, float shx, float shy) {
    if (p == NULL) return;
    Matriz3x3 Sh;
    matriz_cisalhamento(&Sh, shx, shy);
    matriz_aplica_ponto(p, p, &Sh);
}

void transladarReta(Reta* r, float tx, float ty) {
    if (r == NULL) return;
    transladarPonto(&r->p1, tx, ty);
    transladarPonto(&r->p2, tx, ty);
}

void rotacionarReta(Reta* r, float angulo) {
    if (r == NULL) return;
    Ponto centro = calcularCentroReta(r);
    float rad = angulo * M_PI / 180.0;

    Matriz3x3 T_ida, R, T_volta, M_temp, M_final;
    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_rotacao(&R, rad);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &R, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    matriz_aplica_ponto(&r->p1, &r->p1, &M_final);
    matriz_aplica_ponto(&r->p2, &r->p2, &M_final);
}

void escalarReta(Reta* r, float sx, float sy) {
    if (r == NULL) return;
    Ponto centro = calcularCentroReta(r);

    Matriz3x3 T_ida, S, T_volta, M_temp, M_final;
    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_escala(&S, sx, sy);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &S, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    matriz_aplica_ponto(&r->p1, &r->p1, &M_final);
    matriz_aplica_ponto(&r->p2, &r->p2, &M_final);
}

void refletirReta(Reta* r, int eixoX, int eixoY) {
    if (r == NULL) return;
    Ponto centro = calcularCentroReta(r);
    Matriz3x3 T_ida, R, T_volta, M_temp, M_final;

    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_reflexao(&R, eixoX, eixoY);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &R, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    matriz_aplica_ponto(&r->p1, &r->p1, &M_final);
    matriz_aplica_ponto(&r->p2, &r->p2, &M_final);
}

void cisalharReta(Reta* r, float shx, float shy) {
    if (r == NULL) return;
    Ponto centro = calcularCentroReta(r);
    Matriz3x3 T_ida, Sh, T_volta, M_temp, M_final;

    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_cisalhamento(&Sh, shx, shy);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &Sh, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    matriz_aplica_ponto(&r->p1, &r->p1, &M_final);
    matriz_aplica_ponto(&r->p2, &r->p2, &M_final);
}

void transladarPoligono(Poligono* poligono, float tx, float ty) {
    if (poligono == NULL) return;
    Matriz3x3 T;
    matriz_translacao(&T, tx, ty);
    for (int i = 0; i < poligono->numVertices; i++) {
        matriz_aplica_ponto(&poligono->vertices[i], &poligono->vertices[i], &T);
    }
}

void rotacionarPoligono(Poligono* poligono, float angulo) {
    if (poligono == NULL) return;
    Ponto centro = calcularCentroPoligono(poligono);
    float rad = angulo * M_PI / 180.0;

    Matriz3x3 T_ida, R, T_volta, M_temp, M_final;
    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_rotacao(&R, rad);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &R, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    for (int i = 0; i < poligono->numVertices; i++) {
        matriz_aplica_ponto(&poligono->vertices[i], &poligono->vertices[i], &M_final);
    }
}

void escalarPoligono(Poligono* poligono, float sx, float sy) {
    if (poligono == NULL) return;
    Ponto centro = calcularCentroPoligono(poligono);

    Matriz3x3 T_ida, S, T_volta, M_temp, M_final;
    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_escala(&S, sx, sy);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &S, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    for (int i = 0; i < poligono->numVertices; i++) {
        matriz_aplica_ponto(&poligono->vertices[i], &poligono->vertices[i], &M_final);
    }
}

void refletirPoligono(Poligono* poligono, int eixoX, int eixoY) {
    if (poligono == NULL) return;
    Ponto centro = calcularCentroPoligono(poligono);
    Matriz3x3 T_ida, R, T_volta, M_temp, M_final;

    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_reflexao(&R, eixoX, eixoY);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &R, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    for (int i = 0; i < poligono->numVertices; i++) {
        matriz_aplica_ponto(&poligono->vertices[i], &poligono->vertices[i], &M_final);
    }
}

void cisalharPoligono(Poligono* poligono, float shx, float shy) {
    if (poligono == NULL) return;
    Ponto centro = calcularCentroPoligono(poligono);
    Matriz3x3 T_ida, Sh, T_volta, M_temp, M_final;

    matriz_translacao(&T_ida, -centro.x, -centro.y);
    matriz_cisalhamento(&Sh, shx, shy);
    matriz_translacao(&T_volta, centro.x, centro.y);

    matriz_multiplica(&M_temp, &Sh, &T_ida);
    matriz_multiplica(&M_final, &T_volta, &M_temp);

    for (int i = 0; i < poligono->numVertices; i++) {
        matriz_aplica_ponto(&poligono->vertices[i], &poligono->vertices[i], &M_final);
    }
}

float prodVetorial(Ponto a, Ponto b, Ponto c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); // retorna o produto vetorial entre ab e ac
}

int areaPoligono(Poligono* poligono) { // usa a �rea para definir a orienta��o do pol�gono (sentido hor�rio ou antihor�rio)
    float area = 0;
    for (int i = 0; i < poligono->numVertices; i++) {
        int j = (i+1) % poligono->numVertices; // pega sempre o pr�ximo v�rtice (volta para 0 se o atual for o �ltimo)
        area += poligono->vertices[i].x * poligono->vertices[j].y - poligono->vertices[j].x * poligono->vertices[i].y; // calcula o determinante do ponto atual e do pr�ximo ponto
    }
    area = area / 2.0f;

    return (area < 0) ? -1:1; // o sinal da �rea assinalada indica a orienta��o global, ou seja, se for menor que 0, � hor�rio, e vice versa
}

void eliminarConcavidades(Poligono* poligono) {
    int clockWise = areaPoligono(poligono);

    int changed = 1;

    while (changed) { 
        changed = 0;
        for (int i=0; i<poligono->numVertices; i++) { 
            Ponto ant = poligono->vertices[(i-1 + poligono->numVertices) % poligono->numVertices]; // 
            Ponto atual = poligono->vertices[i]; 
            Ponto prox = poligono->vertices[(i+1) % poligono->numVertices]; 

            float prodVet = prodVetorial(ant, atual, prox); 

            if ((clockWise == 1 && prodVet < 0) || (clockWise == -1 && prodVet > 0)) {
                for (int j=i; j<poligono->numVertices-1; j++)
                    poligono->vertices[j] = poligono->vertices[j+1]; 
                poligono->numVertices--; 
                changed = 1; 
                break;
            }
        }
    }
}
