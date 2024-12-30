#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define the dimensions of the image (or grid)
#define ROWS 10
#define COLS 10

// Define the value representing the white object pixel
#define WHITE 1
#define BLACK 0

// Structure to represent a point (coordinate)
typedef struct {
    int row;
    int col;
} Point;

// Function to check if a point is within the grid boundaries
bool is_valid(int row, int col) {
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Depth-First Search (DFS) to find connected components
void dfs(int row, int col, int grid[ROWS][COLS], bool visited[ROWS][COLS], Point* component, int* component_size) {
    if (!is_valid(row, col) || visited[row][col] || grid[row][col] == BLACK) {
        return;
    }

    visited[row][col] = true;
    component[*component_size] = (Point){row, col};
    (*component_size)++;

    // Explore adjacent pixels
    dfs(row + 1, col, grid, visited, component, component_size);
    dfs(row - 1, col, grid, visited, component, component_size);
    dfs(row, col + 1, grid, visited, component, component_size);
    dfs(row, col - 1, grid, visited, component, component_size);
}

// Function to find the single white object in the grid
Point* find_white_object(int grid[ROWS][COLS], int* object_size) {
    bool visited[ROWS][COLS] = {false};
    Point* object_pixels = NULL;
    int max_component_size = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == WHITE && !visited[i][j]) {
                Point* current_component = (Point*)malloc(ROWS * COLS * sizeof(Point));
                int current_component_size = 0;
                dfs(i, j, grid, visited, current_component, &current_component_size);

                if (current_component_size > max_component_size) {
                    if (object_pixels != NULL) {
                        free(object_pixels);
                    }
                    object_pixels = current_component;
                    max_component_size = current_component_size;
                    *object_size = current_component_size;
                } else {
                    free(current_component);
                }
            }
        }
    }

    if (max_component_size == 0) {
        return NULL; // No white object found
    }

    return object_pixels;
}

int main() {
    // Test example input mimicking the image (single white pixel)
    int grid[ROWS][COLS] = {
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
    };

    int object_size;
    Point* object_pixels = find_white_object(grid, &object_size);

    if (object_pixels != NULL) {
        printf("White object found with %d pixels:\n", object_size);
        for (int i = 0; i < object_size; i++) {
            printf("(%d, %d) ", object_pixels[i].row, object_pixels[i].col);
        }
        printf("\n");
        free(object_pixels);
    } else {
        printf("No white object found.\n");
    }

    // Test example with a more complex shape
    int grid2[ROWS][COLS] = {
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}
    };

    object_pixels = find_white_object(grid2, &object_size);

    if (object_pixels != NULL) {
        printf("\nWhite object found with %d pixels in grid2:\n", object_size);
        for (int i = 0; i < object_size; i++) {
            printf("(%d, %d) ", object_pixels[i].row, object_pixels[i].col);
        }
        printf("\n");
        free(object_pixels);
    } else {
        printf("No white object found in grid2.\n");
    }

    // Test example with multiple small white pixels (potential noise) and one larger object
    int grid3[ROWS][COLS] = {
        {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, WHITE, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE}
    };

    object_pixels = find_white_object(grid3, &object_size);

    if (object_pixels != NULL) {
        printf("\nWhite object found with %d pixels in grid3:\n", object_size);
        for (int i = 0; i < object_size; i++) {
            printf("(%d, %d) ", object_pixels[i].row, object_pixels[i].col);
        }
        printf("\n");
        free(object_pixels);
    } else {
        printf("No white object found in grid3.\n");
    }

    return 0;
}
