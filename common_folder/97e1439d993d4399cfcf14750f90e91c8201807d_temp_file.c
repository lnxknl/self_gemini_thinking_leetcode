#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

// Trie Node Structure
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfFragment;
    char character;
} TrieNode;

// Function to create a new Trie node
TrieNode* createNode(char ch) {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    newNode->isEndOfFragment = false;
    newNode->character = ch;
    return newNode;
}

// Function to insert a fragment into the Trie
void insertFragment(TrieNode* root, const char* fragment) {
    TrieNode* curr = root;
    for (int i = 0; fragment[i] != '\0'; i++) {
        int index = fragment[i] - 'a';
        if (!curr->children[index]) {
            curr->children[index] = createNode(fragment[i]);
        }
        curr = curr->children[index];
    }
    curr->isEndOfFragment = true;
}

// Function to perform DFS and check for the signature
bool findSignature(TrieNode* node, const char* signature, int signatureIndex) {
    if (!node) {
        return false;
    }

    if (signatureIndex == strlen(signature)) {
        return true; // Signature found
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] && node->children[i]->character == signature[signatureIndex]) {
            if (findSignature(node->children[i], signature, signatureIndex + 1)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    // Example of a fragmented signal network (Trie)
    TrieNode* root = createNode('\0'); // Root doesn't have a character

    // Insert fragmented signals
    insertFragment(root, "ab");
    insertFragment(root, "cd");
    insertFragment(root, "ef");

    // Signature of the complete signal we are looking for
    const char* targetSignature = "ab";

    // Try to find the signature in the Trie
    if (findSignature(root, targetSignature, 0)) {
        printf("The complete signal '%s' is present in the network.\n", targetSignature);
    } else {
        printf("The complete signal '%s' is NOT present in the network.\n", targetSignature);
    }

    // --- Another Test Case ---
    const char* targetSignature2 = "ac";
    if (findSignature(root, targetSignature2, 0)) {
        printf("The complete signal '%s' is present in the network.\n", targetSignature2);
    } else {
        printf("The complete signal '%s' is NOT present in the network.\n", targetSignature2);
    }

    return 0;
}
