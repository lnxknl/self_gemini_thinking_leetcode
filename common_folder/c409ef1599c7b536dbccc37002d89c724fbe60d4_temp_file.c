#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h> // For INT_MAX

// Comparison function for sorting in descending order
int compareAmounts(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

int findMinProvidersRecursive(int target_amount, int current_amount, int num_providers, int provider_amounts[], bool willingness[], bool approached[], int approached_count) {
    if (current_amount >= target_amount) {
        return approached_count;
    }

    if (approached_count == num_providers) {
        return INT_MAX; // Could not reach the target
    }

    int min_providers = INT_MAX;

    for (int i = 0; i < num_providers; i++) {
        if (!approached[i] && willingness[i]) {
            approached[i] = true;
            int contribution = (target_amount - current_amount < provider_amounts[i]) ? (target_amount - current_amount) : provider_amounts[i];
            int result = findMinProvidersRecursive(target_amount, current_amount + contribution, num_providers, provider_amounts, willingness, approached, approached_count + 1);
            if (result != INT_MAX && result < min_providers) {
                min_providers = result;
            }
            approached[i] = false; // Backtrack
        }
    }
    return min_providers;
}

int solvePettyCashPredicament(int target_amount, int provider_amounts[], int num_providers, bool willingness[], bool police_presence) {
    // Sort provider amounts in descending order (greedy optimization)
    int *sorted_amounts = malloc(sizeof(int) * num_providers);
    for (int i = 0; i < num_providers; i++) {
        sorted_amounts[i] = provider_amounts[i];
    }
    qsort(sorted_amounts, num_providers, sizeof(int), compareAmounts);

    // Adjust willingness based on police presence
    bool *adjusted_willingness = malloc(sizeof(bool) * num_providers);
    for (int i = 0; i < num_providers; i++) {
        adjusted_willingness[i] = willingness[i];
        if (!willingness[i] && police_presence) {
            adjusted_willingness[i] = true; // Assuming best-case scenario for willingness
        }
    }

    bool *approached = calloc(num_providers, sizeof(bool));
    int min_approaches = findMinProvidersRecursive(target_amount, 0, num_providers, sorted_amounts, adjusted_willingness, approached, 0);

    free(sorted_amounts);
    free(adjusted_willingness);
    free(approached);

    return (min_approaches == INT_MAX) ? -1 : min_approaches;
}

// Test Example
int main() {
    int target_amount = 20;
    int provider_amounts[] = {10, 2,3, 15, 8};
    int num_providers = sizeof(provider_amounts) / sizeof(provider_amounts[0]);
    bool willingness[] = {true, false, true, false};
    bool police_presence = true;

    int result = solvePettyCashPredicament(target_amount, provider_amounts, num_providers, willingness, police_presence);
    printf("Minimum providers needed: %d\n", result); // Expected output might vary due to police influence interpretation

    // Test case without police
    police_presence = false;
    result = solvePettyCashPredicament(target_amount, provider_amounts, num_providers, willingness, police_presence);
    printf("Minimum providers needed (no police): %d\n", result);

    return 0;
}
