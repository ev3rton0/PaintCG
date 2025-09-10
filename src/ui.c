#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>
#include "ui.h"
#include "animacao_pontos.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLuint logoTexturaID;
int logoLargura, logoAltura;

static void desenharTexto(float x, float y, char* texto) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, texto[i]);
    }
}

void inicializarBotoes() {
    char* labels[] = {"Ponto", "Reta", "Poligono", "Selecionar", "Animacao Pontos", "Limpar Tudo"};
    int modos[] = {1, 2, 3, 4, -2, -1};
    for (int i = 0; i < NUM_BOTOES; i++) {
        botoes[i].x = 10;
        botoes[i].y = ALTURA_JANELA - 160 - (i * 50);
        botoes[i].w = LARGURA_MENU - 20;
        botoes[i].h = 40;
        botoes[i].label = labels[i];
        botoes[i].modo_associado = modos[i];
    }
}

void carregarTextura(const char* caminho) {
    int nrCanais;

    stbi_set_flip_vertically_on_load(1);
    unsigned char *dados = stbi_load(caminho, &logoLargura, &logoAltura, &nrCanais, 0);

    if (dados) {
        GLenum formato = GL_RGB;
        if (nrCanais == 4) {
            formato = GL_RGBA;
        }

        glGenTextures(1, &logoTexturaID);
        glBindTexture(GL_TEXTURE_2D, logoTexturaID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, formato, logoLargura, logoAltura, 0, formato, GL_UNSIGNED_BYTE, dados);

        stbi_image_free(dados);
        printf("Textura '%s' carregada com sucesso! Dimensoes: %d x %d\n", caminho, logoLargura, logoAltura);
    } else {
        printf("Erro ao carregar a textura '%s'.\n", caminho);
    }
}

void desenharLogo(float x, float y, float w, float h) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, logoTexturaID);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void desenharMenuLateral() {
    float corFundo[]          = {0.17f, 0.24f, 0.31f};
    float corBotaoNormal[]    = {0.20f, 0.29f, 0.37f};
    float corBotaoSelecionado[] = {0.25f, 0.35f, 0.46f};
    float corIndicador[]      = {0.20f, 0.60f, 0.86f};
    float corSeparador[]      = {0.15f, 0.21f, 0.27f};
    float corTextoNormal[]    = {0.93f, 0.94f, 0.95f};
    float corTextoSelecionado[] = {1.0f, 1.0f, 1.0f};

    glViewport(0, 0, LARGURA_JANELA, ALTURA_JANELA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, LARGURA_JANELA, 0.0, ALTURA_JANELA);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3fv(corFundo);
    glBegin(GL_QUADS);
        glVertex2f(0, 0); glVertex2f(LARGURA_MENU, 0);
        glVertex2f(LARGURA_MENU, ALTURA_JANELA); glVertex2f(0, ALTURA_JANELA);
    glEnd();

    if (logoAltura > 0) {
        float logoAlturaDesejada = 100.0f;
        float aspectRatio = (float)logoLargura / (float)logoAltura;
        float logoLarguraCalculada = logoAlturaDesejada * aspectRatio;
        float logoPosX = (LARGURA_MENU - logoLarguraCalculada) / 2.0f;
        float logoPosY = ALTURA_JANELA - 110;

        desenharLogo(logoPosX, logoPosY, logoLarguraCalculada, logoAlturaDesejada);
    }

    for (int i = 0; i < NUM_BOTOES; i++) {
        float textoLargura = strlen(botoes[i].label) * 9.0f;
        float textoPosX = botoes[i].x + (botoes[i].w - textoLargura) / 2.0f;
        float textoPosY = botoes[i].y + (botoes[i].h / 2.0f) - 6.0f;

        int is_selected = (botoes[i].modo_associado == modo);
        if (botoes[i].modo_associado == -2 && is_animacao_pontos_ativa()) {
             is_selected = 1;
        }

        if (is_selected) {
            glColor3fv(corBotaoSelecionado);
            glBegin(GL_QUADS);
                glVertex2f(botoes[i].x, botoes[i].y);
                glVertex2f(botoes[i].x + botoes[i].w, botoes[i].y);
                glVertex2f(botoes[i].x + botoes[i].w, botoes[i].y + botoes[i].h);
                glVertex2f(botoes[i].x, botoes[i].y + botoes[i].h);
            glEnd();

            if (botoes[i].modo_associado > 0) {
                glColor3fv(corIndicador);
                glBegin(GL_QUADS);
                    glVertex2f(botoes[i].x, botoes[i].y);
                    glVertex2f(botoes[i].x + 5, botoes[i].y);
                    glVertex2f(botoes[i].x + 5, botoes[i].y + botoes[i].h);
                    glVertex2f(botoes[i].x, botoes[i].y + botoes[i].h);
                glEnd();
            }

            glColor3fv(corTextoSelecionado);
            desenharTexto(textoPosX, textoPosY, botoes[i].label);
        } else {
            glColor3fv(corBotaoNormal);
            glBegin(GL_QUADS);
                glVertex2f(botoes[i].x, botoes[i].y);
                glVertex2f(botoes[i].x + botoes[i].w, botoes[i].y);
                glVertex2f(botoes[i].x + botoes[i].w, botoes[i].y + botoes[i].h);
                glVertex2f(botoes[i].x, botoes[i].y + botoes[i].h);
            glEnd();

            glColor3fv(corTextoNormal);
            desenharTexto(textoPosX, textoPosY, botoes[i].label);
        }

        glColor3fv(corSeparador);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex2f(botoes[i].x, botoes[i].y);
            glVertex2f(botoes[i].x + botoes[i].w, botoes[i].y);
        glEnd();
    }

    float y_instrucoes = botoes[NUM_BOTOES-1].y - 60;
    glColor3fv(corTextoNormal);
    desenharTexto(10, y_instrucoes +20, "Instrucoes:");
    desenharTexto(10, y_instrucoes, " m: Eliminar Arestas Concavas");
    desenharTexto(10, y_instrucoes - 20, " r: Rot. Anti-horario");
    desenharTexto(10, y_instrucoes - 40, " t: Rot. Horario");
    desenharTexto(10, y_instrucoes - 60, " +: Aumenta Escala");
    desenharTexto(10, y_instrucoes - 80, " -: Diminui Escala");
    desenharTexto(10, y_instrucoes - 100, " x: Cisalhamento X");
    desenharTexto(10, y_instrucoes - 120, " c: Cisalhamento Y");
    desenharTexto(10, y_instrucoes - 140, " v: Reflexao X");
    desenharTexto(10, y_instrucoes - 160, " h: Reflexao Y");
    desenharTexto(10, y_instrucoes - 180, " d: Deselecionar");
    desenharTexto(10, y_instrucoes - 200, " f: Desenhar Poligono");
    desenharTexto(10, y_instrucoes - 220, " DEL: Deletar objeto");
    desenharTexto(10, y_instrucoes - 260, "Arquivo:");
    desenharTexto(10, y_instrucoes - 280, " S: Salvar Arquivo");
    desenharTexto(10, y_instrucoes - 300, " L: Carregar Arquivo");
}
