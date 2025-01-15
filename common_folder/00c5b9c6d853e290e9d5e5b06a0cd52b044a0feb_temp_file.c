#include <stdio.h>
#include <stdbool.h>

// Assume this function is provided and handles querying the grid
int getColor(int x, int y) {
    // In a real scenario, this would interact with the grid representation
    // For testing purposes, we'll define the white pixel location here
    if (x == 5 && y == 5) {
        return 1;
    }
    return 0;
}

bool containsWhitePixel(int min_x, int max_x, int min_y, int max_y) {
    // Check a few points within the region to see if the white pixel exists.
    // This can be optimized, but for simplicity, we check a corner.
    if (getColor(min_x, min_y) == 1) return true;
    return false;
}

void findWhitePixel(int min_x, int max_x, int min_y, int max_y, int* result_x, int* result_y) {
    if (min_x == max_x && min_y == max_y) {
        *result_x = min_x;
        *result_y = min_y;
        return;
    }

    int mid_x = min_x + (max_x - min_x) / 2;
    int mid_y = min_y + (max_y - min_y) / 2;

    // Top-Left
    if (containsWhitePixel(min_x, mid_x, min_y, mid_y)) {
        findWhitePixel(min_x, mid_x, min_y, mid_y, result_x, result_y);
        return;
    }

    // Top-Right
    if (max_x > mid_x && containsWhitePixel(mid_x + 1, max_x, min_y, mid_y)) {
        findWhitePixel(mid_x + 1, max_x, min_y, mid_y, result_x, result_y);
        return;
    }

    // Bottom-Left
    if (max_y > mid_y && containsWhitePixel(min_x, mid_x, mid_y + 1, max_y)) {
        findWhitePixel(min_x, mid_x, mid_y + 1, max_y, result_x, result_y);
        return;
    }

    // Bottom-Right
    if (max_x > mid_x && max_y > mid_y && containsWhitePixel(mid_x + 1, max_x, mid_y + 1, max_y)) {
        findWhitePixel(mid_x + 1, max_x, mid_y + 1, max_y, result_x, result_y);
        return;
    }
}

int main() {
    int white_x = -1, white_y = -1;
    // Assume we have some initial knowledge of the bounds, or we perform an initial search.
    // For this example, let's assume a reasonable bound.
    findWhitePixel(0, 10, 0, 10, &white_x, &white_y);

    if (white_x != -1) {
        printf("White pixel found at coordinates: (%d, %d)\n", white_x, white_y);
    } else {
        printf("White pixel not found.\n");
    }

    return 0;
}
