#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort (ascending order)
int compareHunger(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int maxSatisfiedResidents(int hunger[], int n, int food_items) {
    // 1. Sort the hunger array
    qsort(hunger, n, sizeof(int), compareHunger);

    int satisfied_count = 0;

    // 2. Iterate and allocate food
    for (int i = 0; i < n; i++) {
        if (food_items >= hunger[i]) {
            food_items -= hunger[i];
            satisfied_count++;
        } else {
            break; // No more residents can be fully satisfied
        }
    }

    return satisfied_count;
}

int main() {
    // Test Example 1
    int hunger1[] = {2, 1, 3};
    int n1 = sizeof(hunger1) / sizeof(hunger1[0]);
    int food_items1 = 4;
    int result1 = maxSatisfiedResidents(hunger1, n1, food_items1);
    printf("Test Case 1: Max satisfied residents = %d\n", result1); // Expected Output: 2

    // Test Example 2
    int hunger2[] = {5, 2, 3, 1, 4};
    int n2 = sizeof(hunger2) / sizeof(hunger2[0]);
    int food_items2 = 8;
    int result2 = maxSatisfiedResidents(hunger2, n2, food_items2);
    printf("Test Case 2: Max satisfied residents = %d\n", result2); // Expected Output: 3

    // Test Example 3 (Edge Case: Not enough food for anyone)
    int hunger3[] = {3, 4, 5};
    int n3 = sizeof(hunger3) / sizeof(hunger3[0]);
    int food_items3 = 2;
    int result3 = maxSatisfiedResidents(hunger3, n3, food_items3);
    printf("Test Case 3: Max satisfied residents = %d\n", result3); // Expected Output: 0

    // Test Example 4 (Edge Case: Enough food for everyone)
    int hunger4[] = {1, 1, 1};
    int n4 = sizeof(hunger4) / sizeof(hunger4[0]);
    int food_items4 = 3;
    int result4 = maxSatisfiedResidents(hunger4, n4, food_items4);
    printf("Test Case 4: Max satisfied residents = %d\n", result4); // Expected Output: 3

    return 0;
}
