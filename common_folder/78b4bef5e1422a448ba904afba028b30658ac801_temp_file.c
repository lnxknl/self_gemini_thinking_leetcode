#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 100  // Adjust as needed
#define WAITING_SIGNAL NULL  // Or ""
#define SILENCE_DATA "SILENCE"

typedef struct AudioPacket {
    int sequenceNumber;
    char* audioData;
    struct AudioPacket* next;
} AudioPacket;

AudioPacket* packetListHead = NULL;
AudioPacket* hashTable[HASH_TABLE_SIZE];
int expectedSequenceNumber = 1;

// Simple hash function
int hash(int sequenceNumber) {
    return sequenceNumber % HASH_TABLE_SIZE;
}

// Function to check if a packet with the given sequence number exists in the hash table
AudioPacket* findPacket(int sequenceNumber) {
    int index = hash(sequenceNumber);
    AudioPacket* current = hashTable[index];
    while (current != NULL) {
        if (current->sequenceNumber == sequenceNumber) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void receivePacket(int sequenceNumber, char* audioData) {
    if (findPacket(sequenceNumber) != NULL) {
        printf("Duplicate packet received: %d\n", sequenceNumber);
        return;
    }

    AudioPacket* newPacket = (AudioPacket*)malloc(sizeof(AudioPacket));
    if (newPacket == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newPacket->sequenceNumber = sequenceNumber;
    newPacket->audioData = strdup(audioData); // Allocate memory for audio data
    newPacket->next = NULL;

    // Insert into linked list (maintaining sorted order)
    if (packetListHead == NULL || sequenceNumber < packetListHead->sequenceNumber) {
        newPacket->next = packetListHead;
        packetListHead = newPacket;
    } else {
        AudioPacket* current = packetListHead;
        while (current->next != NULL && current->next->sequenceNumber < sequenceNumber) {
            current = current->next;
        }
        newPacket->next = current->next;
        current->next = newPacket;
    }

    // Insert into hash table
    int index = hash(sequenceNumber);
    newPacket->next = hashTable[index];
    hashTable[index] = newPacket;

    printf("Received packet: %d\n", sequenceNumber);
}

char* getNextAudioChunk() {
    if (packetListHead != NULL && packetListHead->sequenceNumber == expectedSequenceNumber) {
        AudioPacket* nextChunkPacket = packetListHead;
        char* audioChunk = nextChunkPacket->audioData;

        // Remove from linked list
        packetListHead = packetListHead->next;

        // Remove from hash table (iterating to find and remove)
        int index = hash(nextChunkPacket->sequenceNumber);
        AudioPacket* current = hashTable[index];
        AudioPacket* prev = NULL;
        while (current != NULL) {
            if (current->sequenceNumber == nextChunkPacket->sequenceNumber) {
                if (prev == NULL) {
                    hashTable[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                break;
            }
            prev = current;
            current = current->next;
        }

        free(nextChunkPacket->audioData);
        free(nextChunkPacket); // Free the packet structure
        expectedSequenceNumber++;
        printf("Processed chunk for sequence: %d\n", expectedSequenceNumber - 1);
        return audioChunk;
    } else if (packetListHead != NULL && packetListHead->sequenceNumber > expectedSequenceNumber) {
        printf("Missing packet detected: %d\n", expectedSequenceNumber);
        expectedSequenceNumber++;
        return SILENCE_DATA;
    }
    else {
        printf("Waiting for packet: %d\n", expectedSequenceNumber);
        return WAITING_SIGNAL;
    }
}

// Function to free the allocated memory for the linked list and hash table
void cleanup() {
    AudioPacket* current = packetListHead;
    while (current != NULL) {
        AudioPacket* temp = current;
        current = current->next;
        free(temp->audioData);
        free(temp);
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        current = hashTable[i];
        while (current != NULL) {
            AudioPacket* temp = current;
            current = current->next;
            free(temp->audioData);
            free(temp);
        }
        hashTable[i] = NULL;
    }
}

int main() {
    // Initialize hash table
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    // Test input
    receivePacket(3, "AudioData3");
    receivePacket(1, "AudioData1");
    receivePacket(5, "AudioData5");
    receivePacket(2, "AudioData2");
    receivePacket(3, "DuplicateData"); // Simulate a duplicate

    char* chunk;
    printf("Getting audio chunks:\n");
    // Process all received packets plus handle missing ones
    while (expectedSequenceNumber <= 5) {
        chunk = getNextAudioChunk();
        if (chunk == WAITING_SIGNAL) {
            printf("Waiting for next packet\n");
        } else if (chunk == SILENCE_DATA) {
            printf("Missing packet - inserting silence\n");
        } else {
            printf("Next audio chunk: %s\n", chunk);
        }
    }

    cleanup(); // Free allocated memory

    return 0;
}
