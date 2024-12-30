#include <stdio.h>
#include <stdbool.h>

// Assume these represent the grid dimensions
#define GRID_WIDTH 1000
#define GRID_HEIGHT 1000

// Simulated sensor function (in a real scenario, this would interact with the environment)
bool is_active(int x, int y) {
    // In our simulated case, let's assume the beacon is at (500, 500)
    return (x == 500 && y == 500);
}

// Function to check if any activity exists within a given rectangle
bool has_activity(int min_x, int min_y, int max_x, int max_y) {
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            if (is_active(x, y)) {
                return true;
            }
        }
    }
    return false;
}

// Recursive function to find the active beacon
bool find_beacon(int min_x, int min_y, int max_x, int max_y, int *beacon_x, int *beacon_y) {
    // Base case: Single cell
    if (min_x == max_x && min_y == max_y) {
        if (is_active(min_x, min_y)) {
            *beacon_x = min_x;
            *beacon_y = min_y;
            return true;
        }
        return false;
    }

    int mid_x = min_x + (max_x - min_x) / 2;
    int mid_y = min_y + (max_y - min_y) / 2;

    // Quadrant 1 (top-left)
    if (has_activity(min_x, min_y, mid_x, mid_y)) {
        if (find_beacon(min_x, min_y, mid_x, mid_y, beacon_x, beacon_y)) return true;
    }

    // Quadrant 2 (top-right)
    if (has_activity(mid_x + 1, min_y, max_x, mid_y)) {
        if (find_beacon(mid_x + 1, min_y, max_x, mid_y, beacon_x, beacon_y)) return true;
    }

    // Quadrant 3 (bottom-left)
    if (has_activity(min_x, mid_y + 1, mid_x, max_y)) {
        if (find_beacon(min_x, mid_y + 1, mid_x, max_y, beacon_x, beacon_y)) return true;
    }

    // Quadrant 4 (bottom-right)
    if (has_activity(mid_x + 1, mid_y + 1, max_x, max_y)) {
        if (find_beacon(mid_x + 1, mid_y + 1, max_x, max_y, beacon_x, beacon_y)) return true;
    }

    return false;
}

int main() {
    int beacon_x = -1;
    int beacon_y = -1;

    if (find_beacon(0, 0, GRID_WIDTH - 1, GRID_HEIGHT - 1, &beacon_x, &beacon_y)) {
        printf("Beacon found at coordinates: (%d, %d)\n", beacon_x, beacon_y);
    } else {
        printf("Beacon not found.\n");
    }

    return 0;
}
