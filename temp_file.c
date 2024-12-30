#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row;
    int col;
} Coordinates;

Coordinates findSourceNode(int** grid, int rows, int cols) {
    Coordinates source = {-1, -1}; // Initialize with invalid coordinates

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 1) {
                source.row = i;
                source.col = j;
                return source;
            }
        }
    }
    return source;
}

int main() {
    // Test Example Input
    int rows = 5;
    int cols = 7;
    int** grid = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        grid[i] = (int*)malloc(cols * sizeof(int));
    }

    // Initialize the grid (representing the network state)
    int initial_grid[5][7] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 2, 2, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 2, 2, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = initial_grid[i][j];
        }
    }

    Coordinates source = findSourceNode(grid, rows, cols);

    if (source.row != -1) {
        printf("Source node found at: (%d, %d)\n", source.row, source.col);
    } else {
        printf("Source node not found.\n");
    }

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}
