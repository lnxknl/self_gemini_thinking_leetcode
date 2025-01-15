#include <stdio.h>
#include <stdlib.h>

// Structure to represent an interval
typedef struct {
    int start;
    int end;
} Interval;

// Comparison function for sorting intervals by start time
int compareIntervals(const void *a, const void *b) {
    return ((Interval *)a)->start - ((Interval *)b)->start;
}

// Function to merge overlapping intervals
Interval* mergeIntervals(Interval intervals[], int n, int *mergedCount) {
    if (n <= 0) {
        *mergedCount = 0;
        return NULL;
    }

    // Sort the intervals based on start time
    qsort(intervals, n, sizeof(Interval), compareIntervals);

    Interval *merged = (Interval *)malloc(n * sizeof(Interval)); // Allocate maximum possible size
    if (merged == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    merged[0] = intervals[0];
    *mergedCount = 1;

    for (int i = 1; i < n; i++) {
        Interval current = intervals[i];
        Interval lastMerged = merged[*mergedCount - 1];

        if (current.start <= lastMerged.end) {
            // Overlapping interval, merge
            merged[*mergedCount - 1].end = (current.end > lastMerged.end) ? current.end : lastMerged.end;
        } else {
            // Non-overlapping interval, add to merged list
            merged[*mergedCount] = current;
            (*mergedCount)++;
        }
    }

    return merged;
}

// Function to find the longest continuous watched segment
int findLongestContinuousSegment(Interval mergedIntervals[], int mergedCount) {
    int maxLength = 0;
    for (int i = 0; i < mergedCount; i++) {
        int length = mergedIntervals[i].end - mergedIntervals[i].start;
        if (length > maxLength) {
            maxLength = length;
        }
    }
    return maxLength;
}

int main() {
    // Test input
    Interval intervals[] = {{1, 5}, {10, 15}, {3, 7}, {20, 25}, {14, 18}};
    int n = sizeof(intervals) / sizeof(intervals[0]);

    int mergedCount;
    Interval *mergedIntervals = mergeIntervals(intervals, n, &mergedCount);

    printf("Merged Intervals:\n");
    for (int i = 0; i < mergedCount; i++) {
        printf("[%d, %d] ", mergedIntervals[i].start, mergedIntervals[i].end);
    }
    printf("\n");

    int longestSegment = findLongestContinuousSegment(mergedIntervals, mergedCount);
    printf("Longest Continuous Watched Segment: %d\n", longestSegment);

    free(mergedIntervals); // Free allocated memory

    return 0;
}
