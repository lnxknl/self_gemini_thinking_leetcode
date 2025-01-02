#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort (descending order)
int compareDescending(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int maxBeltComfort(int holes[], int n, int num_prongs) {
    if (num_prongs == 0) {
        return 0;
    }

    if (num_prongs > n) {
        // This scenario shouldn't occur based on constraints, but handling it defensively
        num_prongs = n;
    }

    // Create a copy of the holes array to avoid modifying the original
    int *sorted_holes = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        sorted_holes[i] = holes[i];
    }

    // Sort the holes array in descending order
    qsort(sorted_holes, n, sizeof(int), compareDescending);

    int max_comfort = 0;
    for (int i = 0; i < num_prongs; i++) {
        max_comfort += sorted_holes[i];
    }

    free(sorted_holes);
    return max_comfort;
}

int main() {
    // Test Example 1
    int holes1[] = {5, 1, 8, 3, 6};
    int n1 = sizeof(holes1) / sizeof(holes1[0]);
    int num_prongs1 = 3;
    int result1 = maxBeltComfort(holes1, n1, num_prongs1);
    printf("Maximum comfort (Test 1): %d\n", result1); // Expected: 5 + 8 + 6 = 19

    // Test Example 2
    int holes2[] = {-2, 0, 3, -1, 4};
    int n2 = sizeof(holes2) / sizeof(holes2[0]);
    int num_prongs2 = 2;
    int result2 = maxBeltComfort(holes2, n2, num_prongs2);
    printf("Maximum comfort (Test 2): %d\n", result2); // Expected: 4 + 3 = 7

    // Test Example 3
    int holes3[] = {10, 10, 10, 10};
    int n3 = sizeof(holes3) / sizeof(holes3[0]);
    int num_prongs3 = 4;
    int result3 = maxBeltComfort(holes3, n3, num_prongs3);
    printf("Maximum comfort (Test 3): %d\n", result3); // Expected: 10 + 10 + 10 + 10 = 40

    // Test Example 4 (Fewer prongs than holes)
    int holes4[] = {5, 2, 8, 1};
    int n4 = sizeof(holes4) / sizeof(holes4[0]);
    int num_prongs4 = 2;
    int result4 = maxBeltComfort(holes4, n4, num_prongs4);
    printf("Maximum comfort (Test 4): %d\n", result4); // Expected: 8 + 5 = 13

    return 0;
}
