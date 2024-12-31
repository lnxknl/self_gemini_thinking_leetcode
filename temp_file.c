#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PEOPLE 6 // Assuming a maximum number of people for simplicity

// Structure to represent a person's preferences
typedef struct {
    int preferred_neighbors[MAX_PEOPLE];
    int num_preferred;
    int disliked_neighbors[MAX_PEOPLE];
    int num_disliked;
    bool food_likes[MAX_PEOPLE]; // Assuming food IDs are 0 to MAX_PEOPLE - 1
} PersonPreferences;

// Function to calculate the happiness score of an arrangement
int calculate_happiness(int arrangement[], int n, PersonPreferences preferences[], int num_dishes, int food_served[], bool food_preferences[][MAX_PEOPLE]) {
    int happiness = 0;
    for (int i = 0; i < n; i++) {
        int current_person = arrangement[i];
        int left_neighbor = arrangement[(i - 1 + n) % n];
        int right_neighbor = arrangement[(i + 1) % n];

        // Check neighbor preferences
        bool left_preferred = false;
        for (int j = 0; j < preferences[current_person].num_preferred; j++) {
            if (preferences[current_person].preferred_neighbors[j] == left_neighbor) {
                left_preferred = true;
                break;
            }
        }
        bool right_preferred = false;
        for (int j = 0; j < preferences[current_person].num_preferred; j++) {
            if (preferences[current_person].preferred_neighbors[j] == right_neighbor) {
                right_preferred = true;
                break;
            }
        }

        bool left_disliked = false;
        for (int j = 0; j < preferences[current_person].num_disliked; j++) {
            if (preferences[current_person].disliked_neighbors[j] == left_neighbor) {
                left_disliked = true;
                break;
            }
        }
        bool right_disliked = false;
        for (int j = 0; j < preferences[current_person].disliked_neighbors[j]) {
            if (preferences[current_person].disliked_neighbors[j] == right_neighbor) {
                right_disliked = true;
                break;
            }
        }

        if (left_preferred) happiness++;
        if (right_preferred) happiness++;
        if (left_disliked) happiness--;
        if (right_disliked) happiness--;

        // Check food preferences (simplified: if both like at least one common dish)
        for (int j = 0; j < num_dishes; j++) {
            if (food_preferences[current_person][food_served[j]] && food_preferences[left_neighbor][food_served[j]]) {
                happiness++;
            }
            if (food_preferences[current_person][food_served[j]] && food_preferences[right_neighbor][food_served[j]]) {
                happiness++;
            }
        }
    }
    return happiness;
}

// Backtracking function to find the optimal arrangement
void find_optimal_arrangement(int n, PersonPreferences preferences[], int num_dishes, int food_served[], bool food_preferences[][MAX_PEOPLE], int current_arrangement[], bool placed[], int current_index, int* max_happiness, int best_arrangement[]) {
    if (current_index == n) {
        int current_happiness = calculate_happiness(current_arrangement, n, preferences, num_dishes, food_served, food_preferences);
        if (current_happiness > *max_happiness) {
            *max_happiness = current_happiness;
            for (int i = 0; i < n; i++) {
                best_arrangement[i] = current_arrangement[i];
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!placed[i]) {
            current_arrangement[current_index] = i;
            placed[i] = true;

            // Basic constraint check (can be enhanced for pruning)
            if (current_index > 0) {
                int current_person = current_arrangement[current_index];
                int previous_person = current_arrangement[current_index - 1];
                bool disliked = false;
                for(int j = 0; j < preferences[current_person].num_disliked; j++) {
                    if(preferences[current_person].disliked_neighbors[j] == previous_person) {
                        disliked = true;
                        break;
                    }
                }
                if (!disliked) {
                    find_optimal_arrangement(n, preferences, num_dishes, food_served, food_preferences, current_arrangement, placed, current_index + 1, max_happiness, best_arrangement);
                }
            } else {
                find_optimal_arrangement(n, preferences, num_dishes, food_served, food_preferences, current_arrangement, placed, current_index + 1, max_happiness, best_arrangement);
            }

            placed[i] = false; // Backtrack
        }
    }
}

int main() {
    int n = 6; // Number of people
    PersonPreferences preferences[MAX_PEOPLE];

    // Initialize preferences (Example - needs to be filled with actual data)
    for (int i = 0; i < n; i++) {
        preferences[i].num_preferred = 0;
        preferences[i].num_disliked = 0;
        for(int j=0; j<n; ++j) preferences[i].food_likes[j] = false;
    }

    // Example Preferences (Person 0)
    preferences[0].preferred_neighbors[preferences[0].num_preferred++] = 1;
    preferences[0].preferred_neighbors[preferences[0].num_preferred++] = 2;
    preferences[0].disliked_neighbors[preferences[0].num_disliked++] = 3;
    preferences[0].food_likes[0] = true; // Likes food 0
    preferences[0].food_likes[1] = true; // Likes food 1

    // ... Initialize preferences for other people ...

    int num_dishes = 2;
    int food_served[] = {0, 1}; // Example food IDs
    bool food_preferences_matrix[MAX_PEOPLE][MAX_PEOPLE] = {false}; // Needs initialization

    // Example Food Preferences
    food_preferences_matrix[0][0] = true;
    food_preferences_matrix[0][1] = true;
    food_preferences_matrix[1][0] = true;
    // ... Initialize food preferences for other people ...

    int initial_arrangement[MAX_PEOPLE];
    bool placed[MAX_PEOPLE] = {false};
    int max_happiness = -1000; // Initialize with a very low value
    int best_arrangement[MAX_PEOPLE];

    find_optimal_arrangement(n, preferences, num_dishes, food_served, food_preferences_matrix, initial_arrangement, placed, 0, &max_happiness, best_arrangement);

    printf("Optimal Seating Arrangement:\n");
    for (int i = 0; i < n; i++) {
        printf("Person %d ", best_arrangement[i]);
    }
    printf("\nMaximum Happiness: %d\n", max_happiness);

    return 0;
}
