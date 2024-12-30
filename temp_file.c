#include <stdio.h>
#include <stdlib.h>

// Structure for a data packet
typedef struct Packet {
    int id;
    int arrival_time;
    int data;
    struct Packet* next;
} Packet;

// Function to create a new packet
Packet* createPacket(int id, int arrival_time, int data) {
    Packet* new_packet = (Packet*)malloc(sizeof(Packet));
    if (new_packet == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_packet->id = id;
    new_packet->arrival_time = arrival_time;
    new_packet->data = data;
    new_packet->next = NULL;
    return new_packet;
}

// Function to insert a packet into the buffer (sorted by id)
void insertPacket(Packet** head, Packet* new_packet) {
    if (*head == NULL || new_packet->id < (*head)->id) {
        new_packet->next = *head;
        *head = new_packet;
        return;
    }

    Packet* current = *head;
    while (current->next != NULL && current->next->id < new_packet->id) {
        current = current->next;
    }
    new_packet->next = current->next;
    current->next = new_packet;
}

// Function to print the buffer
void printBuffer(Packet* head) {
    printf("Buffer: ");
    Packet* current = head;
    while (current != NULL) {
        printf("(ID: %d, Time: %d, Data: %d) -> ", current->id, current->arrival_time, current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    Packet* buffer_head = NULL;
    int next_expected_packet_id = 0;
    int playback_threshold = 3;
    int current_time = 0;

    // Simulate receiving packets
    // Example input: (id, arrival_time_offset, data)
    int incoming_packets[][3] = {
        {0, 0, 10},
        {2, 1, 20},
        {1, 2, 15},
        {4, 3, 30},
        {3, 4, 25},
        {6, 6, 40},
        {7, 7, 45},
        {5, 8, 35}
    };
    int num_packets = sizeof(incoming_packets) / sizeof(incoming_packets[0]);

    for (int i = 0; i < num_packets; i++) {
        int packet_id = incoming_packets[i][0];
        int arrival_offset = incoming_packets[i][1];
        int packet_data = incoming_packets[i][2];
        current_time += arrival_offset;

        Packet* received_packet = createPacket(packet_id, current_time, packet_data);
        printf("Received Packet (ID: %d, Time: %d, Data: %d)\n", received_packet->id, received_packet->arrival_time, received_packet->data);

        insertPacket(&buffer_head, received_packet);
        printBuffer(buffer_head);

        // Update next_expected_packet_id by finding the longest sequence of consecutive packets from 0
        Packet* current = buffer_head;
        next_expected_packet_id = 0;  // Reset to start checking from 0
        while (current != NULL && current->id == next_expected_packet_id) {
            next_expected_packet_id++;
            current = current->next;
        }

        printf("Next Expected Packet ID: %d\n", next_expected_packet_id);

        // Check playback condition
        Packet* temp = buffer_head;
        int buffer_front_consecutive = 0;
        int expected_id = 0;
        while(temp != NULL && temp->id == expected_id){
            buffer_front_consecutive++;
            expected_id++;
            temp = temp->next;
        }

        if (buffer_front_consecutive >= playback_threshold) {
            printf("Playback can start! (Buffer has %d consecutive packets)\n", buffer_front_consecutive);
        } else {
            printf("Buffering... (Current consecutive packets at front: %d, Threshold: %d)\n", buffer_front_consecutive, playback_threshold);
        }
        printf("\n");
    }

    // Clean up memory (important in C)
    Packet* current = buffer_head;
    while (current != NULL) {
        Packet* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
