#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent a time interval
typedef struct {
    int start;
    int end;
} Interval;

// Function to compare intervals for sorting
int compareIntervals(const void *a, const void *b) {
    return ((Interval *)a)->start - ((Interval *)b)->start;
}

// Function to find a common free time slot of given duration
Interval findOptimalMeetingTime(Interval **schedules, int numIndividuals, int *scheduleSizes, int duration) {
    if (numIndividuals == 0) {
        return (Interval){0, duration}; // If no individuals, any interval of duration works
    }

    // Sort busy schedules for each individual
    for (int i = 0; i < numIndividuals; i++) {
        qsort(schedules[i], scheduleSizes[i], sizeof(Interval), compareIntervals);
    }

    int currentTime = 0;
    while (true) {
        bool allBusy = false;
        int earliestNextAvailable = -1;

        for (int i = 0; i < numIndividuals; i++) {
            for (int j = 0; j < scheduleSizes[i]; j++) {
                if (currentTime >= schedules[i][j].start && currentTime < schedules[i][j].end) {
                    allBusy = true;
                    if (earliestNextAvailable == -1 || schedules[i][j].end < earliestNextAvailable) {
                        earliestNextAvailable = schedules[i][j].end;
                    }
                    break;
                }
            }
            if (allBusy) break;
        }

        if (!allBusy) {
            // Check if the current time can be the start of a meeting
            int potentialEndTime = currentTime + duration;
            bool conflict = false;
            for (int i = 0; i < numIndividuals; i++) {
                for (int j = 0; j < scheduleSizes[i]; j++) {
                    if (potentialEndTime > schedules[i][j].start && currentTime < schedules[i][j].end) {
                        conflict = true;
                        break;
                    }
                }
                if (conflict) break;
            }

            if (!conflict) {
                return (Interval){currentTime, potentialEndTime};
            }
        }

        // Move currentTime to the next available slot
        if (earliestNextAvailable != -1) {
            currentTime = earliestNextAvailable;
        } else {
            // If no more busy slots, any time from now works
            return (Interval){currentTime, currentTime + duration};
        }

        // Add a safeguard to prevent infinite loops in edge cases
        static int iterations = 0;
        iterations++;
        if (iterations > 10000) {
            break; // Safety break
        }
    }

    return (Interval){-1, -1}; // No suitable meeting time found
}

int main() {
    // Test Example: Three friends trying to find a common free slot for 2 hours.
    int numIndividuals = 3;
    int scheduleSizes[] = {2, 1, 2};

    // Individual 1's busy schedule
    Interval schedule1[] = {{1, 3}, {6, 9}};
    // Individual 2's busy schedule
    Interval schedule2[] = {{2, 5}};
    // Individual 3's busy schedule
    Interval schedule3[] = {{0, 2}, {7, 8}};

    Interval *schedules[] = {schedule1, schedule2, schedule3};
    int duration = 2;

    Interval optimalTime = findOptimalMeetingTime(schedules, numIndividuals, scheduleSizes, duration);

    if (optimalTime.start != -1) {
        printf("Optimal meeting time: [%d, %d]\n", optimalTime.start, optimalTime.end);
    } else {
        printf("No suitable meeting time found.\n");
    }

    // Test Case 2: No overlap
    Interval schedule4[] = {{1, 2}};
    Interval schedule5[] = {{3, 4}};
    Interval *schedules2[] = {schedule4, schedule5};
    int scheduleSizes2[] = {1, 1};
    Interval optimalTime2 = findOptimalMeetingTime(schedules2, 2, scheduleSizes2, 1);
    if (optimalTime2.start != -1) {
        printf("Optimal meeting time 2: [%d, %d]\n", optimalTime2.start, optimalTime2.end);
    } else {
        printf("No suitable meeting time found for test case 2.\n");
    }

    // Test Case 3: Single person, duration 5
    Interval schedule6[] = {{1, 3}};
    Interval *schedules3[] = {schedule6};
    int scheduleSizes3[] = {1};
    Interval optimalTime3 = findOptimalMeetingTime(schedules3, 1, scheduleSizes3, 5);
    if (optimalTime3.start != -1) {
        printf("Optimal meeting time 3: [%d, %d]\n", optimalTime3.start, optimalTime3.end);
    } else {
        printf("No suitable meeting time found for test case 3.\n");
    }

    return 0;
}
