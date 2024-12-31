#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for a Knowledge Node
typedef struct KnowledgeNode {
    int id;
    int* prerequisites;
    int numPrerequisites;
} KnowledgeNode;

// Structure for a Hash Table entry
typedef struct HashEntry {
    int key;
    KnowledgeNode* value;
    struct HashEntry* next;
} HashEntry;

// Hash Table structure
typedef struct HashTable {
    int size;
    HashEntry** buckets;
} HashTable;

// Simple hash function
unsigned int hash(int key, int size) {
    return key % size;
}

// Function to create a new Hash Table
HashTable* createHashTable(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = size;
    ht->buckets = (HashEntry**)calloc(size, sizeof(HashEntry*));
    return ht;
}

// Function to insert a key-value pair into the Hash Table
void insert(HashTable* ht, int key, KnowledgeNode* value) {
    unsigned int index = hash(key, ht->size);
    HashEntry* entry = (HashEntry*)malloc(sizeof(HashEntry));
    entry->key = key;
    entry->value = value;
    entry->next = ht->buckets[index];
    ht->buckets[index] = entry;
}

// Function to search for a value by key in the Hash Table
KnowledgeNode* search(HashTable* ht, int key) {
    unsigned int index = hash(key, ht->size);
    HashEntry* current = ht->buckets[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

// DFS function to check if knowledge can be understood
int canUnderstand(int nodeId, HashTable* graph, HashTable* memo, HashTable* visited) {
    if (search(memo, nodeId) != NULL) {
        return 1; // Already computed
    }

    if (search(visited, nodeId) != NULL) {
        return 0; // Cycle detected
    }

    KnowledgeNode* node = search(graph, nodeId);
    if (node == NULL) {
        return 0; // Knowledge not found
    }

    // Mark as visiting
    HashEntry* visited_entry = (HashEntry*)malloc(sizeof(HashEntry));
    visited_entry->key = nodeId;
    visited_entry->value = NULL; // Value not important for visited
    visited_entry->next = visited->buckets[hash(nodeId, visited->size)];
    visited->buckets[hash(nodeId, visited->size)] = visited_entry;

    for (int i = 0; i < node->numPrerequisites; i++) {
        if (!canUnderstand(node->prerequisites[i], graph, memo, visited)) {
            return 0;
        }
    }

    // Mark as understood in memo
    HashEntry* memo_entry = (HashEntry*)malloc(sizeof(HashEntry));
    memo_entry->key = nodeId;
    memo_entry->value = NULL; // Value not important for memo, presence indicates understood
    memo_entry->next = memo->buckets[hash(nodeId, memo->size)];
    memo->buckets[hash(nodeId, memo->size)] = memo_entry;

    // Remove from visited
    HashEntry* prev = NULL;
    HashEntry* curr = visited->buckets[hash(nodeId, visited->size)];
    while (curr != NULL) {
        if (curr->key == nodeId) {
            if (prev == NULL) {
                visited->buckets[hash(nodeId, visited->size)] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return 1;
}

int canUnderstandKnowledge(int targetId, KnowledgeNode* nodes, int numNodes) {
    HashTable* graph = createHashTable(numNodes * 2); // Size for the graph
    for (int i = 0; i < numNodes; i++) {
        insert(graph, nodes[i].id, &nodes[i]);
    }

    HashTable* memo = createHashTable(numNodes * 2); // Memoization table
    HashTable* visited = createHashTable(numNodes * 2); // Cycle detection

    return canUnderstand(targetId, graph, memo, visited);
}

int main() {
    // Example Input
    KnowledgeNode nodes[] = {
        {1, NULL, 0},
        {2, (int[]){1}, 1},
        {3, (int[]){1, 2}, 2},
        {4, (int[]){3, 5}, 2},
        {5, NULL, 0}
    };
    int numNodes = sizeof(nodes) / sizeof(nodes[0]);

    int targetId = 4;
    if (canUnderstandKnowledge(targetId, nodes, numNodes)) {
        printf("It is possible to understand knowledge with ID %d\n", targetId);
    } else {
        printf("It is NOT possible to understand knowledge with ID %d\n", targetId);
    }

    // Example with a cycle
    KnowledgeNode nodes_cycle[] = {
        {1, (int[]){2}, 1},
        {2, (int[]){1}, 1}
    };
    int numNodes_cycle = sizeof(nodes_cycle) / sizeof(nodes_cycle[0]);
    int targetId_cycle = 1;
    if (canUnderstandKnowledge(targetId_cycle, nodes_cycle, numNodes_cycle)) {
        printf("It is possible to understand knowledge with ID %d\n", targetId_cycle);
    } else {
        printf("It is NOT possible to understand knowledge with ID %d (cycle)\n", targetId_cycle);
    }

    return 0;
}
