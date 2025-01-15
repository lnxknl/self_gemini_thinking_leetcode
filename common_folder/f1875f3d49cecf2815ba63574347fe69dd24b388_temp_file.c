#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int maxConsecutiveEqual(int arr[], int n, int k) {
    int left = 0;
    int right = 0;
    int max_length = 0;
    int zeros = 0;
    int ones = 0;

    while (right < n) {
        if (arr[right] == 0) {
            zeros++;
        } else {
            ones++;
        }

        while (fmin(zeros, ones) > k) {
            if (arr[left] == 0) {
                zeros--;
            } else {
                ones--;
            }
            left++;
        }

        if (right - left + 1 > max_length) {
            max_length = right - left + 1;
        }
        right++;
    }
    return max_length;
}

int main() {
    // Test Example 1
    int arr1[] = {1, 1, 0, 0, 1, 1, 1, 0};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int k1 = 1;
    printf("Test Case 1: Maximum consecutive equal elements with %d change(s) = %d\n", k1, maxConsecutiveEqual(arr1, n1, k1)); // Expected output: 6 (1,1,1,1,1,1 or 0,0,0,0,0,0)

    // Test Example 2
    int arr2[] = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int k2 = 2;
    printf("Test Case 2: Maximum consecutive equal elements with %d change(s) = %d\n", k2, maxConsecutiveEqual(arr2, n2, k2)); // Expected output: 8

    // Test Example 3
    int arr3[] = {1, 0, 1, 0, 1};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int k3 = 0;
    printf("Test Case 3: Maximum consecutive equal elements with %d change(s) = %d\n", k3, maxConsecutiveEqual(arr3, n3, k3)); // Expected output: 1

    // Test Example 4 (All same)
    int arr4[] = {1, 1, 1, 1, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int k4 = 0;
    printf("Test Case 4: Maximum consecutive equal elements with %d change(s) = %d\n", k4, maxConsecutiveEqual(arr4, n4, k4)); // Expected output: 5

    // Test Example 5 (Empty array)
    int arr5[] = {};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    int k5 = 2;
    printf("Test Case 5: Maximum consecutive equal elements with %d change(s) = %d\n", k5, maxConsecutiveEqual(arr5, n5, k5)); // Expected output: 0

    return 0;
}
