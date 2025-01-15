#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent attractiveness factors
typedef struct {
    int appearance;
    int personality;
    int skills;
} AttractivenessFactors;

// Structure to represent a person
typedef struct {
    int id;
    char name[50];
    AttractivenessFactors factors;
    int overall_score;
} Person;

// Comparison function for qsort (descending order)
int comparePersons(const void *a, const void *b) {
    Person *personA = (Person *)a;
    Person *personB = (Person *)b;
    return personB->overall_score - personA->overall_score;
}

// Function to calculate overall score
void calculateOverallScore(Person *person) {
    person->overall_score = person->factors.appearance + person->factors.personality + person->factors.skills;
}

int main() {
    // Sample Input
    Person people[] = {
        {1, "Alice", {8, 7, 6}, 0},
        {2, "Bob", {6, 9, 8}, 0},
        {3, "Cathy", {7, 6, 9}, 0},
        {4, "David", {5, 5, 5}, 0}
    };
    int numPeople = sizeof(people) / sizeof(people[0]);
    int targetId = 4; // Let's say David is the target person

    // Calculate overall scores
    for (int i = 0; i < numPeople; i++) {
        calculateOverallScore(&people[i]);
    }

    // Sort the people based on overall score
    qsort(people, numPeople, sizeof(Person), comparePersons);

    // Find the target person and their rank
    int targetRank = -1;
    Person *targetPerson = NULL;
    for (int i = 0; i < numPeople; i++) {
        if (people[i].id == targetId) {
            targetRank = i + 1;
            targetPerson = &people[i];
            break;
        }
    }

    if (targetRank != -1) {
        printf("Target Person: %s (ID: %d)\n", targetPerson->name, targetPerson->id);
        printf("Rank: %d\n", targetRank);

        printf("Areas for Improvement:\n");
        double threshold = 0.2; // Define a threshold for significant difference

        for (int i = 0; i < targetRank - 1; i++) { // Iterate through people ranked higher
            if ((double)(people[i].factors.appearance - targetPerson->factors.appearance) / people[i].factors.appearance > threshold) {
                printf("- Appearance\n");
                break; // Print only once per factor
            }
        }
         for (int i = 0; i < targetRank - 1; i++) { // Iterate through people ranked higher
            if ((double)(people[i].factors.personality - targetPerson->factors.personality) / people[i].factors.personality > threshold) {
                printf("- Personality\n");
                break;
            }
        }
        for (int i = 0; i < targetRank - 1; i++) { // Iterate through people ranked higher
            if ((double)(people[i].factors.skills - targetPerson->factors.skills) / people[i].factors.skills > threshold) {
                printf("- Skills\n");
                break;
            }
        }

        if (targetRank == numPeople) {
            printf("No areas for improvement compared to those ranked higher (already the lowest ranked).\n");
        }
    } else {
        printf("Target person not found.\n");
    }

    return 0;
}
