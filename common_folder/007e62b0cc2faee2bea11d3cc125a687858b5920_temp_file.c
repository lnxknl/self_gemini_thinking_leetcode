#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure for a hash table node
typedef struct Node {
    char* key;
    int count;
    struct Node* next;
} Node;

// Hash table structure
typedef struct HashTable {
    int size;
    Node** table;
} HashTable;

// Hash function (simple polynomial rolling hash)
unsigned int hash(const char* str, int size) {
    unsigned int hashValue = 5381;
    int c;
    while ((c = *str++))
        hashValue = ((hashValue << 5) + hashValue) + c; /* hash * 33 + c */
    return hashValue % size;
}

// Create a new hash table
HashTable* createHashTable(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht == NULL) {
        perror("Failed to allocate memory for hash table");
        exit(EXIT_FAILURE);
    }
    ht->size = size;
    ht->table = (Node**)calloc(ht->size, sizeof(Node*));
    if (ht->table == NULL) {
        perror("Failed to allocate memory for hash table buckets");
        exit(EXIT_FAILURE);
    }
    return ht;
}

// Insert or update a key in the hash table
void insertOrUpdate(HashTable* ht, const char* key) {
    unsigned int index = hash(key, ht->size);
    Node* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for hash table node");
        exit(EXIT_FAILURE);
    }
    newNode->key = strdup(key);
    newNode->count = 1;
    newNode->next = ht->table[index];
    ht->table[index] = newNode;
}

// Get the count of a key in the hash table
int getCount(HashTable* ht, const char* key) {
    unsigned int index = hash(key, ht->size);
    Node* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->count;
        }
        current = current->next;
    }
    return 0;
}

// Function to calculate the minimum outfit changes
int minOutfitChanges(char* initialOutfit[], int initialSize, char* finalOutfit[], int finalSize) {
    HashTable* initialCounts = createHashTable(initialSize + finalSize); // Heuristic size
    HashTable* finalCounts = createHashTable(initialSize + finalSize);

    // Populate hash tables
    for (int i = 0; i < initialSize; i++) {
        insertOrUpdate(initialCounts, initialOutfit[i]);
    }
    for (int i = 0; i < finalSize; i++) {
        insertOrUpdate(finalCounts, finalOutfit[i]);
    }

    int changes = 0;
    int keptItems = 0;

    // Iterate through the initial outfit counts
    for (int i = 0; i < initialCounts->size; i++) {
        Node* current = initialCounts->table[i];
        while (current != NULL) {
            int initialCount = current->count;
            int finalCount = getCount(finalCounts, current->key);
            keptItems += (initialCount < finalCount ? initialCount : finalCount); // Number of items that can be kept
            current = current->next;
        }
    }

    changes = initialSize + finalSize - 2 * keptItems;

    // Free allocated memory (important!)
    for (int i = 0; i < initialCounts->size; i++) {
        Node* current = initialCounts->table[i];
        while (current != NULL) {
            Node* temp = current;
            free(temp->key);
            current = current->next;
            free(temp);
        }
    }
    free(initialCounts->table);
    free(initialCounts);

    for (int i = 0; i < finalCounts->size; i++) {
        Node* current = finalCounts->table[i];
        while (current != NULL) {
            Node* temp = current;
            free(temp->key);
            current = current->next;
            free(temp);
        }
    }
    free(finalCounts->table);
    free(finalCounts);

    return changes;
}

int main() {
    char* initialOutfit[] = {"red_jacket", "white_shirt", "beige_pants", "black_glasses"};
    int initialSize = sizeof(initialOutfit) / sizeof(initialOutfit[0]);
    char* finalOutfit[] = {"white_sweater", "beige_pants", "brown_glasses"};
    int finalSize = sizeof(finalOutfit) / sizeof(finalOutfit[0]);

    int changes = minOutfitChanges(initialOutfit, initialSize, finalOutfit, finalSize);
    printf("Minimum outfit changes: %d\n", changes); // Expected output: 4

    char* initialOutfit2[] = {"blue_shirt", "black_pants"};
    int initialSize2 = sizeof(initialOutfit2) / sizeof(initialOutfit2[0]);
    char* finalOutfit2[] = {"blue_shirt", "black_pants"};
    int finalSize2 = sizeof(finalOutfit2) / sizeof(finalOutfit2[0]);
    changes = minOutfitChanges(initialOutfit2, initialSize2, finalOutfit2, finalSize2);
    printf("Minimum outfit changes: %d\n", changes); // Expected output: 0

    char* initialOutfit3[] = {"red_tshirt"};
    int initialSize3 = sizeof(initialOutfit3) / sizeof(initialOutfit3[0]);
    char* finalOutfit3[] = {"blue_tshirt"};
    int finalSize3 = sizeof(finalOutfit3) / sizeof(finalOutfit3[0]);
    changes = minOutfitChanges(initialOutfit3, initialSize3, finalOutfit3, finalSize3);
    printf("Minimum outfit changes: %d\n", changes); // Expected output: 1

    return 0;
}
