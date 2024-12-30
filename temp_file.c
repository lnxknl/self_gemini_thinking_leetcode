#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define a structure to represent coordinates (assuming a fixed dimension for simplicity)
#define DIMENSION 3
typedef struct {
    int coords[DIMENSION];
} Coordinates;

// Define a structure for a hash table node
typedef struct HashNode {
    Coordinates key;
    int count;
    struct HashNode *next;
} HashNode;

// Simple hash function (can be improved for better distribution)
unsigned int hash_coords(Coordinates c) {
    unsigned int hash = 0;
    for (int i = 0; i < DIMENSION; i++) {
        hash = hash * 31 + c.coords[i];
    }
    return hash;
}

// Function to compare two coordinate sets
bool compare_coords(Coordinates c1, Coordinates c2) {
    for (int i = 0; i < DIMENSION; i++) {
        if (c1.coords[i] != c2.coords[i]) {
            return false;
        }
    }
    return true;
}

// Function to insert or update coordinates in the hash table
void insert_or_update(HashNode **table, int table_size, Coordinates coords) {
    unsigned int index = hash_coords(coords) % table_size;
    HashNode *current = table[index];

    while (current != NULL) {
        if (compare_coords(current->key, coords)) {
            current->count++;
            return;
        }
        current = current->next;
    }

    // If not found, create a new node and insert it
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_node->key = coords;
    new_node->count = 1;
    new_node->next = table[index];
    table[index] = new_node;
}

// Function to find the unique element in the hash table
Coordinates* find_unique_element(HashNode **table, int table_size) {
    for (int i = 0; i < table_size; i++) {
        HashNode *current = table[i];
        while (current != NULL) {
            if (current->count == 1) {
                return &(current->key);
            }
            current = current->next;
        }
    }
    return NULL; // Unique element not found (shouldn't happen if there's exactly one)
}

// Function to free the hash table
void free_hash_table(HashNode **table, int table_size) {
    for (int i = 0; i < table_size; i++) {
        HashNode *current = table[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
        table[i] = NULL;
    }
}

int main() {
    // Simulate an input stream of coordinates
    Coordinates input_stream[] = {
        {{1, 2, 3}},
        {{10, 5, 2}},
        {{4, 5, 6}},
        {{1, 2, 3}},
        {{7, 8, 9}},
        {{10, 5, 2}},
        {{4, 5, 6}}
    };
    int stream_size = sizeof(input_stream) / sizeof(input_stream[0]);

    // Initialize the hash table
    int table_size = 10; // Choose a suitable size
    HashNode **hash_table = (HashNode **)calloc(table_size, sizeof(HashNode *));
    if (hash_table == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Process the input stream
    for (int i = 0; i < stream_size; i++) {
        insert_or_update(hash_table, table_size, input_stream[i]);
    }

    // Find the unique element
    Coordinates *unique_element = find_unique_element(hash_table, table_size);

    // Print the result
    if (unique_element != NULL) {
        printf("Unique element found at coordinates: (%d, %d, %d)\n",
               unique_element->coords[0], unique_element->coords[1], unique_element->coords[2]);
    } else {
        printf("No unique element found.\n");
    }

    // Free the hash table
    free_hash_table(hash_table, table_size);
    free(hash_table);

    return 0;
}
