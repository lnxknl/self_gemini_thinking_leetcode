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
typedef struct {
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

// Function to find the longest path
int findLongestPath(Graph* graph, KeywordMap* keywordMap, char** monologue, int numWords) {
    if (numWords == 0) {
        return 0;
    }

    int dp[numWords]; // dp[i] stores the length of the longest path ending with the i-th word
    for (int i = 0; i < numWords; i++) {
        dp[i] = 1; // Minimum length is 1 (the word itself)
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
                            }
                            break;
                        }
                        currentEdge = currentEdge->next;
                    }
                }
            }
        }
    }

    int maxLength = 0;
    for (int i = 0; i < numWords; i++) {
        if (dp[i] > maxLength) {
            maxLength = dp[i];
        }
    }
    return maxLength;
}

int main() {
    // Example Input
    Graph* graph = createGraph();
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 4, 3);

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

    char* monologue[] = {"network", "information", "processing", "data"};
    int numWords = sizeof(monologue) / sizeof(monologue[0]);

    int longestPath = findLongestPath(graph, keywordMap, monologue, numWords);
    printf("Longest path length: %d\n", longestPath); // Expected output: 3

    // Free allocated memory (important!)
    // ... (Add code to free graph and keyword map)

    return 0;
}
