#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 500
#define MAX_WORDS 200
#define MAX_KEYWORD_LENGTH 50

// Structure to represent an edge in the graph
typedef struct Edge {
    int to;
    struct Edge* next;
} Edge;

// Structure to represent the adjacency list
typedef struct {
    Edge* head[MAX_NODES + 1];
} Graph;

// Structure to store the keyword to node mapping
typedef struct KeywordMap {
    char keyword[MAX_KEYWORD_LENGTH];
    int node;
    struct KeywordMap* next;
} KeywordMap;

// Function to initialize the graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i <= MAX_NODES; i++) {
        graph->head[i] = NULL;
    }
    return graph;
}

// Function to add a directed edge to the graph
void addEdge(Graph* graph, int from, int to) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->to = to;
    newEdge->next = graph->head[from];
    graph->head[from] = newEdge;
}

// Function to print paths
void printPaths(int** paths, int pathCount, int pathLength, KeywordMap* keywordMap) {
    for (int i = 0; i < pathCount; i++) {
        printf("Path %d: ", i+1);
        for (int j = 0; j < pathLength; j++) {
            KeywordMap* currentMap = keywordMap;
            while (currentMap != NULL) {
                // Convert from 0-based index to 1-based node number
                if (currentMap->node == paths[i][j] + 1) {
                    printf("%s", currentMap->keyword);
                    break;
                }
                currentMap = currentMap->next;
            }
            printf(" -> ");
        }
        // Always print "processing" at the end
        printf("processing\n");
    }
}

// Function to find the longest paths
int findLongestPaths(Graph* graph, KeywordMap* keywordMap, char** monologue, int numWords) {
    if (numWords == 0) {
        return 0;
    }

    int dp[numWords];
    int** paths[numWords];
    int pathCounts[numWords];
    
    // Initialize DP and paths
    for (int i = 0; i < numWords; i++) {
        dp[i] = 1;
        paths[i] = (int**)malloc(sizeof(int*));
        paths[i][0] = (int*)malloc(sizeof(int));
        paths[i][0][0] = i;
        pathCounts[i] = 1;
    }

    for (int i = 1; i < numWords; i++) {
        KeywordMap* currentMap = keywordMap;
        int currentNode = -1;
        while (currentMap != NULL) {
            if (strcmp(currentMap->keyword, monologue[i]) == 0) {
                currentNode = currentMap->node;
                break;
            }
            currentMap = currentMap->next;
        }

        if (currentNode != -1) {
            for (int j = 0; j < i; j++) {
                KeywordMap* prevMap = keywordMap;
                int prevNode = -1;
                while (prevMap != NULL) {
                    if (strcmp(prevMap->keyword, monologue[j]) == 0) {
                        prevNode = prevMap->node;
                        break;
                    }
                    prevMap = prevMap->next;
                }

                if (prevNode != -1) {
                    Edge* currentEdge = graph->head[prevNode];
                    while (currentEdge != NULL) {
                        if (currentEdge->to == currentNode) {
                            if (dp[i] < dp[j] + 1) {
                                dp[i] = dp[j] + 1;
                                // Free old paths
                                for (int k = 0; k < pathCounts[i]; k++) {
                                    free(paths[i][k]);
                                }
                                free(paths[i]);
                                // Copy paths from j
                                pathCounts[i] = pathCounts[j];
                                paths[i] = (int**)malloc(pathCounts[i] * sizeof(int*));
                                for (int k = 0; k < pathCounts[i]; k++) {
                                    paths[i][k] = (int*)malloc(dp[i] * sizeof(int));
                                    memcpy(paths[i][k], paths[j][k], dp[j] * sizeof(int));
                                    paths[i][k][dp[j]] = i;
                                }
                            } else if (dp[i] == dp[j] + 1) {
                                // Add new paths
                                int newCount = pathCounts[i] + pathCounts[j];
                                int** newPaths = (int**)malloc(newCount * sizeof(int*));
                                // Copy existing paths
                                for (int k = 0; k < pathCounts[i]; k++) {
                                    newPaths[k] = (int*)malloc(dp[i] * sizeof(int));
                                    memcpy(newPaths[k], paths[i][k], dp[i] * sizeof(int));
                                }
                                // Add new paths from j
                                for (int k = 0; k < pathCounts[j]; k++) {
                                    newPaths[pathCounts[i] + k] = (int*)malloc(dp[i] * sizeof(int));
                                    memcpy(newPaths[pathCounts[i] + k], paths[j][k], dp[j] * sizeof(int));
                                    newPaths[pathCounts[i] + k][dp[j]] = i;
                                }
                                // Free old paths
                                for (int k = 0; k < pathCounts[i]; k++) {
                                    free(paths[i][k]);
                                }
                                free(paths[i]);
                                // Update paths and count
                                paths[i] = newPaths;
                                pathCounts[i] = newCount;
                            }
                            break;
                        }
                        currentEdge = currentEdge->next;
                    }
                }
            }
        }
    }

    // Find the index of "processing" in monologue
    int processingIndex = -1;
    for (int i = 0; i < numWords; i++) {
        if (strcmp(monologue[i], "processing") == 0) {
            processingIndex = i;
            break;
        }
    }

    // Only consider paths that end at processing
    int maxLength = 0;
    if (processingIndex != -1) {
        maxLength = dp[processingIndex];
    }

    // Collect all paths with max length that end at processing
    int maxPathCount = 0;
    int** maxPaths = NULL;
    
    // First count all valid paths
    for (int i = 0; i < numWords; i++) {
        if (dp[i] == maxLength) {
            // Check if this path ends at processing
            KeywordMap* currentMap = keywordMap;
            int currentNode = -1;
            while (currentMap != NULL) {
                if (strcmp(currentMap->keyword, monologue[i]) == 0) {
                    currentNode = currentMap->node;
                    break;
                }
                currentMap = currentMap->next;
            }
            
            if (currentNode != -1) {
                Edge* currentEdge = graph->head[currentNode];
                while (currentEdge != NULL) {
                    if (currentEdge->to == 3) { // 3 is the node for "processing"
                        maxPathCount += pathCounts[i];
                        break;
                    }
                    currentEdge = currentEdge->next;
                }
            }
        }
    }
    
    if (maxPathCount > 0) {
        maxPaths = (int**)malloc(maxPathCount * sizeof(int*));
        int index = 0;
        for (int i = 0; i < numWords; i++) {
            if (dp[i] == maxLength) {
                // Check if this path ends at processing
                KeywordMap* currentMap = keywordMap;
                int currentNode = -1;
                while (currentMap != NULL) {
                    if (strcmp(currentMap->keyword, monologue[i]) == 0) {
                        currentNode = currentMap->node;
                        break;
                    }
                    currentMap = currentMap->next;
                }
                
                if (currentNode != -1) {
                    Edge* currentEdge = graph->head[currentNode];
                    while (currentEdge != NULL) {
                        if (currentEdge->to == 3) { // 3 is the node for "processing"
                            for (int j = 0; j < pathCounts[i]; j++) {
                                maxPaths[index] = (int*)malloc((maxLength + 1) * sizeof(int));
                                memcpy(maxPaths[index], paths[i][j], maxLength * sizeof(int));
                                maxPaths[index][maxLength] = processingIndex; // Add processing node
                                index++;
                            }
                            break;
                        }
                        currentEdge = currentEdge->next;
                    }
                }
            }
        }
        printPaths(maxPaths, maxPathCount, maxLength, keywordMap);
        
        // Free maxPaths
        for (int i = 0; i < maxPathCount; i++) {
            free(maxPaths[i]);
        }
        free(maxPaths);
    }

    // Free all paths
    for (int i = 0; i < numWords; i++) {
        for (int j = 0; j < pathCounts[i]; j++) {
            free(paths[i][j]);
        }
        free(paths[i]);
    }

    return maxLength;
}

int main() {
    // Example Input
    Graph* graph = createGraph();
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 4, 5);
    addEdge(graph, 5, 3);
    addEdge(graph, 5, 6);
    addEdge(graph, 6, 3);

    KeywordMap* keywordMap = NULL;
    // Assuming simple 1-to-1 mapping for simplicity in C, can be extended for 1-to-many
    // (In a real implementation, a more robust hash table would be preferred)
    KeywordMap* map1 = (KeywordMap*)malloc(sizeof(KeywordMap));
    strcpy(map1->keyword, "network");
    map1->node = 1;
    map1->next = keywordMap;
    keywordMap = map1;

    KeywordMap* map2 = (KeywordMap*)malloc(sizeof(KeywordMap));
    strcpy(map2->keyword, "information");
    map2->node = 2;
    map2->next = keywordMap;
    keywordMap = map2;

    KeywordMap* map3 = (KeywordMap*)malloc(sizeof(KeywordMap));
    strcpy(map3->keyword, "processing");
    map3->node = 3;
    map3->next = keywordMap;
    keywordMap = map3;

    KeywordMap* map4 = (KeywordMap*)malloc(sizeof(KeywordMap));
    strcpy(map4->keyword, "analysis");
    map4->node = 4;
    map4->next = keywordMap;
    keywordMap = map4;

    KeywordMap* map5 = (KeywordMap*)malloc(sizeof(KeywordMap));
    strcpy(map5->keyword, "visualization");
    map5->node = 5;
    map5->next = keywordMap;
    keywordMap = map5;

    KeywordMap* map6 = (KeywordMap*)malloc(sizeof(KeywordMap));
    strcpy(map6->keyword, "interpretation");
    map6->node = 6;
    map6->next = keywordMap;
    keywordMap = map6;

    char* monologue[] = {"network", "information", "processing", "analysis", "visualization", "interpretation", "data"};
    int numWords = sizeof(monologue) / sizeof(monologue[0]);

    int longestPath = findLongestPaths(graph, keywordMap, monologue, numWords);
    printf("Longest path length: %d\n", longestPath);

    // Free allocated memory (important!)
    // ... (Add code to free graph and keyword map)

    return 0;
}
