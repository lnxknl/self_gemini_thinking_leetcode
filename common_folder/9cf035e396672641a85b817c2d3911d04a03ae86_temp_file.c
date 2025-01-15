#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure to represent a suspect
typedef struct {
    int id;
    char clothing_color[20];
    char gender[10];
    char hair_color[20];
    int approximate_age;
} Suspect;

// Structure to represent a witness report
typedef struct {
    char clothing_color[20];
    char gender[10];
    char hair_color[20];
    int approximate_age;
    bool has_clothing_color;
    bool has_gender;
    bool has_hair_color;
    bool has_approximate_age;
} WitnessReport;

// Function to calculate a match score between a suspect and a witness report
int calculate_match_score(const Suspect *suspect, const WitnessReport *report) {
    int score = 0;

    if (report->has_clothing_color && strcmp(suspect->clothing_color, report->clothing_color) == 0) {
        score++;
    }
    if (report->has_gender && strcmp(suspect->gender, report->gender) == 0) {
        score++;
    }
    if (report->has_hair_color && strcmp(suspect->hair_color, report->hair_color) == 0) {
        score++;
    }
    if (report->has_approximate_age && suspect->approximate_age == report->approximate_age) {
        score++;
    }
    return score;
}

// Function to identify the most likely culprit
int identify_culprit(const Suspect suspects[], int num_suspects, const WitnessReport reports[], int num_reports) {
    int best_match_id = -1;
    int max_score = -1;

    for (int i = 0; i < num_suspects; i++) {
        int current_score = 0;
        for (int j = 0; j < num_reports; j++) {
            current_score += calculate_match_score(&suspects[i], &reports[j]);
        }

        if (current_score > max_score) {
            max_score = current_score;
            best_match_id = suspects[i].id;
        } else if (current_score == max_score && max_score != -1) {
            // Handle ties - for simplicity, we'll return -1 if there's a tie
            return -1;
        }
    }

    return best_match_id;
}

int main() {
    // Example Input
    Suspect suspects[] = {
        {1, "blue", "male", "black", 30},
        {2, "white", "male", "brown", 25},
        {3, "red", "female", "blonde", 35}
    };
    int num_suspects = sizeof(suspects) / sizeof(suspects[0]);

    WitnessReport reports[] = {
        {"blue", "male", "", 0, true, true, false, false}, // Witness saw blue shirt and male
        {"", "", "black", 30, false, false, true, true}, // Witness saw black hair and age 30
        {"blue", "", "", 0, true, false, false, false}  // Witness saw blue shirt
    };
    int num_reports = sizeof(reports) / sizeof(reports[0]);

    int culprit_id = identify_culprit(suspects, num_suspects, reports, num_reports);

    if (culprit_id != -1) {
        printf("Most likely culprit ID: %d\n", culprit_id);
    } else {
        printf("Could not identify a single culprit.\n");
    }

    return 0;
}
