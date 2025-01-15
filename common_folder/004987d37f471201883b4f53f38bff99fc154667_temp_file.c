#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 101 // A prime number for better distribution

// Linked List Node for Locations
typedef struct LocationNode {
    char* location;
    struct LocationNode* next;
} LocationNode;

// Hash Table Entry
typedef struct HashTableEntry {
    char* item;
    LocationNode* locations;
} HashTableEntry;

HashTableEntry* hashTable[HASH_TABLE_SIZE];

// Hash Function (simple polynomial rolling hash)
unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_TABLE_SIZE;
}

// Create a new location node
LocationNode* createLocationNode(const char* location) {
    LocationNode* newNode = (LocationNode*)malloc(sizeof(LocationNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->location = strdup(location);
    newNode->next = NULL;
    return newNode;
}

// Insert a location for an item
void insert(const char* item, const char* location) {
    unsigned long index = hash(item);

    // Check if the item already exists
    if (hashTable[index] != NULL && strcmp(hashTable[index]->item, item) == 0) {
        // Add location to the beginning of the linked list
        LocationNode* newNode = createLocationNode(location);
        newNode->next = hashTable[index]->locations;
        hashTable[index]->locations = newNode;
    } else {
        // Create a new entry
        HashTableEntry* newEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
        if (!newEntry) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        newEntry->item = strdup(item);
        newEntry->locations = createLocationNode(location);
        hashTable[index] = newEntry;
    }
}

// Search for an item and return its locations
LocationNode* search(const char* item) {
    unsigned long index = hash(item);
    if (hashTable[index] != NULL && strcmp(hashTable[index]->item, item) == 0) {
        return hashTable[index]->locations;
    }
    return NULL;
}

// Function to free the linked list of locations
void freeLocations(LocationNode* head) {
    while (head) {
        LocationNode* temp = head;
        head = head->next;
        free(temp->location);
        free(temp);
    }
}

// Function to free the hash table
void freeHashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i]) {
            free(hashTable[i]->item);
            freeLocations(hashTable[i]->locations);
            free(hashTable[i]);
            hashTable[i] = NULL;
        }
    }
}

int main() {
    // Initialize hash table (optional, as it's global and defaults to NULL)
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    // Populate the data (simulating the "cluttered environment")
    insert("keys", "living room table");
    insert("keys", "coat pocket");
    insert("wallet", "backpack");
    insert("phone", "kitchen counter");
    insert("remote", "sofa");
    insert("glasses", "nightstand");
    insert("water bottle", "gym bag");
    insert("water bottle", "car cup holder");

    // Test Cases
    char* targetItem1 = "keys";
    printf("Possible locations for '%s':\n", targetItem1);
    LocationNode* locations1 = search(targetItem1);
    while (locations1) {
        printf("- %s\n", locations1->location);
        locations1 = locations1->next;
    }
    printf("\n");

    char* targetItem2 = "remote";
    printf("Possible locations for '%s':\n", targetItem2);
    LocationNode* locations2 = search(targetItem2);
    while (locations2) {
        printf("- %s\n", locations2->location);
        locations2 = locations2->next;
    }
    printf("\n");

    char* targetItem3 = "book";
    printf("Possible locations for '%s':\n", targetItem3);
    LocationNode* locations3 = search(targetItem3);
    if (locations3 == NULL) {
        printf("'%s' not found in known locations.\n", targetItem3);
    } else {
        while (locations3) {
            printf("- %s\n", locations3->location);
            locations3 = locations3->next;
        }
    }
    printf("\n");

    // Free allocated memory
    freeHashTable();

    return 0;
}
