#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for a node in the linked list
typedef struct Node {
    char data;
    struct Node* next;
} Node;

// Structure for the FirstUnique data structure
typedef struct {
    int char_counts[256]; // Assuming ASCII characters
    Node* head;
    Node* tail;
} FirstUnique;

// Function to initialize the FirstUnique data structure
FirstUnique* createFirstUnique() {
    FirstUnique* fu = (FirstUnique*)malloc(sizeof(FirstUnique));
    if (!fu) {
        return NULL;
    }
    for (int i = 0; i < 256; i++) {
        fu->char_counts[i] = 0;
    }
    fu->head = NULL;
    fu->tail = NULL;
    return fu;
}

// Function to add a character to the stream
void add(FirstUnique* fu, char character) {
    fu->char_counts[character]++;

    if (fu->char_counts[character] == 1) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (!newNode) return;
        newNode->data = character;
        newNode->next = NULL;

        if (!fu->tail) {
            fu->head = newNode;
            fu->tail = newNode;
        } else {
            fu->tail->next = newNode;
            fu->tail = newNode;
        }
    } else if (fu->char_counts[character] == 2) {
        // Remove the character from the linked list if it's present
        Node* current = fu->head;
        Node* prev = NULL;
        while (current) {
            if (current->data == character) {
                if (prev) {
                    prev->next = current->next;
                    if (!current->next) {
                        fu->tail = prev;
                    }
                } else {
                    fu->head = current->next;
                    if (!fu->head) {
                        fu->tail = NULL;
                    }
                }
                free(current);
                break; // Only remove the first occurrence (should be the only one)
            }
            prev = current;
            current = current->next;
        }
    }
}

// Function to show the first unique character
char showFirstUnique(FirstUnique* fu) {
    if (fu->head) {
        return fu->head->data;
    } else {
        return '#';
    }
}

// Function to free the memory used by the FirstUnique data structure
void freeFirstUnique(FirstUnique* fu) {
    Node* current = fu->head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(fu);
}

int main() {
    FirstUnique* firstUnique = createFirstUnique();

    add(firstUnique, 'a');
    printf("First Unique: %c\n", showFirstUnique(firstUnique)); // Output: a

    add(firstUnique, 'b');
    printf("First Unique: %c\n", showFirstUnique(firstUnique)); // Output: a

    add(firstUnique, 'a');
    printf("First Unique: %c\n", showFirstUnique(firstUnique)); // Output: b

    add(firstUnique, 'c');
    printf("First Unique: %c\n", showFirstUnique(firstUnique)); // Output: b

    add(firstUnique, 'b');
    printf("First Unique: %c\n", showFirstUnique(firstUnique)); // Output: c

    freeFirstUnique(firstUnique);

    return 0;
}
