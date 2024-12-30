#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Improved hash table implementation
#define HASH_SIZE 10007  // Larger prime number for better distribution

typedef struct {
    int* keys;
    bool* occupied;
    int size;
} HashTable;

// Initialize hash table
HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) {
        return NULL;
    }

    ht->size = HASH_SIZE;
    ht->keys = (int*)malloc(sizeof(int) * HASH_SIZE);
    ht->occupied = (bool*)malloc(sizeof(bool) * HASH_SIZE);
    
    if (!ht->keys || !ht->occupied) {
        free(ht->keys);
        free(ht->occupied);
        free(ht);
        return NULL;
    }

    memset(ht->occupied, 0, sizeof(bool) * HASH_SIZE);
    return ht;
}

// Free hash table
void freeHashTable(HashTable* ht) {
    if (ht) {
        free(ht->keys);
        free(ht->occupied);
        free(ht);
    }
}

// Hash function
unsigned int hash(int key) {
    unsigned int hashVal = abs(key);  // Handle negative numbers
    return hashVal % HASH_SIZE;
}

// Insert into hash table with linear probing
bool insert(HashTable* ht, int key) {
    if (!ht) return false;

    unsigned int index = hash(key);
    int i = 0;

    // Linear probing
    while (i < ht->size) {
        unsigned int currentIndex = (index + i) % ht->size;
        
        if (!ht->occupied[currentIndex]) {
            ht->keys[currentIndex] = key;
            ht->occupied[currentIndex] = true;
            return true;
        } else if (ht->keys[currentIndex] == key) {
            return true;  // Key already exists
        }
        
        i++;
    }
    
    return false;  // Hash table is full
}

// Search in hash table with linear probing
bool search(HashTable* ht, int key) {
    if (!ht) return false;

    unsigned int index = hash(key);
    int i = 0;

    while (i < ht->size) {
        unsigned int currentIndex = (index + i) % ht->size;
        
        if (!ht->occupied[currentIndex]) {
            return false;  // Key not found
        }
        
        if (ht->keys[currentIndex] == key) {
            return true;  // Key found
        }
        
        i++;
    }
    
    return false;  // Key not found
}

// Function to find single light that appears twice
bool findSingleLight(int probes[], int numProbes, int* targetFound) {
    if (numProbes <= 1) {
        return false;  // Need at least 2 probes to find a duplicate
    }

    HashTable* ht = createHashTable();
    if (!ht) {
        printf("Failed to create hash table\n");
        return false;
    }

    bool found = false;
    for (int i = 0; i < numProbes; i++) {
        if (search(ht, probes[i])) {
            *targetFound = probes[i];
            found = true;
            break;
        }
        insert(ht, probes[i]);
    }

    freeHashTable(ht);
    return found;
}

int main() {
    // Test case 1: Light is present (1000 appears twice)
    int probes1[] = {-5, 0, 1000, 50, 1000, -20};
    int numProbes1 = sizeof(probes1) / sizeof(probes1[0]);
    int targetFound1;
    if (findSingleLight(probes1, numProbes1, &targetFound1)) {
        printf("Test Case 1: Light found! Value: %d\n", targetFound1);
    } else {
        printf("Test Case 1: Light not found.\n");
    }

    // Test case 2: No duplicate values
    int probes2[] = {1, 2, 3, 4, 5};
    int numProbes2 = sizeof(probes2) / sizeof(probes2[0]);
    int targetFound2;
    if (findSingleLight(probes2, numProbes2, &targetFound2)) {
        printf("Test Case 2: Light found! Value: %d\n", targetFound2);
    } else {
        printf("Test Case 2: Light not found.\n");
    }

    // Test case 3: Single element (should return false)
    int probes3[] = {77};
    int numProbes3 = 1;
    int targetFound3;
    if (findSingleLight(probes3, numProbes3, &targetFound3)) {
        printf("Test Case 3: Light found! Value: %d\n", targetFound3);
    } else {
        printf("Test Case 3: Light not found.\n");
    }

    // Test case 4: Empty array (should return false)
    int targetFound4;
    if (findSingleLight(NULL, 0, &targetFound4)) {
        printf("Test Case 4: Light found! Value: %d\n", targetFound4);
    } else {
        printf("Test Case 4: Light not found.\n");
    }

    return 0;
}
