#ifndef GRAPH_H
#define GRAPH_H

#include "ST.h"

typedef struct graph *Graph;

typedef struct path *Path;

Graph GRAPH_init(int V);

void GRAPH_free(Graph G);

Graph GRAPH_load(FILE *fp);

void GRAPH_insert_E(Graph G, int id1, int id2, int wt);

void GRAPH_path_out(Graph G, Path P);

Path GRAPH_path_init(int C);

Path GRAPH_path_load(Graph G, FILE *fp);

void GRAPH_path_free(Path P);

int GRAPH_path_check(Graph G, Path P, int M, int PF);

Path GRAPH_path_best(Graph G, int M, int PF);

#endif
