#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MIN

// Structure to represent a coordinate
typedef struct {
    int row;
    int col;
    int value;
} Point;

// A very basic (and inefficient for large scale) way to simulate a hash table
// In real-world scenarios, you'd use a proper hash table implementation.
#define MAX_POINTS 100
Point hashTable[MAX_POINTS];
int hashTableSize = 0;

// Simple hash function (can be improved)
int hash(int row, int col) {
    return (row * 31 + col) % MAX_POINTS;
}

// Function to add a point to the hash table (handles collisions naively)
void addPoint(Point p) {
    int index = hash(p.row, p.col);
    // Naive collision handling: linear probing
    while (hashTable[index].row != -1 && hashTable[index].col != -1) {
        index = (index + 1) % MAX_POINTS;
    }
    hashTable[index] = p;
    hashTableSize++;
}

// Function to find the brightest point seen so far
Point findBrightest(Point* points, int numPoints) {
    // Initialize the hash table with "empty" markers
    for (int i = 0; i < MAX_POINTS; i++) {
        hashTable[i].row = -1;
        hashTable[i].col = -1;
        hashTable[i].value = 0;
    }
    hashTableSize = 0;

    Point brightest = {-1, -1, INT_MIN};

    for (int i = 0; i < numPoints; i++) {
        addPoint(points[i]); // Add to our "hash table"
        if (points[i].value > brightest.value) {
            brightest = points[i];
        }
    }
    return brightest;
}

int main() {
    // Example input with multiple potentially bright points
    Point input[] = {
        {1, 2, 3},
        {3, 4, 5},
        {5, 7, 10}, // Brightest point
        {8, 9, 2},
        {2, 3, 7}
    };
    int numPoints = sizeof(input) / sizeof(input[0]);

    Point brightest = findBrightest(input, numPoints);

    if (brightest.row != -1) {
        printf("Brightest point found at: (%d, %d) with value %d\n", brightest.row, brightest.col, brightest.value);
    } else {
        printf("No bright points found.\n");
    }

    return 0;
}
