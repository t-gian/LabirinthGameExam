#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

struct graph {
    int V;
    int C;
    int **madj;
    ST sym;
};

struct path {
    int len;
    int tes;
    int oro;
    int prof_ult_stanza;
    int *stanze;
    int TOT;
};

static int **MATRIX_init(int r, int c, int val);

static void v_cpy(int *dest, int *src, int k);

static int calc_tot(int ult_stanza, int oro, int tes);

static int is_visited(int *camm, int v, int len);

static void
GRAPH_path_best_R(Graph G, int M, int PF, int v, int len, int *len_max, int oro_corr, int tes_corr, int *TOT, int *camm,
                  int *camm_max);

static int **MATRIX_init(int r, int c, int val) {
    int i, j;

    int **t = malloc(r * sizeof(int *));

    if (t == NULL) return NULL;

    for (i = 0; i < r; i++) {
        t[i] = malloc(c * sizeof(int));

        if (t[i] == NULL) return NULL;
    }

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            t[i][j] = val;

    return t;
}

Graph GRAPH_init(int V) {
    Graph G = malloc(sizeof *G);

    if (G == NULL) return NULL;

    G->V = V;
    G->C = 0;
    G->madj = MATRIX_init(V, V, -1);

    if (G->madj == NULL) return NULL;

    G->sym = ST_init(V);

    if (G->sym == NULL) return NULL;

    return G;
}

void GRAPH_free(Graph G) {
    int i;

    for (i = 0; i < G->V; i++)
        free(G->madj[i]);

    free(G->madj);
    ST_free(G->sym);
    free(G);
}

Graph GRAPH_load(FILE *fp) {
    if (fp == NULL) return NULL;

    int V, i, prof, tes, oro, flag;
    char buff1[21], buff2[21];
    Graph G;

    fscanf(fp, "%d", &V);

    G = GRAPH_init(V);

    for (i = 0; i < V; i++) {
        fscanf(fp, "%s %d %d %d", buff1, &prof, &tes, &oro);
        ST_insert(G->sym, buff1, prof, tes, oro);
    }

    while (fscanf(fp, "%s %s %d", buff1, buff2, &flag) == 3) {
        GRAPH_insert_E(G, ST_get_index(G->sym, buff1), ST_get_index(G->sym, buff2), flag);
    }

    return G;
}

void GRAPH_insert_E(Graph G, int id1, int id2, int wt) {

    G->madj[id1][id2] = wt;
    G->madj[id2][id1] = wt;

    G->C++;
}

Path GRAPH_path_load(Graph G, FILE *fp) {
    Path path = malloc(sizeof(*path));
    char buff[21];
    int dim_max = G->V, dim = 0;

    path->oro = 0;
    path->tes = 0;
    path->TOT = 0;
    path->prof_ult_stanza = -1;

    path->stanze = malloc(dim_max * sizeof(int));

    while (fscanf(fp, "%s", buff) == 1) {
        if (dim >= dim_max - 1) {
            dim_max = dim_max * 2;
            path->stanze = realloc(path->stanze, dim_max * sizeof(int));
        }
        path->stanze[dim] = ST_get_index(G->sym, buff);
        dim++;
    }

    path->len = dim;

    return path;
}

void GRAPH_path_free(Path P) {

    free(P->stanze);
    free(P);
}

int GRAPH_path_check(Graph G, Path P, int M, int PF) {
    int i, tes, val;
    int *visited = calloc(P->len, sizeof(int));

    if (P->len > M)
        return 0;

    for (i = 0; i < P->len; i++) {
        if ((tes = ST_get_tes(G->sym, P->stanze[i])) > P->tes)
            P->tes = tes;
        if (!visited[i]) {
            P->oro += ST_get_oro(G->sym, P->stanze[i]);
            visited[i] = 1;
        }
        if (i != P->len - 1 && G->madj[P->stanze[i]][P->stanze[i + 1]] != -1)
            PF--;
    }

    P->prof_ult_stanza = ST_get_prof(G->sym, P->len - 1);

    val = P->oro + P->tes;

    switch (P->prof_ult_stanza) {
        case 0:
            P->TOT = val;
            break;
        case 1:
            P->TOT = (val * 2) / 3;
            break;
        case 2:
            P->TOT = (val * 1) / 2;
        default:
            P->TOT = 0;
            break;
    }

    if (PF < 0)
        return 0;

    free(visited);

    return 1;
}

Path GRAPH_path_init(int C) {
    Path P = malloc(sizeof *P);

    P->stanze = calloc(C+1, sizeof(int));
    P->len = 0;
    P->TOT = 0;
    P->tes = 0;
    P->prof_ult_stanza = -1;
    P->oro = 0;

    return P;
}

static void v_cpy(int *dest, int *src, int k) {
    int i;

    for (i = 0; i < k; i++)
        dest[i] = src[i];
}

static int calc_tot(int ult_stanza, int oro, int tes) {
    int val;

    switch (ult_stanza) {
        case 0:
            val = oro + tes;
            break;
        case 1:
            val = ((oro + tes) * 2) / 3;
            break;
        case 2:
            val = ((oro + tes) * 1) / 2;
        default:
            val = 0;
            break;
    }

    return val;
}

static int is_visited(int *camm, int v, int len){
    int i;

    for(i = 0; i < len; i++)
        if(camm[i] == v)
            return 1;

    return 0;
}

static void
GRAPH_path_best_R(Graph G, int M, int PF, int v, int len, int *len_max, int oro_corr, int tes_corr, int *TOT, int *camm,
                  int *camm_max) {
    int tes, w;

    if (M < 0 || PF < 0 ) {
        if ((tes = calc_tot(ST_get_prof(G->sym,camm[len-1]), oro_corr, tes_corr)) > *TOT) {
            (*TOT) = tes;
            (*len_max) = len;
            v_cpy(camm_max, camm, *len_max);
        }
        return;
    }
    camm[len] = v;
    if (!is_visited(camm, v, len)) {
        oro_corr += ST_get_oro(G->sym, v);
    }
    if ((tes = ST_get_tes(G->sym, v)) > tes_corr)
        tes_corr = tes;

    for (w = 0; w < G->V; w++) {
        if (G->madj[v][w] != -1) {
            GRAPH_path_best_R(G, M - 1, (G->madj[v][w])? PF-1:PF, w, len + 1, len_max, oro_corr, tes_corr, TOT, camm,
                              camm_max);
        }
    }
}

Path GRAPH_path_best(Graph G, int M, int PF) {
    int *camm_max = calloc(M+1, sizeof(int));
    int *camm = calloc(M+1, sizeof(int));
    int len_max = 0, TOT = 0;
    Path P = GRAPH_path_init(M);

    GRAPH_path_best_R(G, M, PF, 0, 0, &len_max, 0, 0, &TOT, camm, camm_max);

    v_cpy(P->stanze, camm_max, len_max);
    P->TOT = TOT;
    P->len = len_max;

    free(camm);
    free(camm_max);

    return P;
}

void GRAPH_path_out(Graph G, Path P){
    int i;

    for(i = 0; i < P->len-1; i++)
        printf("%s->", ST_get_nome(G->sym, P->stanze[i]));
    printf("%s\n", ST_get_nome(G->sym, P->stanze[i]));
    printf("\tPoints >> %d\n", P->TOT);
    printf("\tNumber >> %d\n", P->len-1);
}
