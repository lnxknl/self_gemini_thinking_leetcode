#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define the Sheep structure
typedef struct {
    int id;
    int activity_level;
    char color[50];
    char pattern[50];
} Sheep;

// Function to check if a string is present in an array (simple hash set for this example)
bool is_present(const char *str, const char *arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] != NULL && strcmp(str, arr[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a sheep is calm based on the criteria
bool is_calm(const Sheep *sheep, int max_activity_level, const char *allowed_colors[], int num_colors, const char *allowed_patterns[], int num_patterns) {
    return sheep->activity_level <= max_activity_level &&
           is_present(sheep->color, allowed_colors, num_colors) &&
           is_present(sheep->pattern, allowed_patterns, num_patterns);
}

int main() {
    // Example sheep data
    Sheep sheep_stream[] = {
        {1, 3, "white", "plain"},
        {2, 7, "black", "striped"},
        {3, 2, "white", "spotted"},
        {4, 5, "pink", "plain"},
        {5, 1, "white", "plain"}
    };
    int num_sheep = sizeof(sheep_stream) / sizeof(sheep_stream[0]);

    // Initial criteria
    int max_activity_level = 4;
    const char *allowed_colors[] = {"white", "pink"};
    int num_allowed_colors = 2;
    const char *allowed_patterns[] = {"plain"};
    int num_allowed_patterns = 1;

    int calm_sheep_count = 0;

    printf("Processing sheep with initial criteria:\n");
    printf("Max Activity: %d, Allowed Colors: [%s, %s], Allowed Patterns: [%s]\n",
           max_activity_level, allowed_colors[0], allowed_colors[1], allowed_patterns[0]);

    // Process the sheep stream
    for (int i = 0; i < num_sheep; i++) {
        if (is_calm(&sheep_stream[i], max_activity_level, allowed_colors, num_allowed_colors, allowed_patterns, num_allowed_patterns)) {
            calm_sheep_count++;
            printf("Sheep %d (activity: %d, color: %s, pattern: %s) is calm.\n",
                   sheep_stream[i].id, sheep_stream[i].activity_level, sheep_stream[i].color, sheep_stream[i].pattern);
        } else {
            printf("Sheep %d (activity: %d, color: %s, pattern: %s) is NOT calm.\n",
                   sheep_stream[i].id, sheep_stream[i].activity_level, sheep_stream[i].color, sheep_stream[i].pattern);
        }
    }

    printf("Total calm sheep: %d\n", calm_sheep_count);

    // Simulate updating criteria
    printf("\nUpdating criteria...\n");
    max_activity_level = 2;
    const char *new_allowed_colors[] = {"white"};
    num_allowed_colors = 1;
    const char *new_allowed_patterns[] = {"plain", "spotted"};
    num_allowed_patterns = 2;

    calm_sheep_count = 0;
    printf("New Max Activity: %d, Allowed Colors: [%s], Allowed Patterns: [%s, %s]\n",
           max_activity_level, new_allowed_colors[0], new_allowed_patterns[0], new_allowed_patterns[1]);

    // Reprocess with new criteria
    for (int i = 0; i < num_sheep; i++) {
        if (is_calm(&sheep_stream[i], max_activity_level, new_allowed_colors, num_allowed_colors, new_allowed_patterns, num_allowed_patterns)) {
            calm_sheep_count++;
            printf("Sheep %d (activity: %d, color: %s, pattern: %s) is calm.\n",
                   sheep_stream[i].id, sheep_stream[i].activity_level, sheep_stream[i].color, sheep_stream[i].pattern);
        } else {
            printf("Sheep %d (activity: %d, color: %s, pattern: %s) is NOT calm.\n",
                   sheep_stream[i].id, sheep_stream[i].activity_level, sheep_stream[i].color, sheep_stream[i].pattern);
        }
    }

    printf("Total calm sheep with new criteria: %d\n", calm_sheep_count);

    return 0;
}
