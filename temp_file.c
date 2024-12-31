#include <stdio.h>
#include <stdlib.h>

// Structure to represent a node in the adjacency list
struct Node {
    int dest;
    struct Node* next;
};

// Function to create a new node
struct Node* newNode(int dest) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->dest = dest;
    node->next = NULL;
    return node;
}

// Function to perform topological sort
int* findTaskOrder(int numTasks, int** dependencies, int dependenciesSize, int* dependenciesColSize,
                    int** orderingConstraints, int orderingConstraintsSize, int* orderingConstraintsColSize) {
    struct Node** graph = (struct Node**)malloc(numTasks * sizeof(struct Node*));
    int* inDegree = (int*)calloc(numTasks, sizeof(int));
    int* result = (int*)malloc(numTasks * sizeof(int));
    int resultIndex = 0;
    int queue[numTasks];
    int front = 0, rear = 0;

    // Initialize the graph
    for (int i = 0; i < numTasks; i++) {
        graph[i] = NULL;
    }

    // Build the dependency graph and calculate in-degrees
    for (int i = 0; i < dependenciesSize; i++) {
        int src = dependencies[i][0];
        int dest = dependencies[i][1];
        struct Node* node = newNode(dest);
        node->next = graph[src];
        graph[src] = node;
        inDegree[dest]++;
    }

    // Build the ordering constraint graph and update in-degrees
    for (int i = 0; i < orderingConstraintsSize; i++) {
        int before = orderingConstraints[i][0];
        int after = orderingConstraints[i][1];
        struct Node* node = newNode(after);
        node->next = graph[before];
        graph[before] = node;
        inDegree[after]++;
    }

    // Initialize the queue with tasks having an in-degree of 0
    for (int i = 0; i < numTasks; i++) {
        if (inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }

    // Perform topological sort
    while (front < rear) {
        int currentTask = queue[front++];
        result[resultIndex++] = currentTask;

        struct Node* temp = graph[currentTask];
        while (temp != NULL) {
            inDegree[temp->dest]--;
            if (inDegree[temp->dest] == 0) {
                queue[rear++] = temp->dest;
            }
            temp = temp->next;
        }
    }

    // Check for cycles
    if (resultIndex != numTasks) {
        free(result);
        free(graph);
        free(inDegree);
        return NULL; // Indicate no valid order
    }

    free(graph);
    free(inDegree);
    return result;
}

int main() {
    int numTasks = 4;
    int dependencies_data[][2] = {{0, 1}, {1, 3}};
    int* dependencies[2];
    dependencies[0] = dependencies_data[0];
    dependencies[1] = dependencies_data[1];
    int dependenciesSize = 2;
    int dependenciesColSize[] = {2, 2};

    int orderingConstraints_data[][2] = {{2, 3}, {0, 2}};
    int* orderingConstraints[2];
    orderingConstraints[0] = orderingConstraints_data[0];
    orderingConstraints[1] = orderingConstraints_data[1];
    int orderingConstraintsSize = 2;
    int orderingConstraintsColSize[] = {2, 2};

    int* taskOrder = findTaskOrder(numTasks, dependencies, dependenciesSize, dependenciesColSize,
                                    orderingConstraints, orderingConstraintsSize, orderingConstraintsColSize);

    if (taskOrder != NULL) {
        printf("Valid Task Order: ");
        for (int i = 0; i < numTasks; i++) {
            printf("%d ", taskOrder[i]);
        }
        printf("\n");
        free(taskOrder);
    } else {
        printf("No valid task order exists.\n");
    }

    return 0;
}
