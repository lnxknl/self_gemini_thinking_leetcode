#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// A simple hash table implementation using an array of linked lists
#define HASH_TABLE_SIZE 101 // A prime number for better distribution

typedef struct HashNode {
    char *password;
    struct HashNode *next;
} HashNode;

HashNode *hash_table[HASH_TABLE_SIZE];

// Simple hash function
unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_TABLE_SIZE;
}

// Insert a password into the hash table
void insert_password(const char *password) {
    unsigned int index = hash(password);
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_node->password = strdup(password); // Allocate memory for the password
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}

// Check if a password exists in the hash table
bool is_weak_password(const char *potential_password) {
    unsigned int index = hash(potential_password);
    HashNode *current = hash_table[index];
    while (current) {
        if (strcmp(current->password, potential_password) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to free the hash table memory
void free_hash_table() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *current = hash_table[i];
        while (current) {
            HashNode *temp = current;
            free(temp->password);
            current = current->next;
            free(temp);
        }
        hash_table[i] = NULL;
    }
}

int main() {
    // Initialize the hash table
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }

    // Example list of weak passwords
    char *weak_passwords[] = {"password", "123456", "qwerty", "admin", "111111"};
    int num_weak_passwords = sizeof(weak_passwords) / sizeof(weak_passwords[0]);

    // Populate the hash table with weak passwords
    for (int i = 0; i < num_weak_passwords; i++) {
        insert_password(weak_passwords[i]);
    }

    // Test cases
    char *test_passwords[] = {"mysecurepassword", "password", "qwertyuiop", "123456"};
    int num_test_passwords = sizeof(test_passwords) / sizeof(test_passwords[0]);

    for (int i = 0; i < num_test_passwords; i++) {
        printf("Checking if '%s' is a weak password: %s\n", test_passwords[i], is_weak_password(test_passwords[i]) ? "true" : "false");
    }

    // Free the allocated memory
    free_hash_table();

    return 0;
}
