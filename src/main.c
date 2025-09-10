#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>
#include "tinyfiledialogs.h"

#include "primitivas.h"
#include "gerenciador_objetos.h"
#include "selecao.h"
#include "ui.h"
#include "tessellator.h"
#include "animacao_pontos.h" 

Botao botoes[NUM_BOTOES];
int modo = 0;

static Ponto tempVertices[100];
static int tempNumVertices = 0;
static int arrastando = 0;
static Ponto p_inicial_drag;
static Ponto* ponto_arrastado = NULL;
static Reta* reta_arrastada = NULL;
static Poligono* poligono_arrastado = NULL;
static Ponto mousePos;

void idle() {
    animacao_pontos_atualizar(obter_estado());
    glutPostRedisplay();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    inicializar_gerenciador();
    inicializarBotoes();
    carregarTextura("logo.png");
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    desenharMenuLateral();

    glViewport(LARGURA_MENU, 0, LARGURA_JANELA - LARGURA_MENU, ALTURA_JANELA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, LARGURA_JANELA - LARGURA_MENU, 0.0, ALTURA_JANELA);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    EstadoObjetos* estado = obter_estado();

  
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < estado->numPontos; i++) {
        if (estado->pontos[i].selecionado) glColor3f(66.0f/255.0f, 27.0f/255.0f, 15.0f/255.0f);
        else glColor3f(240.0f/255.0f, 150.0f/255.0f, 93.0f/255.0f);
        glVertex2f(estado->pontos[i].x, estado->pontos[i].y);
    }
    glEnd();

    glLineWidth(2.0);
    for (int i = 0; i < estado->numRetas; i++) {
        if (estado->retas[i].selecionado) glColor3f(66.0f/255.0f, 27.0f/255.0f, 15.0f/255.0f);
        else glColor3f(148.0f/255.0f, 190.0f/255.0f, 70.0f/255.0f);
        glBegin(GL_LINES);
        glVertex2f(estado->retas[i].p1.x, estado->retas[i].p1.y);
        glVertex2f(estado->retas[i].p2.x, estado->retas[i].p2.y);
        glEnd();
    }

    if (modo == 2 && tempNumVertices == 1) {
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(7.0);
        glBegin(GL_POINTS);
        glVertex2f(tempVertices[0].x, tempVertices[0].y);
        glEnd();
        glLineWidth(1.0);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(5, 0xAAAA);
        glBegin(GL_LINES);
            glVertex2f(tempVertices[0].x, tempVertices[0].y);
            glVertex2f(mousePos.x, mousePos.y);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    if (modo == 3 && tempNumVertices > 0) {
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(7.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < tempNumVertices; i++) {
            glVertex2f(tempVertices[i].x, tempVertices[i].y);
        }
        glEnd();
        glLineWidth(1.0);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(5, 0xAAAA);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < tempNumVertices; i++) {
            glVertex2f(tempVertices[i].x, tempVertices[i].y);
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    for (int i = 0; i < estado->numPoligonos; i++) {
        if (estado->poligonos[i].selecionado) {
            glColor3f(66.0f/255.0f, 27.0f/255.0f, 15.0f/255.0f);
            glLineWidth(2.0);
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < estado->poligonos[i].numVertices; j++) {
                glVertex2f(estado->poligonos[i].vertices[j].x, estado->poligonos[i].vertices[j].y);
            }
            glEnd();
        } else {
            glColor3f(98.0f/255.0f, 195.0f/255.0f, 213.0f/255.0f);
            desenharPoligonoCorretamente(&estado->poligonos[i]);
        }
    }

    glFlush();
}

void passiveMotion(int x, int y) {
    if (x >= LARGURA_MENU) {
        mousePos.x = (float)(x - LARGURA_MENU);
        mousePos.y = (float)(ALTURA_JANELA - y);
        if (modo == 2 && tempNumVertices == 1) {
            glutPostRedisplay();
        }
    }
}

void mouse(int button, int state, int x, int y) {
    float screenX = (float)x;
    float screenY = ALTURA_JANELA - (float)y;
    EstadoObjetos* estado = obter_estado();

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (screenX < LARGURA_MENU) {
                for (int i = 0; i < NUM_BOTOES; i++) {
                    if (screenX > botoes[i].x && screenX < botoes[i].x + botoes[i].w &&
                        screenY > botoes[i].y && screenY < botoes[i].y + botoes[i].h) {

                        if (botoes[i].modo_associado == -1) {
                            if (is_animacao_pontos_ativa()) animacao_pontos_parar(estado);
                            limpar_objetos();
                            modo = 0;
                            printf("Tela foi limpa.\n");
                        } else if (botoes[i].modo_associado == -2) {
                            if (is_animacao_pontos_ativa()) {
                                animacao_pontos_parar(estado);
                            } else {
                                animacao_pontos_iniciar(estado);
                            }
                        } else {
                            if (is_animacao_pontos_ativa()) {
                                printf("Pare a animacao antes de mudar de modo.\n");
                            } else {
                                modo = botoes[i].modo_associado;
                                tempNumVertices = 0;
                                deselecionar_todos();
                                printf("Modo alterado para: %s\n", botoes[i].label);
                            }
                        }
                        glutPostRedisplay();
                        return;
                    }
                }
            }
            else {
                
                if (is_animacao_pontos_ativa()) {
                    printf("Pare a animacao para desenhar/editar objetos.\n");
                    return;
                }

                float worldX = screenX - LARGURA_MENU;
                float worldY = screenY;

                if (modo == 4) {
                    deselecionar_todos();
                    ponto_arrastado = NULL;
                    reta_arrastada = NULL;
                    poligono_arrastado = NULL;
                    int objeto_selecionado = 0;

                    for (int i = 0; i < estado->numPontos; i++) {
                        if (selecionaPonto(estado->pontos[i].x, estado->pontos[i].y, worldX, worldY, 5)) {
                            estado->pontos[i].selecionado = 1;
                            objeto_selecionado = 1;
                            ponto_arrastado = &estado->pontos[i];
                            printf("Ponto %d selecionado.\n", i);
                            break;
                        }
                    }
                    if (!objeto_selecionado) {
                        for (int i = 0; i < estado->numRetas; i++) {
                            if (selecionaReta(&estado->retas[i], worldX, worldY, 5)) {
                                estado->retas[i].selecionado = 1;
                                objeto_selecionado = 1;
                                reta_arrastada = &estado->retas[i];
                                printf("Reta %d selecionada.\n", i);
                                break;
                            }
                        }
                    }
                    if (!objeto_selecionado) {
                        for (int i = 0; i < estado->numPoligonos; i++) {
                            if (selecionaArea(&estado->poligonos[i], worldX, worldY)) {
                                estado->poligonos[i].selecionado = 1;
                                poligono_arrastado = &estado->poligonos[i];
                                printf("Poligono %d selecionado pela area.\n", i);
                                break;
                            }
                        }
                    }
                    if (objeto_selecionado || poligono_arrastado != NULL) {
                        arrastando = 1;
                        p_inicial_drag.x = worldX;
                        p_inicial_drag.y = worldY;
                    }
                } else if (modo == 1) {
                    adicionar_ponto(worldX, worldY);
                } else if (modo == 2) {
                    if (tempNumVertices == 0) {
                        tempVertices[0].x = worldX; tempVertices[0].y = worldY;
                        tempNumVertices++;
                    } else {
                        Ponto p2 = {worldX, worldY};
                        adicionar_reta(tempVertices[0], p2);
                        tempNumVertices = 0;
                    }
                } else if (modo == 3) {
                    if (tempNumVertices < 100) {
                        tempVertices[tempNumVertices].x = worldX;
                        tempVertices[tempNumVertices].y = worldY;
                        tempNumVertices++;
                    }
                }
            }
        } else if (state == GLUT_UP) {
            arrastando = 0;
            ponto_arrastado = NULL;
            reta_arrastada = NULL;
            poligono_arrastado = NULL;
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (is_animacao_pontos_ativa()) return; 
    if (arrastando) {
        float worldX = (float)x - LARGURA_MENU;
        float worldY = ALTURA_JANELA - (float)y;
        float tx = worldX - p_inicial_drag.x;
        float ty = worldY - p_inicial_drag.y;

        if (ponto_arrastado != NULL) transladarPonto(ponto_arrastado, tx, ty);
        else if (reta_arrastada != NULL) transladarReta(reta_arrastada, tx, ty);
        else if (poligono_arrastado != NULL) transladarPoligono(poligono_arrastado, tx, ty);

        p_inicial_drag.x = worldX;
        p_inicial_drag.y = worldY;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (is_animacao_pontos_ativa()) { 
        printf("Pare a animacao para usar o teclado.\n");
        return;
    }

    EstadoObjetos* estado = obter_estado();
    Ponto* pontoSelecionado = NULL;
    Reta* retaSelecionada = NULL;
    Poligono* poligonoSelecionado = NULL;

    for (int i = 0; i < estado->numPontos; i++) if (estado->pontos[i].selecionado) { pontoSelecionado = &estado->pontos[i]; break; }
    if (!pontoSelecionado) for (int i = 0; i < estado->numRetas; i++) if (estado->retas[i].selecionado) { retaSelecionada = &estado->retas[i]; break; }
    if (!pontoSelecionado && !retaSelecionada) for (int i = 0; i < estado->numPoligonos; i++) if (estado->poligonos[i].selecionado) { poligonoSelecionado = &estado->poligonos[i]; break; }

    switch (key) {
        case 'f': case 'F':
            if (modo == 3 && tempNumVertices > 1) {
                adicionar_poligono(tempVertices, tempNumVertices);
                tempNumVertices = 0;
                printf("Poligono criado!\n");
            }
            break;
        case 'r': case 'R':
            if (pontoSelecionado) rotacionarPonto(pontoSelecionado, 5.0);
            else if (retaSelecionada) rotacionarReta(retaSelecionada, 5.0);
            else if (poligonoSelecionado) rotacionarPoligono(poligonoSelecionado, 5.0);
            break;
        case 't': case 'T':
            if (pontoSelecionado) rotacionarPonto(pontoSelecionado, -5.0);
            else if (retaSelecionada) rotacionarReta(retaSelecionada, -5.0);
            else if (poligonoSelecionado) rotacionarPoligono(poligonoSelecionado, -5.0);
            break;
        case '+': case '=':
            if (retaSelecionada) escalarReta(retaSelecionada, 1.1, 1.1);
            else if (poligonoSelecionado) escalarPoligono(poligonoSelecionado, 1.1, 1.1);
            break;
        case '-':
            if (retaSelecionada) escalarReta(retaSelecionada, 0.9, 0.9);
            else if (poligonoSelecionado) escalarPoligono(poligonoSelecionado, 0.9, 0.9);
            break;
        case 'h': case 'H':
            if (pontoSelecionado) refletirPonto(pontoSelecionado, 1, 0);
            else if (retaSelecionada) refletirReta(retaSelecionada, 1, 0);
            else if (poligonoSelecionado) refletirPoligono(poligonoSelecionado, 1, 0);
            break;
        case 'v': case 'V':
            if (pontoSelecionado) refletirPonto(pontoSelecionado, 0, 1);
            else if (retaSelecionada) refletirReta(retaSelecionada, 0, 1);
            else if (poligonoSelecionado) refletirPoligono(poligonoSelecionado, 0, 1);
            break;
        case 'x': case 'X':
            if (pontoSelecionado) cisalharPonto(pontoSelecionado, 0.1, 0.0);
            else if (retaSelecionada) cisalharReta(retaSelecionada, 0.1, 0.0);
            else if (poligonoSelecionado) cisalharPoligono(poligonoSelecionado, 0.1, 0.0);
            break;
        case 'c': case 'C':
            if (pontoSelecionado) cisalharPonto(pontoSelecionado, 0.0, 0.1);
            else if (retaSelecionada) cisalharReta(retaSelecionada, 0.0, 0.1);
            else if (poligonoSelecionado) cisalharPoligono(poligonoSelecionado, 0.0, 0.1);
            break;
        case 's': case 'S': {
            const char* filename = tinyfd_saveFileDialog("Salvar Projeto", ".txt", 0, NULL, NULL);
            if (filename) {
                salvar_objetos(filename);
                printf("Objetos salvos em %s\n", filename);
            }
            break;
        }
        case 'l': case 'L': {
            const char* filename = tinyfd_openFileDialog("Abrir Projeto", "", 0, NULL, NULL, 0);
            if (filename) {
                carregar_objetos(filename);
                glutPostRedisplay();
                printf("Objetos carregados de %s\n", filename);
            }
            break;
        }
        case 'd': case 'D':
            deselecionar_todos();
            printf("Objetos deselecionados.\n");
            break;
        case 'm':
            if (poligonoSelecionado) eliminarConcavidades(poligonoSelecionado);
            else printf("Imposs�vel eliminar concavidades para a forma selecionada.");
            break;
        case 127:
            deletar_objetos_selecionados();
            printf("Objetos selecionados foram deletados.\n");
            break;

    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("FuboPaint");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
