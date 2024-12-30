#include <stdio.h>
#include <stdlib.h>

int singleNumber(int* nums, int numsSize) {
    int result = 0;
    for (int i = 0; i < numsSize; i++) {
        result ^= nums[i];
    }
    return result;
}

int main() {
    // Test Example 1
    int nums1[] = {2, 2, 1};
    int size1 = sizeof(nums1) / sizeof(nums1[0]);
    int single1 = singleNumber(nums1, size1);
    printf("Single number in {2, 2, 1}: %d\n", single1); // Expected output: 1

    // Test Example 2
    int nums2[] = {4, 1, 2, 1, 2};
    int size2 = sizeof(nums2) / sizeof(nums2[0]);
    int single2 = singleNumber(nums2, size2);
    printf("Single number in {4, 1, 2, 1, 2}: %d\n", single2); // Expected output: 4

    // Test Example 3 (Negative numbers)
    int nums3[] = {-1, 2, -1};
    int size3 = sizeof(nums3) / sizeof(nums3[0]);
    int single3 = singleNumber(nums3, size3);
    printf("Single number in {-1, 2, -1}: %d\n", single3); // Expected output: 2

    // Test Example 4 (Single element array)
    int nums4[] = {5};
    int size4 = sizeof(nums4) / sizeof(nums4[0]);
    int single4 = singleNumber(nums4, size4);
    printf("Single number in {5}: %d\n", single4); // Expected output: 5

    return 0;
}
