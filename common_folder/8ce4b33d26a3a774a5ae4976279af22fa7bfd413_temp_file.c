#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent a coordinate
typedef struct {
    int x;
    int y;
} Coordinate;

// Structure for the queue
typedef struct QueueNode {
    Coordinate coord;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Function to initialize the queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        perror("Failed to allocate memory for queue");
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Function to check if the queue is empty
bool isEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Function to enqueue a coordinate
void enqueue(Queue* queue, Coordinate coord) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for queue node");
        exit(EXIT_FAILURE);
    }
    newNode->coord = coord;
    newNode->next = NULL;
    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue a coordinate
Coordinate dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Attempted to dequeue from an empty queue\n");
        exit(EXIT_FAILURE);
    }
    QueueNode* temp = queue->front;
    Coordinate coord = temp->coord;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return coord;
}

// Structure for a hash set node (for visited cells)
typedef struct HashSetNode {
    Coordinate coord;
    struct HashSetNode* next;
} HashSetNode;

// Function to create a hash key for a coordinate
unsigned int hashCoordinate(Coordinate coord) {
    // A simple hash function
    return (unsigned int)(coord.x * 31 + coord.y);
}

// Function to insert a coordinate into the hash set
bool insertHashSet(HashSetNode** hashTable, Coordinate coord, int tableSize) {
    unsigned int index = hashCoordinate(coord) % tableSize;
    HashSetNode* current = hashTable[index];
    while (current != NULL) {
        if (current->coord.x == coord.x && current->coord.y == coord.y) {
            return false; // Already exists
        }
        current = current->next;
    }
    HashSetNode* newNode = (HashSetNode*)malloc(sizeof(HashSetNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for hash set node");
        exit(EXIT_FAILURE);
    }
    newNode->coord = coord;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    return true;
}

// Function to solve the "Lost in the Dark Maze" problem
int minSteps(int start_x, int start_y, int target_x, int target_y) {
    Queue* queue = createQueue();
    int tableSize = 1000; // Choose an appropriate size
    HashSetNode** visited = (HashSetNode**)calloc(tableSize, sizeof(HashSetNode*));
    if (visited == NULL) {
        perror("Failed to allocate memory for visited set");
        exit(EXIT_FAILURE);
    }

    Coordinate start = {start_x, start_y};
    enqueue(queue, start);
    insertHashSet(visited, start, tableSize);

    int steps = 0;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!isEmpty(queue)) {
        int levelSize = 0;
        QueueNode* current_node = queue->front;
        while(current_node != NULL) {
            levelSize++;
            current_node = current_node->next;
        }

        for (int i = 0; i < levelSize; i++) {
            Coordinate current = dequeue(queue);

            if (current.x == target_x && current.y == target_y) {
                // Free allocated memory
                while (!isEmpty(queue)) dequeue(queue);
                free(queue);
                for (int j = 0; j < tableSize; j++) {
                    HashSetNode* node = visited[j];
                    while (node != NULL) {
                        HashSetNode* temp = node;
                        node = node->next;
                        free(temp);
                    }
                }
                free(visited);
                return steps;
            }

            for (int j = 0; j < 4; j++) {
                Coordinate next = {current.x + dx[j], current.y + dy[j]};
                if (insertHashSet(visited, next, tableSize)) {
                    enqueue(queue, next);
                }
            }
        }
        steps++;
    }

    // Free allocated memory if target is unreachable
    free(queue);
    for (int j = 0; j < tableSize; j++) {
        HashSetNode* node = visited[j];
        while (node != NULL) {
            HashSetNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(visited);
    return -1;
}

int main() {
    // Test Case 1: Simple case
    int start_x1 = 0, start_y1 = 0;
    int target_x1 = 3, target_y1 = 3;
    printf("Minimum steps from (%d, %d) to (%d, %d): %d\n", start_x1, start_y1, target_x1, target_y1, minSteps(start_x1, start_y1, target_x1, target_y1)); // Expected: 3

    // Test Case 2: Target unreachable (in a practical sense, always reachable in an infinite grid)
    int start_x2 = 0, start_y2 = 0;
    int target_x2 = -5, target_y2 = 0;
    printf("Minimum steps from (%d, %d) to (%d, %d): %d\n", start_x2, start_y2, target_x2, target_y2, minSteps(start_x2, start_y2, target_x2, target_y2)); // Expected: 5

    // Test Case 3: Same start and target
    int start_x3 = 2, start_y3 = 2;
    int target_x3 = 2, target_y3 = 2;
    printf("Minimum steps from (%d, %d) to (%d, %d): %d\n", start_x3, start_y3, target_x3, target_y3, minSteps(start_x3, start_y3, target_x3, target_y3)); // Expected: 0

    return 0;
}
