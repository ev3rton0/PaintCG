#include "selecao.h"
#include <stdio.h>

const int DENTRO = 0;
const int ESQUERDA = 1;
const int DIREITA = 2;
const int ABAIXO = 4;
const int ACIMA = 8;

int selecionaPonto(float px, float py, float mx, float my, int tol) {
    return (mx <= px + tol && mx >= px - tol && my <= py + tol && my >= py - tol);
}

int calcularOutcode(float x, float y, float xmin, float xmax, float ymin, float ymax) {
    int code = DENTRO;

    if (x < xmin)
        code |= ESQUERDA;
    else if (x > xmax)
        code |= DIREITA;

    if (y < ymin)
        code |= ABAIXO;
    else if (y > ymax)
        code |= ACIMA;

    return code;
}

int selecionaReta(const Reta* reta, float mx, float my, int tol) {
    float xmin = mx - tol;
    float xmax = mx + tol;
    float ymin = my - tol;
    float ymax = my + tol;

    float x0 = reta->p1.x, y0 = reta->p1.y;
    float x1 = reta->p2.x, y1 = reta->p2.y;

    int outcode0 = calcularOutcode(x0, y0, xmin, xmax, ymin, ymax);
    int outcode1 = calcularOutcode(x1, y1, xmin, xmax, ymin, ymax);

    while (1) {
        if (!(outcode0 | outcode1)) {
            return 1;
        } else if (outcode0 & outcode1) {
            return 0;
        } else {
            float x, y;
            int outcodeFora = outcode0 ? outcode0 : outcode1;

            if (outcodeFora & ACIMA) {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeFora & ABAIXO) {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeFora & DIREITA) {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else {
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeFora == outcode0) {
                x0 = x; y0 = y;
                outcode0 = calcularOutcode(x0, y0, xmin, xmax, ymin, ymax);
            } else {
                x1 = x; y1 = y;
                outcode1 = calcularOutcode(x1, y1, xmin, xmax, ymin, ymax);
            }
        }
    }
}

int selecionaArea(const Poligono* poligono, float mx, float my) {
    int i, j, intersecoes = 0;

    for (i = 0, j = poligono->numVertices - 1; i < poligono->numVertices; j = i++) {
        Ponto p1 = poligono->vertices[i];
        Ponto p2 = poligono->vertices[j];

        if (p1.y == my && p2.y == my) {
            continue;
        }

        if (p1.y == my && p1.x >= mx) {
            if (p2.y > my) {
                 intersecoes++;
            }
            continue;
        }
        if (p2.y == my && p2.x >= mx) {
             if (p1.y > my) {
                 intersecoes++;
            }
            continue;
        }

        if (((p1.y > my) != (p2.y > my))) {
            double xi = p1.x + (my - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            if (xi > mx) {
                intersecoes++;
            }
        }
    }

    return (intersecoes % 2 == 1);
}
