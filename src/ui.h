#ifndef UI_H
#define UI_H


#define LARGURA_JANELA 1200
#define ALTURA_JANELA 800
#define LARGURA_MENU 300
#define NUM_BOTOES 6

typedef struct {
    float x, y, w, h;
    char* label;
    int modo_associado;
} Botao;

extern Botao botoes[NUM_BOTOES];
extern int modo;


void inicializarBotoes();
void desenharMenuLateral();
void carregarTextura(const char* caminho);
void desenharLogo(float x, float y, float w, float h);

#endif // UI_H
