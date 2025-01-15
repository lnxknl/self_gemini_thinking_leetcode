#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STEPS 100 // Limit to prevent infinite loops

typedef struct {
    int position;
    int velocity; // Positive for right, negative for left
} Car;

bool canReachDestination(int road_length, int m_start, int m_end, Car cars[], int num_cars) {
    int m_pos = m_start;
    int time_step = 0;

    while (m_pos != m_end && time_step < MAX_STEPS) {
        time_step++;

        // 1. Move the cars
        for (int i = 0; i < num_cars; i++) {
            cars[i].position += cars[i].velocity;
        }

        // 2. Determine motorcycle's next move (greedy approach)
        int next_m_pos = m_pos;
        if (m_pos < m_end) {
            next_m_pos++;
        } else if (m_pos > m_end) {
            next_m_pos--;
        }

        // 3. Check for potential collision at the next motorcycle position
        bool collision_imminent = false;
        for (int i = 0; i < num_cars; i++) {
            if (cars[i].position == next_m_pos) {
                collision_imminent = true;
                break;
            }
        }

        // 4. Move the motorcycle if no immediate collision
        if (!collision_imminent) {
            m_pos = next_m_pos;
        } else {
            // If collision is imminent, consider staying or moving backward (simple avoidance)
            bool can_stay = true;
            for (int i = 0; i < num_cars; i++) {
                if (cars[i].position == m_pos) {
                    can_stay = false;
                    break;
                }
            }

            if (!can_stay && m_pos > 0) { // Try moving backward if possible
                bool collision_backward = false;
                for (int i = 0; i < num_cars; i++) {
                    if (cars[i].position == m_pos - 1) {
                        collision_backward = true;
                        break;
                    }
                }
                if (!collision_backward) {
                    m_pos--;
                }
            }
        }

        // 5. Check for collision at the current motorcycle position
        for (int i = 0; i < num_cars; i++) {
            if (cars[i].position == m_pos) {
                return false; // Collision occurred
            }
        }
    }

    return m_pos == m_end;
}

int main() {
    int road_length = 10;
    int m_start = 1;
    int m_end = 8;

    Car cars[] = {
        { .position = 4, .velocity = 1 },
        { .position = 6, .velocity = -1 }
    };
    int num_cars = sizeof(cars) / sizeof(cars[0]);

    if (canReachDestination(road_length, m_start, m_end, cars, num_cars)) {
        printf("Motorcycle can reach the destination.\n");
    } else {
        printf("Motorcycle cannot reach the destination.\n");
    }

    return 0;
}
