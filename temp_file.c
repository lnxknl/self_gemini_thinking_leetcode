#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct to represent a person
typedef struct {
    int personality; // -1 (pessimistic) to 1 (optimistic)
    int mood;       // -5 (very negative) to 5 (very positive)
    int communication_style; // 0 (indirect) to 1 (direct)
} Person;

// Simple sentiment analysis (hardcoded for demonstration)
int get_sentiment(const char *text) {
    if (strstr(text, "happy") || strstr(text, "good") || strstr(text, "agree")) return 1;
    if (strstr(text, "sad") || strstr(text, "bad") || strstr(text, "disagree")) return -1;
    return 0;
}

// Function to update the mood of a person
void update_state(Person *p, const char *interaction, const Person *other_person) {
    int sentiment = get_sentiment(interaction);
    p->mood += sentiment * (other_person->communication_style + 1); // Direct has more impact
    // Add some influence from personality
    p->mood += p->personality * sentiment;
    // Clamp mood
    if (p->mood > 5) p->mood = 5;
    if (p->mood < -5) p->mood = -5;
}

// Function to simulate the interaction
int simulate_interaction(Person *person1, Person *person2, char **interactions, int num_interactions) {
    for (int i = 0; i < num_interactions; i++) {
        if (i % 2 == 0) { // Person 1 speaks
            update_state(person2, interactions[i], person1);
        } else { // Person 2 speaks
            update_state(person1, interactions[i], person2);
        }
        // Check for positive resolution (simplified)
        if (person1->mood > 2 && person2->mood > 2) {
            return 1; // Positive resolution
        }
    }
    return 0; // No positive resolution
}

int main() {
    Person alice = {1, 0, 1}; // Optimistic, neutral mood, direct communicator
    Person bob = {-1, 0, 0};  // Pessimistic, neutral mood, indirect communicator

    char *interactions[] = {
        "Hi, how are you?",
        "I'm okay.",
        "That's good to hear.",
        "Yeah, I guess.",
        "Did you like the movie?",
        "It was alright.",
        "I thought it was great!",
        "Hmm, maybe.",
        "So, we're on for tomorrow?",
        "Sure, if you want."
    };
    int num_interactions = sizeof(interactions) / sizeof(interactions[0]);

    int outcome = simulate_interaction(&alice, &bob, interactions, num_interactions);

    if (outcome == 1) {
        printf("Likelihood of positive resolution: High\n");
    } else {
        printf("Likelihood of positive resolution: Low\n");
    }

    return 0;
}
