#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure for linked list node (idea sequence)
typedef struct IdeaNode {
    char* action;
    struct IdeaNode* next;
} IdeaNode;

// Structure for hash table entry (constraints)
typedef struct ConstraintEntry {
    char* key;
    bool value;
    struct ConstraintEntry* next; // For handling collisions (simple chaining)
} ConstraintEntry;

#define HASH_TABLE_SIZE 10 // Simple hash table size

// Simple hash function
unsigned int hash(const char* key) {
    unsigned int hashVal = 0;
    while (*key != '\0') {
        hashVal = (hashVal << 5) + *key++;
    }
    return hashVal % HASH_TABLE_SIZE;
}

// Function to insert a constraint into the hash table
void insertConstraint(ConstraintEntry** hashTable, const char* key, bool value) {
    unsigned int index = hash(key);
    ConstraintEntry* newEntry = (ConstraintEntry*)malloc(sizeof(ConstraintEntry));
    if (!newEntry) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newEntry->key = strdup(key);
    newEntry->value = value;
    newEntry->next = hashTable[index];
    hashTable[index] = newEntry;
}

// Function to get the value of a constraint
bool getConstraintValue(ConstraintEntry** hashTable, const char* key) {
    unsigned int index = hash(key);
    ConstraintEntry* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return false; // Constraint not found or inactive
}

// Structure for linked list node (validation messages)
typedef struct MessageNode {
    char* message;
    struct MessageNode* next;
} MessageNode;

// Function to add a message to the validation messages list
void addMessage(MessageNode** head, const char* message) {
    MessageNode* newNode = (MessageNode*)malloc(sizeof(MessageNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->message = strdup(message);
    newNode->next = *head;
    *head = newNode;
}

// Function to validate the idea sequence against constraints
MessageNode* validateIdea(IdeaNode* idea_sequence, ConstraintEntry** constraints) {
    MessageNode* validation_messages = NULL;
    IdeaNode* current_action = idea_sequence;

    while (current_action != NULL) {
        bool violation_found = false;
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            ConstraintEntry* current_constraint = constraints[i];
            while (current_constraint != NULL) {
                if (current_constraint->value && strcmp(current_action->action, current_constraint->key) == 0) {
                    char message[200];
                    sprintf(message, "Action '%s' violates constraint '%s'.", current_action->action, current_constraint->key);
                    addMessage(&validation_messages, message);
                    violation_found = true;
                    break; // Move to the next action
                }
                current_constraint = current_constraint->next;
            }
            if (violation_found) break;
        }
        if (!violation_found) {
            char message[100];
            sprintf(message, "Action '%s' is valid.", current_action->action);
            addMessage(&validation_messages, message);
        }
        current_action = current_action->next;
    }

    return validation_messages;
}

// Function to print the validation messages
void printMessages(MessageNode* head) {
    MessageNode* current = head;
    while (current != NULL) {
        printf("%s\n", current->message);
        current = current->next;
    }
}

// Function to free the linked list
void freeLinkedList(IdeaNode* head) {
    while (head != NULL) {
        IdeaNode* temp = head;
        head = head->next;
        free(temp->action);
        free(temp);
    }
}

// Function to free the hash table
void freeHashTable(ConstraintEntry** hashTable) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ConstraintEntry* current = hashTable[i];
        while (current != NULL) {
            ConstraintEntry* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
}

// Function to free the validation messages list
void freeMessages(MessageNode* head) {
    while (head != NULL) {
        MessageNode* temp = head;
        head = head->next;
        free(temp->message);
        free(temp);
    }
}

int main() {
    // Example Input
    IdeaNode* idea_sequence = NULL;
    // Add actions to the idea sequence (in reverse order for easier insertion at head)
    IdeaNode* action3 = (IdeaNode*)malloc(sizeof(IdeaNode));
    action3->action = strdup("Implement Feature X");
    action3->next = idea_sequence;
    idea_sequence = action3;

    IdeaNode* action2 = (IdeaNode*)malloc(sizeof(IdeaNode));
    action2->action = strdup("Deploy to Production");
    action2->next = idea_sequence;
    idea_sequence = action2;

    IdeaNode* action1 = (IdeaNode*)malloc(sizeof(IdeaNode));
    action1->action = strdup("Write Code");
    action1->next = idea_sequence;
    idea_sequence = action1;

    ConstraintEntry* constraints[HASH_TABLE_SIZE] = {NULL};
    insertConstraint(constraints, "Deploy to Production", false); // Constraint inactive
    insertConstraint(constraints, "Write Buggy Code", true);     // Constraint active
    insertConstraint(constraints, "Implement Feature Y", true);  // Constraint active

    // Validate the idea
    MessageNode* messages = validateIdea(idea_sequence, constraints);

    // Print the validation messages
    printf("Validation Results:\n");
    printMessages(messages);

    // Free allocated memory
    freeLinkedList(idea_sequence);
    freeHashTable(constraints);
    freeMessages(messages);

    return 0;
}
