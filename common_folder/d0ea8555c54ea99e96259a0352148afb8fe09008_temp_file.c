#include <stdio.h>
#include <stdbool.h>

// Assume this function interacts with the "image"
// For testing purposes, we'll simulate it.
bool is_bright(int x, int y) {
    // Replace these with the actual coordinates of the bright pixel
    int bright_x = 5;
    int bright_y = -3;
    return (x == bright_x && y == bright_y);
}

// Function to find the bright pixel
void find_bright_pixel(int *bright_x, int *bright_y) {
    int x = 0, y = 0;
    int step = 1;

    // Initial check
    if (is_bright(x, y)) {
        *bright_x = x;
        *bright_y = y;
        return;
    }

    while (true) {
        // Move right
        for (int i = 0; i < step; ++i) {
            x++;
            if (is_bright(x, y)) {
                *bright_x = x;
                *bright_y = y;
                return;
            }
        }

        // Move up
        for (int i = 0; i < step; ++i) {
            y--;
            if (is_bright(x, y)) {
                *bright_x = x;
                *bright_y = y;
                return;
            }
        }

        // Move left
        for (int i = 0; i < step; ++i) {
            x--;
            if (is_bright(x, y)) {
                *bright_x = x;
                *bright_y = y;
                return;
            }
        }

        // Move down
        for (int i = 0; i < step; ++i) {
            y++;
            if (is_bright(x, y)) {
                *bright_x = x;
                *bright_y = y;
                return;
            }
        }
        step++;
    }
}

int main() {
    int bright_x, bright_y;
    find_bright_pixel(&bright_x, &bright_y);
    printf("The bright pixel is at coordinates (%d, %d)\n", bright_x, bright_y);
    return 0;
}
