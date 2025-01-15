#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// --- Data Structures ---

typedef struct {
    int user_id;
    char** posts;
    int num_posts;
} User;

typedef struct {
    int user_id;
    float hesitation_score;
} Result;

// --- Trie Implementation (Simplified for Keyword Counting) ---

typedef struct TrieNode {
    struct TrieNode* children[26]; // Assuming lowercase English letters
    bool isEndOfWord;
} TrieNode;

TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    node->isEndOfWord = false;
    return node;
}

void insert(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = true;
}

int countOccurrences(TrieNode* root, const char* text) {
    int count = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++) {
        TrieNode* curr = root;
        for (int j = i; j < n; j++) {
            if (text[j] >= 'a' && text[j] <= 'z') {
                int index = text[j] - 'a';
                if (curr->children[index] == NULL) {
                    break;
                }
                curr = curr->children[index];
                if (curr->isEndOfWord) {
                    count++;
                }
            } else {
                break; // Consider only contiguous lowercase words for simplicity
            }
        }
    }
    return count;
}

// --- Main Logic ---

Result* calculateHesitationScores(User* users, int num_users, int** friendships, int num_friendships, char** keywords, int num_keywords) {
    // 1. Preprocessing: Create user map and adjacency list
    User* user_map = (User*)malloc(sizeof(User) * num_users);
    int* adj_list[num_users];
    int adj_counts[num_users];
    for (int i = 0; i < num_users; i++) {
        user_map[i] = users[i];
        adj_list[i] = NULL;
        adj_counts[i] = 0;
    }

    for (int i = 0; i < num_friendships; i++) {
        int user1_id = friendships[i][0];
        int user2_id = friendships[i][1];

        // Find indices in user_map
        int index1 = -1, index2 = -1;
        for (int j = 0; j < num_users; j++) {
            if (user_map[j].user_id == user1_id) index1 = j;
            if (user_map[j].user_id == user2_id) index2 = j;
        }

        if (index1 != -1 && index2 != -1) {
            // Add to adjacency list (undirected)
            adj_counts[index1]++;
            adj_list[index1] = (int*)realloc(adj_list[index1], sizeof(int) * adj_counts[index1]);
            adj_list[index1][adj_counts[index1] - 1] = user2_id;

            adj_counts[index2]++;
            adj_list[index2] = (int*)realloc(adj_list[index2], sizeof(int) * adj_counts[index2]);
            adj_list[index2][adj_counts[index2] - 1] = user1_id;
        }
    }

    // 2. Build Trie from keywords
    TrieNode* root = createTrieNode();
    for (int i = 0; i < num_keywords; i++) {
        insert(root, keywords[i]);
    }

    // 3. Calculate initial hesitation scores (based on text)
    float* hesitation_scores = (float*)malloc(sizeof(float) * num_users);
    for (int i = 0; i < num_users; i++) {
        int total_keyword_count = 0;
        for (int j = 0; j < users[i].num_posts; j++) {
            total_keyword_count += countOccurrences(root, users[i].posts[j]);
        }
        hesitation_scores[i] = (float)total_keyword_count; // Simple count as initial score
    }

    // 4. Iterative score refinement
    int num_iterations = 10;
    float alpha = 0.5;
    for (int iter = 0; iter < num_iterations; iter++) {
        float* new_hesitation_scores = (float*)malloc(sizeof(float) * num_users);
        for (int i = 0; i < num_users; i++) {
            float friend_score_sum = 0;
            int friend_count = 0;
            for (int j = 0; j < adj_counts[i]; j++) {
                int friend_id = adj_list[i][j];
                for (int k = 0; k < num_users; k++) {
                    if (user_map[k].user_id == friend_id) {
                        friend_score_sum += hesitation_scores[k];
                        friend_count++;
                        break;
                    }
                }
            }
            float average_friend_score = (friend_count > 0) ? friend_score_sum / friend_count : 0;
            new_hesitation_scores[i] = alpha * hesitation_scores[i] + (1 - alpha) * average_friend_score;
        }
        free(hesitation_scores);
        hesitation_scores = new_hesitation_scores;
    }

    // 5. Prepare output
    Result* results = (Result*)malloc(sizeof(Result) * num_users);
    for (int i = 0; i < num_users; i++) {
        results[i].user_id = users[i].user_id;
        results[i].hesitation_score = hesitation_scores[i];
    }

    // Clean up memory (Trie needs proper freeing, omitted for brevity)
    free(hesitation_scores);
    for (int i = 0; i < num_users; i++) {
        free(adj_list[i]);
    }

    return results;
}

// --- Test Example ---
int main() {
    // Example Input
    User users[] = {
        {1, (char*[]){"I'm not sure about marriage anymore.", "Single life is great."}, 2},
        {2, (char*[]){"My friend's divorce was terrible.", "Focusing on my career."}, 2},
        {3, (char*[]){"Happy for my married friends.", "Thinking about the future."}, 2},
        {4, (char*[]){"Marriage seems like a trap.", "Enjoying my freedom."}, 2}
    };
    int num_users = sizeof(users) / sizeof(users[0]);

    int friendships[][2] = {{1, 2}, {2, 3}, {4, 1}};
    int num_friendships = sizeof(friendships) / sizeof(friendships[0]);

    char* keywords[] = {"marriage", "single", "divorce", "trap", "freedom"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    Result* results = calculateHesitationScores(users, num_users, (int**)friendships, num_friendships, keywords, num_keywords);

    printf("Hesitation Scores:\n");
    for (int i = 0; i < num_users; i++) {
        printf("User ID: %d, Score: %.2f\n", results[i].user_id, results[i].hesitation_score);
    }

    free(results);
    return 0;
}
