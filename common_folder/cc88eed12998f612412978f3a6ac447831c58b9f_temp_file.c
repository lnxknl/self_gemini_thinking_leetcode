#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * This program implements a First Unique Character Finder in a stream of characters.
 * Example:
 * Stream: a -> b -> a -> c -> b
 * After 'a': First unique = 'a'
 * After 'b': First unique = 'a'
 * After 'a': First unique = 'b' (because 'a' is now repeated)
 * After 'c': First unique = 'b'
 * After 'b': First unique = 'c' (because 'b' is now repeated)
 */

// Node for our linked list to maintain character order
typedef struct CharNode {
    char character;
    struct CharNode* next;
} CharNode;

// Main structure to track unique characters
typedef struct UniqueCharFinder {
    int frequency[256];         // Count how many times each character appears
    CharNode* uniqueList;       // Linked list of characters in order of appearance
    CharNode* lastUnique;       // Points to last node for quick append
} UniqueCharFinder;

// Create a new character node
CharNode* createNode(char c) {
    CharNode* node = (CharNode*)malloc(sizeof(CharNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->character = c;
    node->next = NULL;
    return node;
}

// Initialize our character finder
UniqueCharFinder* createFinder() {
    UniqueCharFinder* finder = (UniqueCharFinder*)malloc(sizeof(UniqueCharFinder));
    if (finder == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    // Initialize frequency array to 0
    for (int i = 0; i < 256; i++) {
        finder->frequency[i] = 0;
    }
    
    finder->uniqueList = NULL;
    finder->lastUnique = NULL;
    return finder;
}

// Add a character to our stream
void addCharacter(UniqueCharFinder* finder, char c) {
    printf("\nAdding character: '%c'\n", c);
    
    // Increment frequency count
    finder->frequency[c]++;
    
    if (finder->frequency[c] == 1) {
        // First time seeing this character - add to list
        CharNode* newNode = createNode(c);
        
        if (finder->uniqueList == NULL) {
            finder->uniqueList = newNode;
            finder->lastUnique = newNode;
        } else {
            finder->lastUnique->next = newNode;
            finder->lastUnique = newNode;
        }
        
        printf("New unique character added\n");
    }
    else if (finder->frequency[c] == 2) {
        // Character is now repeated - remove from unique list
        printf("Character '%c' is now repeated - removing from unique list\n", c);
        
        // Special case: if it's the first node
        if (finder->uniqueList && finder->uniqueList->character == c) {
            CharNode* temp = finder->uniqueList;
            finder->uniqueList = finder->uniqueList->next;
            free(temp);
            
            // Update lastUnique if list is now empty
            if (finder->uniqueList == NULL) {
                finder->lastUnique = NULL;
            }
            return;
        }
        
        // Search for the character in the list
        CharNode* current = finder->uniqueList;
        CharNode* prev = NULL;
        
        while (current != NULL) {
            if (current->character == c) {
                prev->next = current->next;
                if (current == finder->lastUnique) {
                    finder->lastUnique = prev;
                }
                free(current);
                break;
            }
            prev = current;
            current = current->next;
        }
    }
}

// Get the first unique character in our stream
char getFirstUnique(UniqueCharFinder* finder) {
    if (finder->uniqueList == NULL) {
        return '#';  // Return '#' if no unique character exists
    }
    return finder->uniqueList->character;
}

// Clean up memory
void cleanupFinder(UniqueCharFinder* finder) {
    CharNode* current = finder->uniqueList;
    while (current != NULL) {
        CharNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(finder);
}

int main() {
    // Create our unique character finder
    UniqueCharFinder* finder = createFinder();
    
    // Test cases
    printf("=== Starting Character Stream Processing ===\n");
    
    // Test Case 1: Single character
    addCharacter(finder, 'a');
    printf("First unique after 'a': %c\n", getFirstUnique(finder));
    
    // Test Case 2: Two different characters
    addCharacter(finder, 'b');
    printf("First unique after 'b': %c\n", getFirstUnique(finder));
    
    // Test Case 3: Repeated character
    addCharacter(finder, 'a');
    printf("First unique after second 'a': %c\n", getFirstUnique(finder));
    
    // Test Case 4: Add new character
    addCharacter(finder, 'c');
    printf("First unique after 'c': %c\n", getFirstUnique(finder));
    
    // Test Case 5: Another repeated character
    addCharacter(finder, 'b');
    printf("First unique after second 'b': %c\n", getFirstUnique(finder));
    
    // Clean up
    cleanupFinder(finder);
    
    return 0;
}
