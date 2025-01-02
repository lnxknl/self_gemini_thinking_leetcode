#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h" // For hash table implementation

// Structure for video metadata
typedef struct {
    int video_id;
    char title[100];
    int duration;
    UT_hash_handle hh;
    unsigned char *popularity_bitmap;
} VideoMetadata;

// Structure for watched segments (for Red-Black Tree - simplified for this example)
typedef struct WatchedSegment {
    int start_time;
    int end_time;
    // In a real Red-Black Tree implementation, you would have color, parent, left, and right pointers.
} WatchedSegment;

// Structure for user viewing history
typedef struct {
    int user_id;
    // Using a simplified array to represent watched segments, a real implementation would use a Red-Black Tree
    WatchedSegment *watched_segments;
    int num_segments;
    int max_segments;
    UT_hash_handle hh;
} UserViewingHistory;

// Hash table for video metadata
VideoMetadata *videos = NULL;

// Hash table for user viewing history
UserViewingHistory *user_history = NULL;

// Function to add video metadata
void add_video(int video_id, const char *title, int duration) {
    VideoMetadata *video;
    HASH_FIND_INT(videos, &video_id, video);
    if (video == NULL) {
        video = (VideoMetadata *)malloc(sizeof(VideoMetadata));
        video->video_id = video_id;
        strcpy(video->title, title);
        video->duration = duration;
        video->popularity_bitmap = (unsigned char *)calloc((duration + 7) / 8, sizeof(unsigned char)); // Allocate bitmap
        HASH_ADD_INT(videos, video_id, video);
    }
}

// Function to set a bit in the popularity bitmap
void set_popularity_bit(VideoMetadata *video, int time) {
    if (time >= 0 && time < video->duration) {
        int byte_index = time / 8;
        int bit_index = time % 8;
        video->popularity_bitmap[byte_index] |= (1 << bit_index);
    }
}

// Function to add to user viewing history (simplified)
void add_viewing_history(int user_id, int video_id, int start_time, int end_time) {
    UserViewingHistory *user;
    HASH_FIND_INT(user_history, &user_id, user);
    if (user == NULL) {
        user = (UserViewingHistory *)malloc(sizeof(UserViewingHistory));
        user->user_id = user_id;
        user->watched_segments = (WatchedSegment *)malloc(sizeof(WatchedSegment) * 10); // Initial size
        user->num_segments = 0;
        user->max_segments = 10;
        HASH_ADD_INT(user_history, user_id, user);
    }

    if (user->num_segments == user->max_segments) {
        user->max_segments *= 2;
        user->watched_segments = (WatchedSegment *)realloc(user->watched_segments, sizeof(WatchedSegment) * user->max_segments);
    }

    user->watched_segments[user->num_segments].start_time = start_time;
    user->watched_segments[user->num_segments].end_time = end_time;
    user->num_segments++;

    // Update popularity bitmap
    VideoMetadata *video;
    HASH_FIND_INT(videos, &video_id, video);
    if (video != NULL) {
        for (int i = start_time; i < end_time && i < video->duration; ++i) {
            set_popularity_bit(video, i);
        }
    }
}

// Function to check if a user has watched a specific time in a video (simplified)
int has_user_watched(int user_id, int video_id, int time) {
    UserViewingHistory *user;
    HASH_FIND_INT(user_history, &user_id, user);
    if (user != NULL) {
        for (int i = 0; i < user->num_segments; ++i) {
            if (time >= user->watched_segments[i].start_time && time < user->watched_segments[i].end_time) {
                return 1;
            }
        }
    }
    return 0;
}

// Function to recommend segments (simplified)
void recommend_segments(int user_id, int video_id) {
    VideoMetadata *video;
    HASH_FIND_INT(videos, &video_id, video);
    if (video == NULL) {
        printf("Video not found.\n");
        return;
    }

    printf("Recommendations for user %d, video %d (%s):\n", user_id, video_id, video->title);

    for (int i = 0; i < video->duration; ++i) {
        if (!has_user_watched(user_id, video_id, i)) {
            int byte_index = i / 8;
            int bit_index = i % 8;
            if (video->popularity_bitmap[byte_index] & (1 << bit_index)) {
                printf("Consider watching second %d.\n", i);
            }
        }
    }
}

int main() {
    // Example Usage
    add_video(1, "Funny Cats Compilation", 600);
    add_video(2, "Learn C Programming", 1200);

    add_viewing_history(101, 1, 10, 20);
    add_viewing_history(101, 1, 30, 40);
    add_viewing_history(102, 1, 15, 25);
    add_viewing_history(102, 2, 100, 150);

    recommend_segments(101, 1);
    recommend_segments(102, 1);
    recommend_segments(101, 2);

    // Clean up hash tables (important in real applications)
    VideoMetadata *current_video, *tmp_video;
    HASH_ITER(hh, videos, current_video, tmp_video) {
        HASH_DEL(videos, current_video);
        free(current_video->popularity_bitmap);
        free(current_video);
    }

    UserViewingHistory *current_user, *tmp_user;
    HASH_ITER(hh, user_history, current_user, tmp_user) {
        HASH_DEL(user_history, current_user);
        free(current_user->watched_segments);
        free(current_user);
    }

    return 0;
}
