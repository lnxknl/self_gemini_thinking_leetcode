#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define a structure to represent a data point
typedef struct {
    int x;
    int y;
} Point;

// A simple hash table implementation (for demonstration purposes)
#define HASH_TABLE_SIZE 1000

typedef struct HashTableEntry {
    Point key;
    bool exists;
    struct HashTableEntry* next;
} HashTableEntry;

HashTableEntry* hashTable[HASH_TABLE_SIZE];

// Hash function (simple modulo)
unsigned int hash(Point p) {
    // Combine x and y to create a better distribution
    unsigned int combined = (p.x * 31) ^ p.y;
    return combined % HASH_TABLE_SIZE;
}

// Insert a point into the hash table
void insert(Point p) {
    unsigned int index = hash(p);
    HashTableEntry* newEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    if (!newEntry) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newEntry->key = p;
    newEntry->exists = true;
    newEntry->next = hashTable[index];
    hashTable[index] = newEntry;
}

// Search for a point in the hash table
bool search(Point p) {
    unsigned int index = hash(p);
    HashTableEntry* current = hashTable[index];
    while (current != NULL) {
        if (current->key.x == p.x && current->key.y == p.y) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int main() {
    // Initialize the hash table
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    // Simulate the data stream (replace with actual stream input)
    Point stream[] = {
        {1, 2},
        {5, 7},
        {1, 2}, // Duplicate
        {10, -5}, // Potential target
        {5, 7},  // Duplicate
        {12, 3}
        // ... potentially many more data points
    };
    int streamSize = sizeof(stream) / sizeof(stream[0]);

    Point target = {-1, -1}; // Initialize target

    printf("Processing data stream...\n");

    for (int i = 0; i < streamSize; i++) {
        Point currentPoint = stream[i];
        printf("Received point: (%d, %d)\n", currentPoint.x, currentPoint.y);

        if (!search(currentPoint)) {
            printf("Potential target found: (%d, %d)\n", currentPoint.x, currentPoint.y);
            target = currentPoint;
            break; // Target found, terminate processing
        } else {
            printf("Duplicate point encountered.\n");
        }
        insert(currentPoint);
    }

    if (target.x != -1) {
        printf("\nTarget data point located at: (%d, %d)\n", target.x, target.y);
    } else {
        printf("\nTarget data point not found in the processed stream.\n");
    }

    // Basic cleanup (free allocated memory in hash table - omitted for brevity)

    return 0;
}
