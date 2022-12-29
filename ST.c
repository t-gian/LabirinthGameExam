#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ST.h"

struct symboltable {
    char **nomi;
    int *oro;
    int *tes;
    int *prof;
    int N;
    int maxN;
};

ST ST_init(int maxN) {
    ST st;

    st = malloc(sizeof(*st));
    assert(st != NULL);

    st->nomi = calloc(maxN, sizeof(char *));
    assert(st->nomi != NULL);

    st->oro = calloc(maxN, sizeof(int));
    assert(st->oro != NULL);

    st->tes = calloc(maxN, sizeof(int));
    assert(st->tes != NULL);

    st->prof = calloc(maxN, sizeof(int));
    assert(st->prof != NULL);

    st->N = 0;
    st->maxN = maxN;

    return st;
}

void ST_free(ST st) {
    int i;

    if (st == NULL) return;

    for (i = 0; i < st->N; i++) {
        if (st->nomi[i] != NULL) free(st->nomi[i]);
    }

    if (st->oro != NULL) free(st->oro);

    if (st->tes != NULL) free(st->tes);

    if (st->prof != NULL) free(st->prof);

    free(st->nomi);

    free(st);
}

void ST_insert(ST st, char *nome, int prof, int tes, int oro) {
    if (st->N + 1 >= st->maxN) {
        st->nomi = realloc(st->nomi, (2 * st->maxN) * sizeof(char *));
        st->oro = realloc(st->oro, (2 * st->maxN) * sizeof(int));
        st->tes = realloc(st->tes, (2 * st->maxN) * sizeof(int));
        st->prof = realloc(st->prof, (2 * st->maxN) * sizeof(int));

        if (st->nomi == NULL || st->oro == NULL || st->tes == NULL) return;

        st->maxN = 2 * st->maxN;
    }

    st->nomi[st->N] = strdup(nome);
    st->prof[st->N] = prof;
    st->tes[st->N] = tes;
    st->oro[st->N] = oro;

    st->N++;
}

int ST_get_index(ST st, char *nome) {
    int i;

    for (i = 0; i < st->N; i++)
        if (st->nomi[i] != NULL && strcmp(nome, st->nomi[i]) == 0)
            return i;
    return -1;
}

int ST_get_prof(ST st, int ind) {

    return st->prof[ind];
}

int ST_get_tes(ST st, int ind) {

    return st->tes[ind];
}

int ST_get_oro(ST st, int ind) {

    return st->oro[ind];
}

char *ST_get_nome(ST st, int ind) {

    return (st->nomi[ind]);
}
