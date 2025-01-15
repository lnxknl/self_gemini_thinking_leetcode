#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ALPHABET_SIZE 26

// Trie Node Structure
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

// Function to create a new Trie node
TrieNode* createNode() {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    newNode->isEndOfWord = 0;
    return newNode;
}

// Function to insert a word into the Trie
void insert(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!curr->children[index]) {
            curr->children[index] = createNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;
}

// Function to calculate the minimum of three integers
int min(int a, int b, int c) {
    return (a < b) ? (a < c ? a : c) : (b < c ? b : c);
}

// Function to calculate Levenshtein distance
int editDistance(const char* s1, const char* s2, int m, int n) {
    int dp[m + 1][n + 1];

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0)
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min(dp[i - 1][j],        // Deletion
                                   dp[i][j - 1],        // Insertion
                                   dp[i - 1][j - 1]);    // Substitution
        }
    }
    return dp[m][n];
}

// Structure to hold the best match found so far
typedef struct {
    char* word;
    int distance;
} BestMatch;

// Global variable to store the best match
BestMatch bestMatch;

// Recursive function to search the Trie with edit distance
void searchTrieWithEditDistance(TrieNode* node, const char* target, char* currentWord, int depth) {
    if (node->isEndOfWord) {
        int dist = editDistance(currentWord, target, strlen(currentWord), strlen(target));
        if (dist < bestMatch.distance) {
            free(bestMatch.word);
            bestMatch.word = strdup(currentWord);
            bestMatch.distance = dist;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            currentWord[depth] = 'a' + i;
            currentWord[depth + 1] = '\0';
            searchTrieWithEditDistance(node->children[i], target, currentWord, depth + 1);
        }
    }
}

// Function to find the closest match in the Trie
char* findClosestMatch(TrieNode* root, const char* target) {
    bestMatch.distance = INT_MAX;
    bestMatch.word = NULL;
    char currentWord[100]; // Assuming max word length is 99

    searchTrieWithEditDistance(root, target, currentWord, 0);
    return bestMatch.word;
}

int main() {
    // Sample book titles
    char* bookTitles[] = {"algorithms", "data structures", "artificial intelligence", "machine learning", "deep learning", "computer science"};
    int numTitles = sizeof(bookTitles) / sizeof(bookTitles[0]);

    // Build the Trie
    TrieNode* root = createNode();
    for (int i = 0; i < numTitles; i++) {
        insert(root, bookTitles[i]);
    }

    // Target title (representing the white object)
    char* targetTitle = "datr structure";

    // Find the closest match
    char* closestMatch = findClosestMatch(root, targetTitle);

    if (closestMatch) {
        printf("Closest match for '%s': '%s'\n", targetTitle, closestMatch);
        free(closestMatch);
    } else {
        printf("No matches found for '%s'\n", targetTitle);
    }

    return 0;
}
