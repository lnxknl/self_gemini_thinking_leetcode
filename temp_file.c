#include <stdio.h>
#include <stdbool.h>

// Assume getPixel is a provided function
int getPixel(int x, int y) {
    // In a real implementation, this would interact with the data source.
    // For testing, we can simulate the white pixel's location.
    static int white_x = 5;
    static int white_y = -3;
    if (x == white_x && y == white_y) {
        return 1;
    }
    return 0;
}

typedef struct {
    int x;
    int y;
} Point;

Point findWhitePixel() {
    int step = 1;
    int x = 0, y = 0;

    // Phase 1: Expand outwards to find a bounding box
    while (true) {
        if (getPixel(x + step, y)) {
            return (Point){x + step, y};
        }
        if (getPixel(x - step, y)) {
            return (Point){x - step, y};
        }
        if (getPixel(x, y + step)) {
            return (Point){x, y + step};
        }
        if (getPixel(x, y - step)) {
            return (Point){x, y - step};
        }
        if (getPixel(x + step, y + step)) {
            return (Point){x + step, y + step};
        }
        if (getPixel(x - step, y + step)) {
            return (Point){x - step, y + step};
        }
        if (getPixel(x + step, y - step)) {
            return (Point){x + step, y - step};
        }
        if (getPixel(x - step, y - step)) {
            return (Point){x - step, y - step};
        }
        step *= 2; // Exponentially increase the search radius
        if (step > 1000) break; // Optional: Add a safety break to prevent infinite loops in simulation
    }

    // Phase 2: More refined search (can be implemented with binary search principles)
    // This is a simplified approach for demonstration. A more robust solution
    // would involve tracking search boundaries and using binary search.
    for (int current_step_x = -step; current_step_x <= step; ++current_step_x) {
        for (int current_step_y = -step; current_step_y <= step; ++current_step_y) {
            if (getPixel(current_step_x, current_step_y)) {
                return (Point){current_step_x, current_step_y};
            }
        }
    }

    return (Point){-1, -1}; // Should not reach here if a white pixel exists
}

int main() {
    Point white_loc = findWhitePixel();
    if (white_loc.x != -1) {
        printf("White pixel found at coordinates: (%d, %d)\n", white_loc.x, white_loc.y);
    } else {
        printf("White pixel not found.\n");
    }
    return 0;
}
