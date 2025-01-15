#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_GRID_SIZE 50
#define MAX_STEPS 1000

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
    State items[MAX_GRID_SIZE * MAX_GRID_SIZE * MAX_STEPS]; // A large enough buffer
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

bool isEmpty(Queue *q) {
    return q->front == -1;
}

void enqueue(Queue *q, State s) {
    if (q->rear == MAX_GRID_SIZE * MAX_GRID_SIZE * MAX_STEPS - 1)
        return; // Queue overflow

    if (q->front == -1)
        q->front = 0;

    q->rear++;
    q->items[q->rear] = s;
}

State dequeue(Queue *q) {
    State item = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
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
    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            serialized[idx++] = grid[i][j];
        }
    }
    serialized[idx] = '\0';
    return serialized;
}

bool canEscape(char initialGrid[MAX_GRID_SIZE][MAX_GRID_SIZE], int rows, int cols, Position start, Position exit, int maxSteps) {
    Queue q;
    initQueue(&q);

    // Visited states (using a simple array for demonstration, a hash set would be more efficient for larger grids)
    // Here we are simplifying the visited check by only tracking position for demonstration.
    // A full solution would need to track the grid configuration as well.
    bool visited[MAX_GRID_SIZE][MAX_GRID_SIZE] = {false};

    State initialState;
    memcpy(initialState.grid, initialGrid, sizeof(initialGrid[0][0]) * rows * cols);
    initialState.pos = start;
    initialState.steps = 0;

    enqueue(&q, initialState);
    visited[start.row][start.col] = true;

    while (!isEmpty(&q)) {
        State current = dequeue(&q);

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
                    simulateEvent(nextState.grid, rows, cols);
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
    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE] = {
        {'S', '.', '.', '#'},
        {'.', 'E', '.', '.'},
        {'.', '#', '.', 'T'}
    };
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
