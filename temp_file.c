#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Simple hash table implementation (for demonstration purposes)
#define HASH_TABLE_SIZE 101 // A prime number for better distribution

typedef struct HashTable {
    int key;
    bool occupied;
} HashTable;

unsigned int hash(int key) {
    return key % HASH_TABLE_SIZE;
}

bool insert(HashTable *ht, int key) {
    unsigned int index = hash(key);
    if (!ht[index].occupied) {
        ht[index].key = key;
        ht[index].occupied = true;
        return true;
    }
    return false; // Collision, or already present (for this simple case)
}

bool search(HashTable *ht, int key) {
    unsigned int index = hash(key);
    return ht[index].occupied && ht[index].key == key;
}

bool findSingleLight(int probes[], int numProbes, int *targetFound) {
    if (numProbes <= 0) {
        return false;
    }

    HashTable *ht = (HashTable *)malloc(sizeof(HashTable) * HASH_TABLE_SIZE);
    if (ht == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht[i].occupied = false;
    }

    int potentialTarget = probes[0];
    insert(ht, potentialTarget);

    for (int i = 1; i < numProbes; i++) {
        if (search(ht, probes[i])) {
            *targetFound = probes[i];
            free(ht);
            return true;
        }
    }

    free(ht);
    return false;
}

int main() {
    // Test case 1: Light is present
    int probes1[] = {-5, 0, 1000, 50, 1000, -20};
    int numProbes1 = sizeof(probes1) / sizeof(probes1[0]);
    int targetFound1;
    if (findSingleLight(probes1, numProbes1, &targetFound1)) {
        printf("Test Case 1: Light found! Value: %d\n", targetFound1);
    } else {
        printf("Test Case 1: Light not found.\n");
    }

    // Test case 2: Light is not present (all probes are unique initially)
    int probes2[] = {1, 2, 3, 4, 5};
    int numProbes2 = sizeof(probes2) / sizeof(probes2[0]);
    int targetFound2;
    if (findSingleLight(probes2, numProbes2, &targetFound2)) {
        printf("Test Case 2: Light found! Value: %d\n", targetFound2);
    } else {
        printf("Test Case 2: Light not found.\n");
    }

    // Test case 3: Single probe, which is the light
    int probes3[] = {77};
    int numProbes3 = sizeof(probes3) / sizeof(probes3[0]);
    int targetFound3;
    if (findSingleLight(probes3, numProbes3, &targetFound3)) {
        printf("Test Case 3: Light found! Value: %d\n", targetFound3);
    } else {
        printf("Test Case 3: Light not found.\n");
    }

    // Test case 4: Empty probes
    int probes4[] = {};
    int numProbes4 = sizeof(probes4) / sizeof(probes4[0]);
    int targetFound4;
    if (findSingleLight(probes4, numProbes4, &targetFound4)) {
        printf("Test Case 4: Light found! Value: %d\n", targetFound4);
    } else {
        printf("Test Case 4: Light not found.\n");
    }

    return 0;
}
