#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent a speck
typedef struct Speck {
    int x;
    int y;
    int time;
    struct Speck* next; // For collision handling (separate chaining)
} Speck;

// Hash table structure
typedef struct HashTable {
    int size;
    Speck** table;
} HashTable;

// Simple hash function
unsigned int hash(int x, int y, int time, int table_size) {
    // A basic combination method, can be improved
    unsigned int combined = (unsigned int)x * 31 + (unsigned int)y * 37 + (unsigned int)time;
    return combined % table_size;
}

// Function to create a new speck
Speck* createSpeck(int x, int y, int time) {
    Speck* newSpeck = (Speck*)malloc(sizeof(Speck));
    if (!newSpeck) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newSpeck->x = x;
    newSpeck->y = y;
    newSpeck->time = time;
    newSpeck->next = NULL;
    return newSpeck;
}

// Function to create a hash table
HashTable* createHashTable(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    ht->size = size;
    ht->table = (Speck**)calloc(size, sizeof(Speck*));
    if (!ht->table) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ht;
}

// Function to insert a speck into the hash table
void insertSpeck(HashTable* ht, int x, int y, int time) {
    unsigned int index = hash(x, y, time, ht->size);
    Speck* newSpeck = createSpeck(x, y, time);
    newSpeck->next = ht->table[index];
    ht->table[index] = newSpeck;
}

// Function to search for the target speck
bool findTargetSpeck(HashTable* ht, int target_x, int target_y, int target_time) {
    unsigned int index = hash(target_x, target_y, target_time, ht->size);
    Speck* current = ht->table[index];
    while (current) {
        if (current->x == target_x && current->y == target_y && current->time == target_time) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to free the hash table
void freeHashTable(HashTable* ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        Speck* current = ht->table[i];
        while (current) {
            Speck* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

int main() {
    // Example usage

    // Simulate speck appearances
    int appearances[][3] = {
        {10, 20, 1},
        {15, 25, 2},
        {10, 20, 3}, // Note the same coordinates but different time
        {30, 40, 2},
        {5, 10, 4}
    };
    int num_appearances = sizeof(appearances) / sizeof(appearances[0]);

    // Target speck information
    int target_x = 10;
    int target_y = 20;
    int target_time = 3;

    // Create a hash table (choose a reasonable size)
    int hash_table_size = 10;
    HashTable* ht = createHashTable(hash_table_size);

    // Insert the appeared specks into the hash table
    for (int i = 0; i < num_appearances; i++) {
        insertSpeck(ht, appearances[i][0], appearances[i][1], appearances[i][2]);
    }

    // Search for the target speck
    if (findTargetSpeck(ht, target_x, target_y, target_time)) {
        printf("Target speck found at (%d, %d) at time %d!\n", target_x, target_y, target_time);
    } else {
        printf("Target speck not found at (%d, %d) at time %d.\n", target_x, target_y, target_time);
    }

    // Example where the target is not found (different time)
    int target_x2 = 10;
    int target_y2 = 20;
    int target_time2 = 1;
    
    if (findTargetSpeck(ht, target_x2, target_y2, target_time2)) {
        printf("Target speck found at (%d, %d) at time %d!\n", target_x2, target_y2, target_time2);
    } else {
        printf("Target speck not found at (%d, %d) at time %d.\n", target_x2, target_y2, target_time2);
    }

    // Clean up
    freeHashTable(ht);

    return 0;
}
