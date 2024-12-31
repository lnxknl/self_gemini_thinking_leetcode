#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_RESIDENTS 64 // Assuming a maximum of 64 residents
#define NUM_APARTMENTS 10000 // Assuming a maximum of 10000 apartments

// Structure to represent the access control system
typedef struct {
    unsigned long long access_map[NUM_APARTMENTS]; // Using an array as a simple hash table (direct addressing)
} AccessControlSystem;

// Function to initialize the access control system
AccessControlSystem* initAccessControlSystem() {
    AccessControlSystem* system = (AccessControlSystem*)malloc(sizeof(AccessControlSystem));
    if (system != NULL) {
        for (int i = 0; i < NUM_APARTMENTS; i++) {
            system->access_map[i] = 0;
        }
    }
    return system;
}

// Function to grant access to an apartment for a resident
bool grantAccess(AccessControlSystem* system, int apartment_number, int resident_id) {
    if (system == NULL || apartment_number < 0 || apartment_number >= NUM_APARTMENTS || resident_id < 0 || resident_id >= MAX_RESIDENTS) {
        return false; // Invalid input
    }
    system->access_map[apartment_number] |= (1ULL << resident_id);
    return true;
}

// Function to check if a resident has access to an apartment
bool hasAccess(AccessControlSystem* system, int apartment_number, int resident_id) {
    if (system == NULL || apartment_number < 0 || apartment_number >= NUM_APARTMENTS || resident_id < 0 || resident_id >= MAX_RESIDENTS) {
        return false; // Invalid input
    }
    return (system->access_map[apartment_number] & (1ULL << resident_id)) != 0;
}

int main() {
    AccessControlSystem* system = initAccessControlSystem();
    if (system == NULL) {
        fprintf(stderr, "Failed to initialize access control system.\n");
        return 1;
    }

    // Test case: Grant access to resident 1 for apartment 3602
    int apartment_3602 = 3602;
    int resident_1 = 1;
    if (grantAccess(system, apartment_3602, resident_1)) {
        printf("Granted access to resident %d for apartment %d.\n", resident_1, apartment_3602);
    } else {
        printf("Failed to grant access.\n");
    }

    // Test case: Check if resident 1 has access to apartment 3602
    if (hasAccess(system, apartment_3602, resident_1)) {
        printf("Resident %d has access to apartment %d.\n", resident_1, apartment_3602);
    } else {
        printf("Resident %d does not have access to apartment %d.\n", resident_1, apartment_3602);
    }

    // Test case: Check if resident 2 has access to apartment 3602 (should not)
    int resident_2 = 2;
    if (hasAccess(system, apartment_3602, resident_2)) {
        printf("Resident %d has access to apartment %d.\n", resident_2, apartment_3602);
    } else {
        printf("Resident %d does not have access to apartment %d.\n", resident_2, apartment_3602);
    }

    free(system);
    return 0;
}
