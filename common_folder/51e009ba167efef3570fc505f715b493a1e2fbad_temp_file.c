#include <stdio.h>
#include <stdlib.h>

/*
 * This program solves a network connection problem:
 * - We have a network with some nodes already connected in pairs
 * - We also have some "lonely" nodes that need to be connected to the network
 * - The goal is to find the minimum number of new connections needed
 */

// Structure to manage groups of connected nodes
typedef struct {
    int* parent;  // Array to track parent of each node
    int* rank;    // Array to optimize tree height in union operations
    int size;     // Total number of nodes
} NetworkGroups;

// Function to print connected components
void printNetworkState(NetworkGroups* groups) {
    printf("\nNetwork State:\n");
    // First, find all root nodes and their members
    int* components = (int*)calloc(groups->size, sizeof(int));
    
    // Count members in each component
    for (int i = 0; i < groups->size; i++) {
        int root = findGroup(groups, i);
        components[root]++;
    }

    // Print each component
    for (int i = 0; i < groups->size; i++) {
        if (components[i] > 0) {  // If this is a root node
            printf("Group %d: [", i);
            int first = 1;
            for (int j = 0; j < groups->size; j++) {
                if (findGroup(groups, j) == i) {
                    if (!first) printf(", ");
                    printf("%d", j);
                    first = 0;
                }
            }
            printf("]\n");
        }
    }
    printf("\n");
    free(components);
}

// Create and initialize network groups data structure
NetworkGroups* createNetworkGroups(int total_nodes) {
    NetworkGroups* groups = (NetworkGroups*)malloc(sizeof(NetworkGroups));
    if (!groups) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Allocate arrays for parent and rank
    groups->parent = (int*)malloc(total_nodes * sizeof(int));
    groups->rank = (int*)malloc(total_nodes * sizeof(int));
    groups->size = total_nodes;
    
    if (!groups->parent || !groups->rank) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Initialize: each node starts in its own group
    for (int i = 0; i < total_nodes; i++) {
        groups->parent[i] = i;  // Each node is its own parent
        groups->rank[i] = 0;    // Initial rank is 0
    }

    printf("\nInitial network state - each node in its own group:\n");
    printNetworkState(groups);
    return groups;
}

// Find which group a node belongs to (with path compression)
int findGroup(NetworkGroups* groups, int node) {
    // If node is not its own parent, recursively find the root parent
    if (groups->parent[node] != node) {
        // Path compression: make all nodes in path point directly to root
        groups->parent[node] = findGroup(groups, groups->parent[node]);
    }
    return groups->parent[node];
}

// Connect two nodes into the same group
void connectNodes(NetworkGroups* groups, int node1, int node2) {
    printf("\nConnecting nodes %d and %d:\n", node1, node2);
    
    int group1 = findGroup(groups, node1);
    int group2 = findGroup(groups, node2);

    // If nodes are already in the same group, do nothing
    if (group1 == group2) {
        printf("Nodes %d and %d are already in the same group\n", node1, node2);
        return;
    }

    // Union by rank: attach smaller rank tree under root of higher rank tree
    if (groups->rank[group1] < groups->rank[group2]) {
        groups->parent[group1] = group2;
    }
    else if (groups->rank[group1] > groups->rank[group2]) {
        groups->parent[group2] = group1;
    }
    else {
        // If ranks are equal, make one root point to the other
        groups->parent[group2] = group1;
        groups->rank[group1]++;
    }
    
    printNetworkState(groups);
}

// Calculate minimum connections needed
int calculateMinConnections(int total_nodes, int connections[][2], int num_connections, 
                          int lonely_nodes[], int num_lonely) {
    printf("\n=== Starting Network Construction ===\n");
    
    // Create network groups
    NetworkGroups* groups = createNetworkGroups(total_nodes);

    // Process existing connections
    printf("\n=== Processing Existing Connections ===\n");
    for (int i = 0; i < num_connections; i++) {
        connectNodes(groups, connections[i][0], connections[i][1]);
    }

    // Print lonely nodes
    printf("\n=== Lonely Nodes ===\n[");
    for (int i = 0; i < num_lonely; i++) {
        printf("%d%s", lonely_nodes[i], i < num_lonely - 1 ? ", " : "");
    }
    printf("]\n");

    // Print final state before cleanup
    printf("\n=== Final Network State ===\n");
    printNetworkState(groups);

    // Clean up
    free(groups->parent);
    free(groups->rank);
    free(groups);

    return num_lonely;
}

int main() {
    // Example network setup
    int total_nodes = 10;
    
    // Existing connections in the network (pairs of connected nodes)
    int connections[][2] = {
        {0, 1},  // Node 0 is connected to Node 1
        {2, 3},  // Node 2 is connected to Node 3
        {4, 5},  // and so on...
        {6, 7},
        {8, 9}
    };
    int num_connections = sizeof(connections) / sizeof(connections[0]);

    // Nodes that need to be connected to the network
    int lonely_nodes[] = {10, 11, 12};
    int num_lonely = sizeof(lonely_nodes) / sizeof(lonely_nodes[0]);

    // Adjust total_nodes if lonely nodes have higher numbers
    for (int i = 0; i < num_lonely; i++) {
        if (lonely_nodes[i] >= total_nodes) {
            total_nodes = lonely_nodes[i] + 1;
        }
    }

    printf("=== Network Connection Analysis ===\n");
    printf("Total nodes: %d\n", total_nodes);
    printf("Number of existing connections: %d\n", num_connections);
    printf("Number of lonely nodes: %d\n", num_lonely);

    // Calculate and print result
    int min_connections = calculateMinConnections(total_nodes, connections, 
                                                num_connections, lonely_nodes, num_lonely);
    printf("\n=== Result ===\n");
    printf("Minimum new connections needed: %d\n", min_connections);

    return 0;
}
