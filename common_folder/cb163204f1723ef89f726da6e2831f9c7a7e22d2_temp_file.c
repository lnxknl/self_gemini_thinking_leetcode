#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define a structure for the hash table entry
typedef struct RelationshipNode {
    int person1Id;
    int person2Id;
    int status;
    struct RelationshipNode* next;
} RelationshipNode;

// Define the hash table structure
#define HASH_TABLE_SIZE 101 // A prime number for better distribution
RelationshipNode* hashTable[HASH_TABLE_SIZE];

// Hash function (simple modulo operation)
unsigned int hash(int person1Id, int person2Id) {
    // Ensure consistent ordering for the key
    int minId = (person1Id < person2Id) ? person1Id : person2Id;
    int maxId = (person1Id > person2Id) ? person1Id : person2Id;
    unsigned int key = (unsigned int)minId * HASH_TABLE_SIZE + maxId; // Combine IDs
    return key % HASH_TABLE_SIZE;
}

// Function to establish or update a relationship
void establishRelationship(int person1Id, int person2Id, int initialStatus) {
    unsigned int index = hash(person1Id, person2Id);
    RelationshipNode* current = hashTable[index];
    RelationshipNode* prev = NULL;

    while (current != NULL) {
        if ((current->person1Id == person1Id && current->person2Id == person2Id) ||
            (current->person1Id == person2Id && current->person2Id == person1Id)) {
            current->status = initialStatus;
            return;
        }
        prev = current;
        current = current->next;
    }

    RelationshipNode* newNode = (RelationshipNode*)malloc(sizeof(RelationshipNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->person1Id = person1Id;
    newNode->person2Id = person2Id;
    newNode->status = initialStatus;
    newNode->next = NULL;

    if (prev) {
        prev->next = newNode;
    } else {
        hashTable[index] = newNode;
    }
}

// Function to get the relationship status
int getRelationshipStatus(int person1Id, int person2Id) {
    unsigned int index = hash(person1Id, person2Id);
    RelationshipNode* current = hashTable[index];

    while (current != NULL) {
        if ((current->person1Id == person1Id && current->person2Id == person2Id) ||
            (current->person1Id == person2Id && current->person2Id == person1Id)) {
            return current->status;
        }
        current = current->next;
    }
    return -999; // Relationship not found
}

// Function to update the relationship status
void updateRelationshipStatus(int person1Id, int person2Id, int newStatus) {
    unsigned int index = hash(person1Id, person2Id);
    RelationshipNode* current = hashTable[index];

    while (current != NULL) {
        if ((current->person1Id == person1Id && current->person2Id == person2Id) ||
            (current->person1Id == person2Id && current->person2Id == person1Id)) {
            current->status = newStatus;
            return;
        }
        current = current->next;
    }
    printf("Relationship between %d and %d not found for update.\n", person1Id, person2Id);
}

// Function to get all relationships for a person
void getAllRelationshipsForPerson(int personId) {
    printf("Relationships for person %d:\n", personId);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        RelationshipNode* current = hashTable[i];
        while (current != NULL) {
            if (current->person1Id == personId || current->person2Id == personId) {
                int otherPersonId = (current->person1Id == personId) ? current->person2Id : current->person1Id;
                printf("  With person %d: Status = %d\n", otherPersonId, current->status);
            }
            current = current->next;
        }
    }
}

// Function to initialize the hash table
void initializeHashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

int main() {
    initializeHashTable();

    // Test Example Input (based on the image's implied scenario)
    int personA = 1; // Representing the woman
    int personB = 2; // Representing the man

    // Initial state: Perhaps some level of connection exists
    establishRelationship(personA, personB, 2); // Assume they are friends initially
    printf("Initial status between %d and %d: %d\n", personA, personB, getRelationshipStatus(personA, personB));

    // The interaction in the image suggests a moment of support
    updateRelationshipStatus(personA, personB, 3); // Status updated to 'Close Friends' or similar
    printf("Status after interaction: %d\n", getRelationshipStatus(personA, personB));

    // Introduce other individuals
    int personC = 3;
    establishRelationship(personA, personC, 1); // Acquaintances
    establishRelationship(personB, personC, -1); // Conflict

    getAllRelationshipsForPerson(personA);
    getAllRelationshipsForPerson(personB);
    getAllRelationshipsForPerson(personC);

    printf("Status between %d and %d: %d\n", personA, personC, getRelationshipStatus(personA, personC));
    printf("Status between %d and %d: %d\n", personB, personC, getRelationshipStatus(personB, personC));

    // Trying to get status of a non-existent relationship
    printf("Status between %d and %d: %d\n", 4, 5, getRelationshipStatus(4, 5));

    return 0;
}
