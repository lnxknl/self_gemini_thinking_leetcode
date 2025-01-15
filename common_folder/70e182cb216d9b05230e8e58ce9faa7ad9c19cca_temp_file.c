#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_GRID_SIZE 50
#define MAX_STEPS 1000
#define QUEUE_CAPACITY (MAX_GRID_SIZE * MAX_GRID_SIZE * MAX_STEPS)

// Structure to represent a position
typedef struct {
    int row;
    int col;
} Position;

// Structure to represent a state (grid + position)
typedef struct {
    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    Position pos;
    int steps;
} State;

// Structure for the queue
typedef struct {
    State items[QUEUE_CAPACITY];
    int size;
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
    q->size = 0;
}

bool isEmpty(Queue *q) {
    return q->front == -1;
}

void enqueue(Queue *q, State s) {
    if (q->size >= QUEUE_CAPACITY) {
        fprintf(stderr, "Queue overflow\n");
        exit(1);
    }

    if (isEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    }
    
    q->items[q->rear] = s;
    q->size++;
    printf("Enqueued at %d, size: %d\n", q->rear, q->size); // Debug output
}

State dequeue(Queue *q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue underflow\n");
        exit(1);
    }

    State item = q->items[q->front];
    printf("Dequeued from %d, size: %d\n", q->front, q->size); // Debug output
    
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % QUEUE_CAPACITY;
    }
    q->size--;
    return item;
}

// Function to simulate the event's effect (example - replace 'E' with '#')
void simulateEvent(char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 'E') {
                grid[i][j] = '#';
            }
        }
    }
}

// Function to serialize the grid (for visited set)
char* serializeGrid(char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols) {
    char *serialized = (char *)malloc(rows * cols + 1);
    if (!serialized) {
        return NULL;
    }
    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            serialized[idx++] = grid[i][j];
        }
    }
    serialized[idx] = '\0';
    char *result = strdup(serialized);
    free(serialized);
    return result;
}

bool canEscape(char initialGrid[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols, Position start, Position exit, int maxSteps) {
    printf("Entering canEscape\n");
    printf("Rows: %d, Cols: %d\n", rows, cols);
    printf("Start: (%d, %d), Exit: (%d, %d)\n", start.row, start.col, exit.row, exit.col);
    
    Queue q = {0};
    initQueue(&q);
    printf("Queue initialized at address: %p\n", (void*)&q);

    // Visited states (using a simple array for demonstration, a hash set would be more efficient for larger grids)
    // Here we are simplifying the visited check by only tracking position for demonstration.
    // A full solution would need to track the grid configuration as well.
    bool visited[MAX_GRID_SIZE][MAX_GRID_SIZE] = {0};
    printf("Visited array initialized\n");

    State initialState;
    printf("Creating initial state\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            initialState.grid[i][j] = initialGrid[i][j];
        }
    }
    initialState.pos = start;
    initialState.steps = 0;
    printf("Initial state created\n");

    enqueue(&q, initialState);
    printf("Initial state enqueued\n");
    visited[start.row][start.col] = true;
    printf("Start position marked as visited\n");

    while (!isEmpty(&q)) {
        printf("Processing next state\n");
        State current = dequeue(&q);
        printf("Dequeued state at position (%d, %d)\n", current.pos.row, current.pos.col);

        if (current.pos.row == exit.row && current.pos.col == exit.col) {
            return true;
        }

        if (current.steps >= maxSteps) {
            continue;
        }

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int newRow = current.pos.row + dr[i];
            int newCol = current.pos.col + dc[i];

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && current.grid[newRow][newCol] != '#') {
                State nextState = current;
                nextState.pos.row = newRow;
                nextState.pos.col = newCol;
                nextState.steps++;

                if (current.grid[newRow][newCol] == 'E') {
                    // Create a copy of the grid before simulating the event
                    char newGrid[MAX_GRID_SIZE][MAX_GRID_SIZE];
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) {
                            newGrid[i][j] = nextState.grid[i][j];
                        }
                    }
                    simulateEvent(newGrid, rows, cols);
                    memcpy(nextState.grid, newGrid, sizeof(char) * rows * cols);
                }

                if (!visited[nextState.pos.row][nextState.pos.col]) { // Simplified visited check
                    visited[nextState.pos.row][nextState.pos.col] = true;
                    enqueue(&q, nextState);
                }
            }
        }
    }

    return false;
}

int main() {
    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    memset(grid, '.', sizeof(grid)); // Initialize all cells to '.'
    
    // Set specific cells
    grid[0][0] = 'S';
    grid[0][3] = '#';
    grid[1][1] = 'E';
    grid[2][1] = '#';
    grid[2][3] = 'T';
    
    printf("Grid initialized successfully\n"); // Debug output
    int rows = 3;
    int cols = 4;
    Position start = {0, 0};
    Position exit = {2, 3};
    int maxSteps = 10;

    if (canEscape(grid, rows, cols, start, exit, maxSteps)) {
        printf("Can escape!\n");
    } else {
        printf("Cannot escape.\n");
    }

    return 0;
}
