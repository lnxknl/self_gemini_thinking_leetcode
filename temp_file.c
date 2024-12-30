#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <uthash.h> // Using uthash for hash table

// Structure to represent coordinates
typedef struct {
    int x;
    int y;
    UT_hash_handle hh;
} Coordinate;

// Hash table to store visited coordinates
Coordinate *visited = NULL;

// Queue structure for BFS
typedef struct QueueNode {
    int x;
    int y;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, int x, int y) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

bool isEmpty(Queue* q) {
    return q->front == NULL;
}

QueueNode* dequeue(Queue* q) {
    if (isEmpty(q)) return NULL;
    QueueNode* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

// Function to check if coordinates are already visited
bool isVisited(int x, int y) {
    Coordinate *c;
    HASH_FIND(hh, visited, &x, sizeof(int), c);
    if (c) {
        return true;
    } else {
        return false;
    }
}

// Function to mark coordinates as visited
void markVisited(int x, int y) {
    Coordinate *c = (Coordinate*)malloc(sizeof(Coordinate));
    c->x = x;
    c->y = y;
    HASH_ADD(hh, visited, &c->x, sizeof(int), c);
}

// Simulated check function (for testing)
bool check(int x, int y, int signal_x, int signal_y) {
    return x == signal_x && y == signal_y;
}

// Function to find the signal
bool findSignal(int* signal_x_out, int* signal_y_out, int signal_x_target, int signal_y_target) {
    Queue* q = createQueue();
    enqueue(q, 0, 0);
    markVisited(0, 0);

    while (!isEmpty(q)) {
        QueueNode* current = dequeue(q);
        int x = current->x;
        int y = current->y;
        free(current);

        if (check(x, y, signal_x_target, signal_y_target)) {
            *signal_x_out = x;
            *signal_y_out = y;
            return true;
        }

        // Explore neighbors
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (!isVisited(nx, ny)) {
                enqueue(q, nx, ny);
                markVisited(nx, ny);
            }
        }
    }

    return false; // Signal not found (should not happen with the problem constraints)
}

int main() {
    int signal_x = -5;
    int signal_y = 3;
    int found_x, found_y;

    if (findSignal(&found_x, &found_y, signal_x, signal_y)) {
        printf("Signal found at coordinates: (%d, %d)\n", found_x, found_y);
    } else {
        printf("Signal not found.\n");
    }

    // Free allocated memory (visited hash table)
    Coordinate *current, *tmp;
    HASH_ITER(hh, visited, current, tmp) {
        HASH_DEL(visited, current);
        free(current);
    }

    return 0;
}
