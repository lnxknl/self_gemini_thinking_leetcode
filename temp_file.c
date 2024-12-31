#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a hash table node
typedef struct HashTableNode {
    char* key;
    int count;
    struct HashTableNode* next;
} HashTableNode;

// Simple hash function (can be improved for better distribution)
unsigned int hash(const char* str, int capacity) {
    unsigned int hashValue = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hashValue = 31 * hashValue + str[i];
    }
    return hashValue % capacity;
}

// Function to insert/update an item in the hash table
void insertOrUpdate(HashTableNode** hashTable, const char* key, int capacity) {
    unsigned int index = hash(key, capacity);
    HashTableNode* current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }

    HashTableNode* newNode = (HashTableNode*)malloc(sizeof(HashTableNode));
    newNode->key = strdup(key);
    newNode->count = 1;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Function to find the count of an item in the hash table
int findCount(HashTableNode** hashTable, const char* key, int capacity) {
    unsigned int index = hash(key, capacity);
    HashTableNode* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->count;
        }
        current = current->next;
    }
    return 0;
}

// Function to find the missing items
char** findMissingItems(char** expected_items, int expected_count, char** packed_items, int packed_count, int* missing_count) {
    int capacity = expected_count * 2; // A reasonable capacity for the hash tables
    HashTableNode** expected_counts = (HashTableNode**)calloc(capacity, sizeof(HashTableNode*));
    HashTableNode** packed_counts = (HashTableNode**)calloc(capacity, sizeof(HashTableNode*));

    // Count expected items
    for (int i = 0; i < expected_count; i++) {
        insertOrUpdate(expected_counts, expected_items[i], capacity);
    }

    // Count packed items
    for (int i = 0; i < packed_count; i++) {
        if (findCount(expected_counts, packed_items[i], capacity) > 0) {
            insertOrUpdate(packed_counts, packed_items[i], capacity);
        }
    }

    // Identify missing items
    char** missing_items_list = (char**)malloc(expected_count * sizeof(char*)); // Max possible missing items
    *missing_count = 0;
    for (int i = 0; i < capacity; i++) {
        HashTableNode* current = expected_counts[i];
        while (current != NULL) {
            int expected = current->count;
            int packed = findCount(packed_counts, current->key, capacity);
            int difference = expected - packed;
            for (int j = 0; j < difference; j++) {
                missing_items_list[*missing_count] = strdup(current->key);
                (*missing_count)++;
            }
            current = current->next;
        }
    }

    // Clean up hash tables (free memory - important!)
    for (int i = 0; i < capacity; i++) {
        HashTableNode* current = expected_counts[i];
        while (current != NULL) {
            HashTableNode* temp = current;
            free(temp->key);
            current = current->next;
            free(temp);
        }
        current = packed_counts[i];
        while (current != NULL) {
            HashTableNode* temp = current;
            free(temp->key);
            current = current->next;
            free(temp);
        }
    }
    free(expected_counts);
    free(packed_counts);

    return missing_items_list;
}

// Test Example
int main() {
    char* expected[] = {"sock_blue_left", "sock_blue_right", "onesie_size3", "hat_winter", "sock_blue_left"};
    int expected_count = sizeof(expected) / sizeof(expected[0]);
    char* packed[] = {"sock_blue_right", "onesie_size3", "hat_winter"};
    int packed_count = sizeof(packed) / sizeof(packed[0]);

    int missing_count;
    char** missing = findMissingItems(expected, expected_count, packed, packed_count, &missing_count);

    printf("Missing items:\n");
    for (int i = 0; i < missing_count; i++) {
        printf("- %s\n", missing[i]);
        free(missing[i]); // Free the allocated strings
    }
    free(missing); // Free the array of strings

    return 0;
}
