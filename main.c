#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 10
#define MAX_EDGES 20

typedef struct {
    int u, v;
    double weight;
} Edge;

int n = 5;  
int m = 8;  
Edge edges[MAX_EDGES] = {
    {0, 1, 1.0}, {0, 2, 2.0}, {1, 2, 3.0}, {1, 3, 4.0},
    {2, 3, 5.0}, {3, 4, 6.0}, {2, 4, 7.0}, {0, 4, 8.0}
};
int l = 4;  

int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

void union_sets(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

int is_valid_tree(int selected_edges[], int edge_count) {
    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    int edge_in_tree = 0;
    for (int i = 0; i < m; i++) {
        if (selected_edges[i]) {
            int u = edges[i].u;
            int v = edges[i].v;
            int set_u = find(parent, u);
            int set_v = find(parent, v);
            if (set_u == set_v) {
                return 0; 
            }
            union_sets(parent, rank, set_u, set_v);
            edge_in_tree++;
        }
    }

    return edge_in_tree == l; 
}

double calculate_weight(int selected_edges[]) {
    double total_weight = 0.0;
    for (int i = 0; i < m; i++) {
        if (selected_edges[i]) {
            total_weight += edges[i].weight;
        }
    }
    return total_weight;
}

void solve() {
    int best_solution[MAX_EDGES];
    double best_weight = 1e9; 
    int total_combinations = 1 << m; 
    for (int comb = 0; comb < total_combinations; comb++) {
        int selected_edges[MAX_EDGES];
        for (int i = 0; i < m; i++) {
            selected_edges[i] = (comb >> i) & 1;
        }
        if (is_valid_tree(selected_edges, l)) {
            double weight = calculate_weight(selected_edges);
            if (weight < best_weight) {
                best_weight = weight;
                memcpy(best_solution, selected_edges, sizeof(selected_edges));
            }
        }
    }
    printf("Best tree with weight %.2f:\n", best_weight);
    for (int i = 0; i < m; i++) {
        if (best_solution[i]) {
            printf("Edge: (%d, %d) with weight %.2f\n", edges[i].u, edges[i].v, edges[i].weight);
        }
    }
}

int main() {
    solve();
    return 0;
}
