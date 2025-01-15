#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define a simple hash table structure (for demonstration purposes)
#define TABLE_SIZE 1024

typedef struct HashNode {
    int key;
    int count;
    struct HashNode *next;
} HashNode;

HashNode *hash_table[TABLE_SIZE];

// Simple hash function
unsigned int hash(int key) {
    return (unsigned int)key % TABLE_SIZE;
}

// Insert or update an element in the hash table
void hash_insert(int key) {
    unsigned int index = hash(key);
    HashNode *current = hash_table[index];

    while (current != NULL) {
        if (current->key == key) {
            current->count++;
            return;
        }
        current = current->next;
    }

    // Key not found, create a new node
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_node->key = key;
    new_node->count = 1;
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}

// Find the unique element
int find_lone_signal(int arr[], int size) {
    // Initialize the hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }

    // Populate the hash table with signal counts
    for (int i = 0; i < size; i++) {
        hash_insert(arr[i]);
    }

    // Find the signal with a count of 1
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *current = hash_table[i];
        while (current != NULL) {
            if (current->count == 1) {
                return current->key;
            }
            current = current->next;
        }
    }

    // Should not happen if the problem constraints are met
    return -1;
}

int main() {
    // Test example input (representing a stream of signals)
    int signals[] = {1, 2, 3, 4, 5, 2, 1, 4, 3, 6, 5, 1, 2, 4, 3, 5, 6, 7, 6, 5, 4, 3, 2, 1};
    int size = sizeof(signals) / sizeof(signals[0]);

    int lone_signal = find_lone_signal(signals, size);

    if (lone_signal != -1) {
        printf("The lone signal is: %d\n", lone_signal);
    } else {
        printf("No lone signal found.\n");
    }

    // Clean up hash table memory (important in real applications)
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *current = hash_table[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}
