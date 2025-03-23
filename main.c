#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#define MAX_NODES 100

void solve_ILP(int n, int m, int graph[MAX_NODES][MAX_NODES], int terminals[], int l) {
    glp_prob *lp;
    int i, j, k;
    int edge_count = 0;
    
    lp = glp_create_prob();
    glp_set_prob_name(lp, "Arc-weighted l-cardinality tree");
    glp_set_obj_dir(lp, GLP_MIN);
    
    glp_add_cols(lp, m);
    for (i = 1; i <= m; i++) {
        glp_set_col_kind(lp, i, GLP_BV);
        glp_set_obj_coef(lp, i, graph[i-1][i]);
    }
    
    int *row_ind = malloc((m+1) * sizeof(int));
    double *row_val = malloc((m+1) * sizeof(double));
    
    glp_add_rows(lp, 1);
    glp_set_row_name(lp, 1, "cardinality");
    glp_set_row_bnds(lp, 1, GLP_UP, 0, l);
    
    for (i = 1; i <= m; i++) {
        row_ind[i] = i;
        row_val[i] = 1.0;
    }
    
    glp_set_mat_row(lp, 1, m, row_ind, row_val);
    
    glp_simplex(lp, NULL);
    
    printf("Optimal Solution:\n");
    for (i = 1; i <= m; i++) {
        if (glp_get_col_prim(lp, i) > 0.5) {
            printf("Edge %d included in the tree\n", i);
        }
    }

    free(row_ind);
    free(row_val);
    glp_delete_prob(lp);
}

int main() {
    int n = 5;
    int m = 7;
    int graph[MAX_NODES][MAX_NODES] = {
        {0, 2, 3, 0, 0},
        {2, 0, 4, 5, 0},
        {3, 4, 0, 6, 7},
        {0, 5, 6, 0, 8},
        {0, 0, 7, 8, 0}
    };
    
    int terminals[] = {1, 3, 4};
    int l = 4;
    
    solve_ILP(n, m, graph, terminals, l);
    
    return 0;
}
