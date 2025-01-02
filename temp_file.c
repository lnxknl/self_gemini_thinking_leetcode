#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Episode {
    char* title;
    int season;
    int episode_number;
    int duration_seconds;
    struct Episode* next;
    struct Episode* prev;
} Episode;

// --- Simplified Hash Table (Illustrative - Needs full implementation) ---
#define HASH_TABLE_SIZE 100 // Example size
Episode* title_map[HASH_TABLE_SIZE];

unsigned long hash_string(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_TABLE_SIZE;
}

void insert_episode_by_title(Episode* episode) {
    unsigned long index = hash_string(episode->title);
    // Simple collision handling: overwrite (not ideal for real-world)
    title_map[index] = episode;
}

Episode* find_episode_by_title(char* title) {
    unsigned long index = hash_string(title);
    return title_map[index];
}
// --- End Simplified Hash Table ---

// Array to hold the head of the linked list for each season (Example)
#define MAX_SEASONS 10
Episode* seasons[MAX_SEASONS] = {NULL};

void add_episode_to_season(Episode* episode) {
    if (episode->season > 0 && episode->season <= MAX_SEASONS) {
        if (seasons[episode->season - 1] == NULL) {
            seasons[episode->season - 1] = episode;
            episode->prev = NULL;
            episode->next = NULL;
        } else {
            Episode* current = seasons[episode->season - 1];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = episode;
            episode->prev = current;
            episode->next = NULL;
        }
    }
}

Episode* get_current_episode_by_time(int season, int elapsed_time) {
    if (season > 0 && season <= MAX_SEASONS && seasons[season - 1] != NULL) {
        Episode* current = seasons[season - 1];
        int cumulative_time = 0;
        while (current != NULL) {
            if (elapsed_time >= cumulative_time && elapsed_time < cumulative_time + current->duration_seconds) {
                return current;
            }
            cumulative_time += current->duration_seconds;
            current = current->next;
        }
    }
    return NULL; // No episode found for the given time
}

Episode* get_next_episode(Episode* current_episode) {
    return current_episode ? current_episode->next : NULL;
}

Episode* get_previous_episode(Episode* current_episode) {
    return current_episode ? current_episode->prev : NULL;
}

int main() {
    // Example Usage
    Episode* ep1 = (Episode*)malloc(sizeof(Episode));
    ep1->title = "The One Where They All Meet";
    ep1->season = 1;
    ep1->episode_number = 1;
    ep1->duration_seconds = 1400;
    insert_episode_by_title(ep1);
    add_episode_to_season(ep1);

    Episode* ep2 = (Episode*)malloc(sizeof(Episode));
    ep2->title = "The One with the Sonogram at the End";
    ep2->season = 1;
    ep2->episode_number = 2;
    ep2->duration_seconds = 1350;
    insert_episode_by_title(ep2);
    add_episode_to_season(ep2);

    // Link episodes in the linked list (assuming add_episode handles this)
    ep1->next = ep2;
    ep2->prev = ep1;

    Episode* found_by_title = find_episode_by_title("The One Where They All Meet");
    if (found_by_title) {
        printf("Found by title: %s, Season: %d, Episode: %d\n", found_by_title->title, found_by_title->season, found_by_title->episode_number);
    }

    Episode* current_ep = get_current_episode_by_time(1, 1000);
    if (current_ep) {
        printf("Current episode at time 1000s: %s\n", current_ep->title);
        Episode* next_ep = get_next_episode(current_ep);
        if (next_ep) {
            printf("Next episode: %s\n", next_ep->title);
        }
    }

    return 0;
}
