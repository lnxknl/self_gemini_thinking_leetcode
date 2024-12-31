#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LOCATIONS 10
#define MAX_ITEMS 20
#define MAX_ITEM_NAME_LENGTH 50

// Structure to represent a location and its items
typedef struct {
    char name[MAX_ITEM_NAME_LENGTH];
    char items[MAX_ITEMS][MAX_ITEM_NAME_LENGTH];
    int num_items;
} Location;

// Function to find the location of an item
int find_item_location(Location locations[], int num_locations, const char *item_name, int *location_index) {
    for (int i = 0; i < num_locations; i++) {
        for (int j = 0; j < locations[i].num_items; j++) {
            if (strcmp(locations[i].items[j], item_name) == 0) {
                *location_index = i;
                return 1; // Found
            }
        }
    }
    return 0; // Not found
}

int solve_item_retrieval(Location locations[], int num_locations, char required_items[][MAX_ITEM_NAME_LENGTH], int num_required_items, int retrieval_time, int movement_time) {
    int total_time = 0;
    int current_location_index = -1; // Start at no specific location
    bool retrieved[num_required_items];
    for (int i = 0; i < num_required_items; i++) {
        retrieved[i] = false;
    }

    int remaining_items = num_required_items;

    while (remaining_items > 0) {
        int best_next_item_index = -1;
        int min_time_to_retrieve = -1;

        for (int i = 0; i < num_required_items; i++) {
            if (!retrieved[i]) {
                int target_location_index;
                if (find_item_location(locations, num_locations, required_items[i], &target_location_index)) {
                    int time_to_reach = (current_location_index != -1 && current_location_index != target_location_index) ? movement_time : 0;
                    int current_time_to_retrieve = time_to_reach + retrieval_time;

                    if (best_next_item_index == -1 || current_time_to_retrieve < min_time_to_retrieve) {
                        min_time_to_retrieve = current_time_to_retrieve;
                        best_next_item_index = i;
                        // Update the target location for the best item
                        current_location_index = target_location_index;
                    }
                }
            }
        }

        if (best_next_item_index != -1) {
            total_time += min_time_to_retrieve;
            retrieved[best_next_item_index] = true;
            remaining_items--;
        } else {
            // Should not happen if all required items exist
            break;
        }
    }

    return total_time;
}

int main() {
    // Example Input (based on the image hints)
    Location locations[MAX_LOCATIONS] = {
        {"Shelf", {"Book1", "Decoration", "Hat"}, 3},
        {"Drawer", {"Socks", "Keys"}, 2},
        {"Door Hook", {"Jacket"}, 1}
    };
    int num_locations = 3;

    char required_items[MAX_ITEMS][MAX_ITEM_NAME_LENGTH] = {"Hat", "Keys", "Jacket"};
    int num_required_items = 3;

    int retrieval_time = 2;
    int movement_time = 3;

    int min_time = solve_item_retrieval(locations, num_locations, required_items, num_required_items, retrieval_time, movement_time);

    printf("Minimum time to retrieve all items: %d\n", min_time);

    return 0;
}
