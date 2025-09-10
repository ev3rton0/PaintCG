#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "tessellator.h"

typedef struct Mem_Node {
    void* ptr;
    struct Mem_Node* next;
} Mem_Node;

static Mem_Node* g_tess_mem_head = NULL;

static void add_tess_mem(void* ptr) {
    Mem_Node* new_node = (Mem_Node*)malloc(sizeof(Mem_Node));
    new_node->ptr = ptr;
    new_node->next = g_tess_mem_head;
    g_tess_mem_head = new_node;
}

static void free_tess_mem() {
    Mem_Node* current = g_tess_mem_head;
    while (current != NULL) {
        Mem_Node* next = current->next;
        free(current->ptr);
        free(current);
        current = next;
    }
    g_tess_mem_head = NULL;
}

static void CALLBACK tessBeginCallback(GLenum type) { glBegin(type); }
static void CALLBACK tessEndCallback(void) { glEnd(); }
static void CALLBACK tessVertexCallback(GLvoid *data) {
    GLdouble *ptr = (GLdouble*)data;
    glVertex3dv(ptr);
}
static void CALLBACK tessErrorCallback(GLenum errorCode) {
    const GLubyte *errorStr = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", errorStr);
}
static void CALLBACK tessCombineCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **outData) {
    GLdouble *new_vertex = (GLdouble *) malloc(sizeof(GLdouble) * 3);
    if (!new_vertex) return;
    add_tess_mem(new_vertex);
    new_vertex[0] = coords[0];
    new_vertex[1] = coords[1];
    new_vertex[2] = coords[2];
    *outData = new_vertex;
}

void desenharPoligonoCorretamente(const Poligono* poligono) {
    if (poligono == NULL || poligono->numVertices < 3) return;

    GLdouble (*vertices_data)[3] = malloc(poligono->numVertices * sizeof(GLdouble[3]));
    if (!vertices_data) return;

    for (int i = 0; i < poligono->numVertices; i++) {
        vertices_data[i][0] = poligono->vertices[i].x;
        vertices_data[i][1] = poligono->vertices[i].y;
        vertices_data[i][2] = 0;
    }

    GLUtesselator* tess = gluNewTess();
    gluTessCallback(tess, GLU_TESS_BEGIN, (_GLUfuncptr) &tessBeginCallback);
    gluTessCallback(tess, GLU_TESS_END, (_GLUfuncptr) &tessEndCallback);
    gluTessCallback(tess, GLU_TESS_VERTEX, (_GLUfuncptr) &tessVertexCallback);
    gluTessCallback(tess, GLU_TESS_ERROR, (_GLUfuncptr) &tessErrorCallback);
    gluTessCallback(tess, GLU_TESS_COMBINE, (_GLUfuncptr) &tessCombineCallback);
    gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

    gluTessBeginPolygon(tess, NULL);
        gluTessBeginContour(tess);
            for (int i = 0; i < poligono->numVertices; i++) {
                gluTessVertex(tess, vertices_data[i], vertices_data[i]);
            }
        gluTessEndContour(tess);
    gluTessEndPolygon(tess);

    gluDeleteTess(tess);
    free(vertices_data);
    free_tess_mem();
}
