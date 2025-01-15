#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define event types
typedef enum {
    JOIN,
    LEAVE,
    MESSAGE,
    MENTION
} EventType;

// Structure for an event
typedef struct {
    time_t timestamp;
    EventType event_type;
    int user_id;
    // Add more specific data if needed
} Event;

// Structure for user data
typedef struct UserData {
    int message_count_window;
    int join_leave_count_window;
    int mention_count_window;
    // Consider using a linked list for event history if needed for more detailed analysis
} UserData;

// Simple hash table implementation (for demonstration purposes, a more robust implementation is needed for production)
#define MAX_USERS 10000
UserData user_data[MAX_USERS];
int user_initialized[MAX_USERS] = {0};

// Time window for anomaly detection (in seconds)
#define TIME_WINDOW 60

// Anomaly thresholds (example values, these would need tuning)
#define MESSAGE_FREQUENCY_THRESHOLD 5
#define JOIN_LEAVE_FREQUENCY_THRESHOLD 3
#define MENTION_COUNT_THRESHOLD 10

// Function to process an event
void process_event(Event event) {
    int user_id = event.user_id;

    if (user_id >= 0 && user_id < MAX_USERS) {
        if (!user_initialized[user_id]) {
            memset(&user_data[user_id], 0, sizeof(UserData));
            user_initialized[user_id] = 1;
        }

        switch (event.event_type) {
            case JOIN:
            case LEAVE:
                user_data[user_id].join_leave_count_window++;
                break;
            case MESSAGE:
                user_data[user_id].message_count_window++;
                break;
            case MENTION:
                user_data[user_id].mention_count_window++;
                break;
        }
    }
}

// Function to detect anomalies within the time window
void detect_anomalies() {
    printf("--- Anomaly Detection ---\n");
    for (int i = 0; i < MAX_USERS; i++) {
        if (user_initialized[i]) {
            if (user_data[i].message_count_window > MESSAGE_FREQUENCY_THRESHOLD) {
                printf("User %d: High message frequency (%d)\n", i, user_data[i].message_count_window);
            }
            if (user_data[i].join_leave_count_window > JOIN_LEAVE_FREQUENCY_THRESHOLD) {
                printf("User %d: High join/leave frequency (%d)\n", i, user_data[i].join_leave_count_window);
            }
            if (user_data[i].mention_count_window > MENTION_COUNT_THRESHOLD) {
                printf("User %d: High mention count (%d)\n", i, user_data[i].mention_count_window);
            }
            // Reset window counters after detection (or at intervals)
            user_data[i].message_count_window = 0;
            user_data[i].join_leave_count_window = 0;
            user_data[i].mention_count_window = 0;
        }
    }
}

int main() {
    // Example event stream
    Event events[] = {
        {time(NULL), JOIN, 1},
        {time(NULL) + 5, MESSAGE, 1},
        {time(NULL) + 10, MESSAGE, 1},
        {time(NULL) + 12, MESSAGE, 1},
        {time(NULL) + 15, MESSAGE, 1},
        {time(NULL) + 18, MESSAGE, 1},
        {time(NULL) + 20, JOIN, 2},
        {time(NULL) + 22, LEAVE, 2},
        {time(NULL) + 25, MENTION, 3},
        {time(NULL) + 26, MENTION, 3},
        {time(NULL) + 27, MENTION, 3},
        {time(NULL) + 28, MENTION, 3},
        {time(NULL) + 29, MENTION, 3},
        {time(NULL) + 30, MENTION, 3},
        {time(NULL) + 31, MENTION, 3},
        {time(NULL) + 32, MENTION, 3},
        {time(NULL) + 33, MENTION, 3},
        {time(NULL) + 35, MESSAGE, 4},
    };
    int num_events = sizeof(events) / sizeof(events[0]);

    for (int i = 0; i < num_events; i++) {
        process_event(events[i]);
    }

    detect_anomalies();

    return 0;
}
