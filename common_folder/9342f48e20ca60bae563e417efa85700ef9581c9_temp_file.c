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

bool findStar(int min_x, int max_x, int min_y, int max_y, int* star_x, int* star_y) {
    // Initialize star coordinates to -1
    *star_x = -1;
    *star_y = -1;

    // Base case: single point to check
    if (min_x == max_x && min_y == max_y) {
        if (isStar(min_x, min_y)) {
            *star_x = min_x;
            *star_y = min_y;
            return true;
        }
        return false;
    }

    int mid_x = min_x + (max_x - min_x) / 2;
    int mid_y = min_y + (max_y - min_y) / 2;

    // Check each quadrant
    if (mightContainStar(min_x, mid_x, min_y, mid_y)) {
        if (findStar(min_x, mid_x, min_y, mid_y, star_x, star_y)) {
            return true;
        }
    }
    if (mightContainStar(mid_x + 1, max_x, min_y, mid_y)) {
        if (findStar(mid_x + 1, max_x, min_y, mid_y, star_x, star_y)) {
            return true;
        }
    }
    if (mightContainStar(min_x, mid_x, mid_y + 1, max_y)) {
        if (findStar(min_x, mid_x, mid_y + 1, max_y, star_x, star_y)) {
            return true;
        }
    }
    if (mightContainStar(mid_x + 1, max_x, mid_y + 1, max_y)) {
        if (findStar(mid_x + 1, max_x, mid_y + 1, max_y, star_x, star_y)) {
            return true;
        }
    }

    return false;
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
        if (findStar(-100, 200, -100, 200, &star_x, &star_y)) { // Example large search space

            printf("Lone star found at coordinates: (%d, %d)\n", star_x, star_y);
        } else {
            printf("Lone star not found (within the search space).\n");
        }
    } else {
        printf("Could not find any star in the initial search area.\n");
    }

    return 0;
}
