#ifndef SELECAO_H
#define SELECAO_H

#include "primitivas.h"

int selecionaPonto(float px, float py, float mx, float my, int tol);
int selecionaReta(const Reta* reta, float mx, float my, int tol);
int selecionaArea(const Poligono* poligono, float mx, float my);

#endif
