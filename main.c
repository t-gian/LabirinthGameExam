#include <stdio.h>
#include "Graph.h"
#include <stdlib.h>
#include <assert.h>

#define F_GR "Graph2.txt"
#define F_PT "Path2.txt"

int main(int argc, char **argv) {
    FILE *fp_g, *fp_p;
    int PF, M;
    Graph my_graph;
    Path my_path, best_path;


    assert(argc == 3);
    printf("Entrance, Starting Points >> %d\n", (PF = atoi(argv[1])), (M = atoi(argv[2])));


    fp_g = fopen(F_GR, "r");
    fp_p = fopen(F_PT, "r");
    assert(fp_g != NULL && fp_p != NULL);

    my_graph = GRAPH_load(fp_g);
    fclose(fp_g);
    my_path = GRAPH_path_load(my_graph, fp_p);
    fclose(fp_p);
  
    GRAPH_path_out(my_graph, my_path);


    best_path = GRAPH_path_best(my_graph, M, PF);

    GRAPH_path_out(my_graph, best_path);

    GRAPH_free(my_graph);
    GRAPH_path_free(my_path);
    GRAPH_path_free(best_path);

    return 0;
}
