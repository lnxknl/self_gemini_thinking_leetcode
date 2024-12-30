#include <stdio.h>
#include <stdlib.h>

// Disjoint Set Union (DSU) structure
typedef struct {
    int *parent;
    int *rank;
} DSU;

DSU* createDSU(int num_nodes) {
    DSU* dsu = (DSU*)malloc(sizeof(DSU));
    dsu->parent = (int*)malloc(num_nodes * sizeof(int));
    dsu->rank = (int*)malloc(num_nodes * sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
    return dsu;
}

int find(DSU* dsu, int i) {
    if (dsu->parent[i] == i)
        return i;
    return dsu->parent[i] = find(dsu, dsu->parent[i]); // Path compression
}

void unionSets(DSU* dsu, int a, int b) {
    int rootA = find(dsu, a);
    int rootB = find(dsu, b);
    if (rootA != rootB) {
        if (dsu->rank[rootA] < dsu->rank[rootB])
            dsu->parent[rootA] = rootB;
        else if (dsu->rank[rootA] > dsu->rank[rootB])
            dsu->parent[rootB] = rootA;
        else {
            dsu->parent[rootB] = rootA;
            dsu->rank[rootA]++;
        }
    }
}

int minConnections(int num_nodes, int connections[][2], int num_connections, int lonely_nodes[], int num_lonely) {
    DSU* dsu = createDSU(num_nodes);

    // Build connected components
    for (int i = 0; i < num_connections; i++) {
        unionSets(dsu, connections[i][0], connections[i][1]);
    }

    // Find the largest connected component size (not strictly needed for this specific problem, but good practice)
    int max_component_size = 0;
    int* component_sizes = (int*)calloc(num_nodes, sizeof(int)); // Use representative as index

    for (int i = 0; i < num_nodes; i++) {
        component_sizes[find(dsu, i)]++;
    }

    for (int i = 0; i < num_nodes; i++) {
        if (component_sizes[i] > max_component_size) {
            max_component_size = component_sizes[i];
        }
    }
    free(component_sizes);
    free(dsu->parent);
    free(dsu->rank);
    free(dsu);

    // The minimum connections needed is simply the number of lonely nodes.
    return num_lonely;
}

int main() {
    int num_nodes = 10;
    int connections[][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}};
    int num_connections = sizeof(connections) / sizeof(connections[0]);
    int lonely_nodes[] = {10, 11, 12}; // Assuming node numbering can extend
    int num_lonely = sizeof(lonely_nodes) / sizeof(lonely_nodes[0]);

    // Adjust num_nodes if lonely nodes extend beyond initial range
    int max_node = num_nodes - 1;
    for(int i = 0; i < num_lonely; i++) {
        if (lonely_nodes[i] > max_node) {
            max_node = lonely_nodes[i];
        }
    }
    num_nodes = max_node + 1;

    int result = minConnections(num_nodes, connections, num_connections, lonely_nodes, num_lonely);
    printf("Minimum connections required: %d\n", result);

    return 0;
}
