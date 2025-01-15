#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int start_time;
    int end_time;
    int disturbance_level;
} Activity;

typedef struct {
    int time;
    int type; // 0: sleep_start, 1: sleep_end, 2: start_activity, 3: end_activity
    int disturbance;
    int activity_index; // For identifying the activity ending
} Event;

int compareEvents(const void *a, const void *b) {
    Event *eventA = (Event *)a;
    Event *eventB = (Event *)b;
    if (eventA->time != eventB->time) {
        return eventA->time - eventB->time;
    }
    return eventA->type - eventB->type; // Prioritize sleep start, then activity start, then activity end, then sleep end
}

int calculateLostSleep(Activity *activities, int num_activities, int sleep_start, int sleep_end, int disturbance_threshold) {
    int sleep_capacity = sleep_end - sleep_start;
    if (sleep_capacity <= 0) return 0;

    int num_events = num_activities * 2 + 2;
    Event events[num_events];
    int event_index = 0;

    events[event_index++] = (Event){sleep_start, 0, 0, -1};
    events[event_index++] = (Event){sleep_end, 1, 0, -1};

    for (int i = 0; i < num_activities; i++) {
        events[event_index++] = (Event){activities[i].start_time, 2, activities[i].disturbance_level, i};
        events[event_index++] = (Event){activities[i].end_time, 3, activities[i].disturbance_level, i};
    }

    qsort(events, num_events, sizeof(Event), compareEvents);

    int accumulated_disturbance = 0;
    int good_sleep_obtained = 0;
    bool is_sleeping = false;
    bool disrupted = false;
    int last_event_time = 0;

    for (int i = 0; i < num_events; i++) {
        int current_time = events[i].time;
        int time_elapsed = current_time - last_event_time;

        if (is_sleeping && !disrupted && time_elapsed > 0) {
            int disturbance_this_interval = accumulated_disturbance * time_elapsed;
            if (accumulated_disturbance > 0) {
                 if (accumulated_disturbance > 0 && accumulated_disturbance > disturbance_threshold) {
                    disrupted = true;
                } else {
                    good_sleep_obtained += time_elapsed;
                }
            } else {
                good_sleep_obtained += time_elapsed;
            }
        }

        switch (events[i].type) {
            case 0: // sleep_start
                is_sleeping = true;
                break;
            case 1: // sleep_end
                is_sleeping = false;
                break;
            case 2: // start_activity
                if (current_time >= sleep_start && current_time < sleep_end) {
                    accumulated_disturbance += events[i].disturbance;
                }
                break;
            case 3: // end_activity
                if (current_time > sleep_start && current_time <= sleep_end) {
                    accumulated_disturbance -= events[i].disturbance;
                    if (accumulated_disturbance < 0) accumulated_disturbance = 0; // Avoid negative disturbance
                }
                break;
        }
        last_event_time = current_time;
    }

    if (disrupted) {
        return sleep_capacity;
    } else {
        return sleep_capacity - good_sleep_obtained;
    }
}

int main() {
    Activity activities[] = {
        {10, 20, 2},
        {25, 30, 5},
        {40, 50, 3}
    };
    int num_activities = sizeof(activities) / sizeof(activities[0]);
    int sleep_start = 15;
    int sleep_end = 45;
    int disturbance_threshold = 8;

    int lost_sleep = calculateLostSleep(activities, num_activities, sleep_start, sleep_end, disturbance_threshold);
    printf("Lost Sleep: %d\n", lost_sleep); // Expected output will vary based on precise interpretation of disturbance

    // Test case 2: No overlap
    Activity activities2[] = {{5, 8, 5}};
    int num_activities2 = sizeof(activities2) / sizeof(activities2[0]);
    int lost_sleep2 = calculateLostSleep(activities2, num_activities2, sleep_start, sleep_end, disturbance_threshold);
    printf("Lost Sleep 2: %d\n", lost_sleep2); // Expected: 0

    // Test case 3: High disturbance, threshold crossed
    Activity activities3[] = {{20, 30, 10}};
    int num_activities3 = sizeof(activities3) / sizeof(activities3[0]);
    int lost_sleep3 = calculateLostSleep(activities3, num_activities3, sleep_start, sleep_end, disturbance_threshold);
    printf("Lost Sleep 3: %d\n", lost_sleep3); // Expected: 30

    return 0;
}
