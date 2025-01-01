#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Comparison function for qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int organizeStreetBrawl(int* skills, int numFighters) {
    if (numFighters < 2) {
        return 0; // No matches possible
    }

    // Sort the skill levels
    qsort(skills, numFighters, sizeof(int), compare);

    int totalExcitement = 0;
    int left = 0;
    int right = numFighters - 1;

    while (left < right) {
        totalExcitement += abs(skills[right] - skills[left]);
        left++;
        right--;
    }

    return totalExcitement;
}

int main() {
    // Test Example 1
    int skills1[] = {1, 5, 3, 10};
    int numFighters1 = sizeof(skills1) / sizeof(skills1[0]);
    int excitement1 = organizeStreetBrawl(skills1, numFighters1);
    printf("Total excitement for fighters [1, 5, 3, 10]: %d\n", excitement1); // Expected: 12 (Pairing 1 and 10, 3 and 5)

    // Test Example 2 (Odd number of fighters)
    int skills2[] = {1, 5, 3, 10, 7};
    int numFighters2 = sizeof(skills2) / sizeof(skills2[0]);
    int excitement2 = organizeStreetBrawl(skills2, numFighters2);
    printf("Total excitement for fighters [1, 5, 3, 10, 7]: %d\n", excitement2); // Expected: 15 (Pairing 1 and 10, 3 and 7, 5 sits out)

    // Test Example 3 (Already sorted)
    int skills3[] = {1, 3, 5, 7};
    int numFighters3 = sizeof(skills3) / sizeof(skills3[0]);
    int excitement3 = organizeStreetBrawl(skills3, numFighters3);
    printf("Total excitement for fighters [1, 3, 5, 7]: %d\n", excitement3); // Expected: 10

    // Test Example 4 (Small input)
    int skills4[] = {1, 10};
    int numFighters4 = sizeof(skills4) / sizeof(skills4[0]);
    int excitement4 = organizeStreetBrawl(skills4, numFighters4);
    printf("Total excitement for fighters [1, 10]: %d\n", excitement4); // Expected: 9

    // Test Example 5 (Single fighter)
    int skills5[] = {5};
    int numFighters5 = sizeof(skills5) / sizeof(skills5[0]);
    int excitement5 = organizeStreetBrawl(skills5, numFighters5);
    printf("Total excitement for fighters [5]: %d\n", excitement5); // Expected: 0

    return 0;
}
