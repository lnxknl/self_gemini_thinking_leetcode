#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Assume this function is provided
int isStar(int x, int y) {
    // Simulate the star at a specific location (e.g., 10, 15)
    if (x == 10 && y == 15) {
        return 1;
    }
    return 0;
}

// Function to check if a rectangle might contain the star (check corners)
bool mightContainStar(int min_x, int max_x, int min_y, int max_y) {
    if (isStar(min_x, min_y) || isStar(min_x, max_y) || isStar(max_x, min_y) || isStar(max_x, max_y)) {
        return true;
    }
    return false;
}

void findStar(int min_x, int max_x, int min_y, int max_y, int* star_x, int* star_y) {
    // Initialize coordinates
    *star_x = -1;
    *star_y = -1;

    // Base case: area is small enough to check directly
    if (max_x - min_x <= 10 || max_y - min_y <= 10) {
        // Small enough area to check directly
        for (int x = min_x; x <= max_x; x++) {
            for (int y = min_y; y <= max_y; y++) {
                if (isStar(x, y)) {
                    *star_x = x;
                    *star_y = y;
                    return;
                }
            }
        }
        return;
    }

    int mid_x = min_x + (max_x - min_x) / 2;
    int mid_y = min_y + (max_y - min_y) / 2;

    // Check each quadrant, but only if it might contain a star
    if (mightContainStar(min_x, mid_x, min_y, mid_y)) {
        findStar(min_x, mid_x, min_y, mid_y, star_x, star_y);
        if (*star_x != -1) return;
    }
    if (mightContainStar(mid_x + 1, max_x, min_y, mid_y)) {
        findStar(mid_x + 1, max_x, min_y, mid_y, star_x, star_y);
        if (*star_x != -1) return;
    }
    if (mightContainStar(min_x, mid_x, mid_y + 1, max_y)) {
        findStar(min_x, mid_x, mid_y + 1, max_y, star_x, star_y);
        if (*star_x != -1) return;
    }
    if (mightContainStar(mid_x + 1, max_x, mid_y + 1, max_y)) {
        findStar(mid_x + 1, max_x, mid_y + 1, max_y, star_x, star_y);
        if (*star_x != -1) return;
    }
}

int main() {
    int star_x = -1, star_y = -1;

    // 1. Initial Search to find a hit and establish a rough bounding box
    int current_x = 0, current_y = 0;
    bool found_first_hit = false;
    while (!found_first_hit && current_x < 100 && current_y < 100) { // Setting arbitrary initial search limit
        if (isStar(current_x, current_y)) {
            found_first_hit = true;
            break;
        }
        current_x++;
        if (current_x >= 100) {
            current_x = 0;
            current_y++;
        }
    }

    if (found_first_hit) {
        printf("Initial star hit at: (%d, %d)\n", current_x, current_y);

        // 2. Instead of fully determining the bounding box, let's directly use a large search space
        //    knowing the star exists within it. A more sophisticated approach would be to
        //    expand outwards from the initial hit.
        // Initialize coordinates to -1 before search
        star_x = -1;
        star_y = -1;
        findStar(-100, 200, -100, 200, &star_x, &star_y); // Example large search space

        if (star_x != -1) {
            printf("Lone star found at coordinates: (%d, %d)\n", star_x, star_y);
        } else {
            printf("Lone star not found (within the search space).\n");
        }
    } else {
        printf("Could not find any star in the initial search area.\n");
    }

    return 0;
}
