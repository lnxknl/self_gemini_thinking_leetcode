#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Assume this is the external function provided
bool is_white(int x, int y) {
    // In a real scenario, this would interact with the image data.
    // For testing, let's assume the white pixel is at (5, 5).
    return x == 5 && y == 5;
}

typedef struct {
    int min_x, max_x, min_y, max_y;
} BoundingBox;

typedef struct {
    int x, y;
} Point;

Point find_white_pixel_recursive(BoundingBox box) {
    if (box.min_x == box.max_x && box.min_y == box.max_y) {
        return (Point){box.min_x, box.min_y};
    }

    int mid_x = box.min_x + (box.max_x - box.min_x) / 2;
    int mid_y = box.min_y + (box.max_y - box.min_y) / 2;

    BoundingBox quadrants[4];
    quadrants[0] = (BoundingBox){box.min_x, mid_x, box.min_y, mid_y}; // Top-left
    quadrants[1] = (BoundingBox){mid_x + 1, box.max_x, box.min_y, mid_y}; // Top-right
    quadrants[2] = (BoundingBox){box.min_x, mid_x, mid_y + 1, box.max_y}; // Bottom-left
    quadrants[3] = (BoundingBox){mid_x + 1, box.max_x, mid_y + 1, box.max_y}; // Bottom-right

    for (int i = 0; i < 4; ++i) {
        // Optimization: Check corners of the quadrant before recursive call
        if (is_white(quadrants[i].min_x, quadrants[i].min_y) ||
            is_white(quadrants[i].max_x, quadrants[i].min_y) ||
            is_white(quadrants[i].min_x, quadrants[i].max_y) ||
            is_white(quadrants[i].max_x, quadrants[i].max_y) ||
            (quadrants[i].max_x - quadrants[i].min_x <= 1 && quadrants[i].max_y - quadrants[i].min_y <= 1 &&
             (is_white(quadrants[i].min_x, quadrants[i].min_y) ||
              (quadrants[i].max_x > quadrants[i].min_x && is_white(quadrants[i].max_x, quadrants[i].min_y)) ||
              (quadrants[i].max_y > quadrants[i].min_y && is_white(quadrants[i].min_x, quadrants[i].max_y)) ||
              (quadrants[i].max_x > quadrants[i].min_x && quadrants[i].max_y > quadrants[i].min_y && is_white(quadrants[i].max_x, quadrants[i].max_y)))))
         {
            return find_white_pixel_recursive(quadrants[i]);
        }
    }

    // Should not happen if there's exactly one white pixel
    return (Point){-1, -1};
}

Point find_white_pixel() {
    // Initial bounding box expansion
    BoundingBox initial_box = {-1, 1, -1, 1};
    while (true) {
        bool found = false;
        for (int x = initial_box.min_x; x <= initial_box.max_x; ++x) {
            for (int y = initial_box.min_y; y <= initial_box.max_y; ++y) {
                if (is_white(x, y)) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) {
            return find_white_pixel_recursive(initial_box);
        } else {
            initial_box.min_x *= 2;
            initial_box.max_x *= 2;
            initial_box.min_y *= 2;
            initial_box.max_y *= 2;
            if (initial_box.min_x > initial_box.max_x || initial_box.min_y > initial_box.max_y) {
                 initial_box.min_x = -abs(initial_box.max_x) -1;
                 initial_box.min_y = -abs(initial_box.max_y) -1;
                 initial_box.max_x = abs(initial_box.max_x) + 1;
                 initial_box.max_y = abs(initial_box.max_y) + 1;
            }
        }
    }
}

int main() {
    Point white_pixel = find_white_pixel();
    printf("White pixel found at (%d, %d)\n", white_pixel.x, white_pixel.y);
    return 0;
}
