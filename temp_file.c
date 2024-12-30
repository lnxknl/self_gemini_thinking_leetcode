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
} NetworkGroups;

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
    
    if (!groups->parent || !groups->rank) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Initialize: each node starts in its own group
    for (int i = 0; i < total_nodes; i++) {
        groups->parent[i] = i;  // Each node is its own parent
        groups->rank[i] = 0;    // Initial rank is 0
    }

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
    int group1 = findGroup(groups, node1);
    int group2 = findGroup(groups, node2);

    // If nodes are already in the same group, do nothing
    if (group1 == group2) return;

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
}

// Calculate minimum connections needed
int calculateMinConnections(int total_nodes, int connections[][2], int num_connections, 
                          int lonely_nodes[], int num_lonely) {
    // Create network groups
    NetworkGroups* groups = createNetworkGroups(total_nodes);

    // Process existing connections
    for (int i = 0; i < num_connections; i++) {
        connectNodes(groups, connections[i][0], connections[i][1]);
    }

    // Clean up
    free(groups->parent);
    free(groups->rank);
    free(groups);

    // For this specific problem, the minimum connections needed
    // is simply the number of lonely nodes, as each lonely node
    // needs exactly one connection to join the network
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

    // Calculate and print result
    int min_connections = calculateMinConnections(total_nodes, connections, 
                                                num_connections, lonely_nodes, num_lonely);
    printf("Minimum new connections needed: %d\n", min_connections);

    return 0;
}
