#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26 // Assuming lowercase English letters
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// Trie Node Structure
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

// Creates a new Trie node
TrieNode *createNode() {
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    if (newNode) {
        newNode->isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Inserts a string into the Trie
void insert(TrieNode *root, const char *key) {
    TrieNode *curr = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = CHAR_TO_INDEX(key[i]);
        if (!curr->children[index]) {
            curr->children[index] = createNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = true;
}

// Searches for a string in the Trie
bool search(TrieNode *root, const char *key) {
    TrieNode *curr = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = CHAR_TO_INDEX(key[i]);
        if (!curr->children[index]) {
            return false;
        }
        curr = curr->children[index];
    }
    return (curr != NULL && curr->isEndOfWord);
}

int main() {
    // The "darkness" - a stream of identifiers
    char *identifiers[] = {"randomstring1", "anotherone", "notthetarget", "thisisthetarget", "yetanother"};
    int num_identifiers = sizeof(identifiers) / sizeof(identifiers[0]);

    // The "spark" - the target identifier
    char *target = "thisisthetarget";

    // Initialize the Trie
    TrieNode *root = createNode();

    // Process the stream of identifiers
    for (int i = 0; i < num_identifiers; i++) {
        insert(root, identifiers[i]);
    }

    // Search for the target identifier
    if (search(root, target)) {
        printf("The 'spark' (target identifier '%s') was found in the darkness.\n", target);
    } else {
        printf("The 'spark' (target identifier '%s') was not found.\n", target);
    }

    // Example of a negative case
    char *negative_target = "notpresent";
    if (search(root, negative_target)) {
        printf("Error: '%s' should not be found.\n", negative_target);
    } else {
        printf("'%s' was correctly not found.\n", negative_target);
    }

    // Free the Trie memory (important in real-world applications)
    // (Implementation for freeing Trie memory is more involved and omitted for brevity)

    return 0;
}
