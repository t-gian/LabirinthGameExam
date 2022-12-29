#ifndef ST_H
#define ST_H

typedef struct symboltable *ST;

ST ST_init(int maxN);

void ST_free(ST st);

void ST_insert(ST st, char *nome, int prof, int tes, int oro);

int ST_get_index(ST st, char *nome);

int ST_get_prof(ST st, int ind);

int ST_get_tes(ST st, int ind);

int ST_get_oro(ST st, int ind);

char *ST_get_nome(ST st, int ind);


#endif
