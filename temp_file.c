#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define data structures
typedef enum { RED, YELLOW, GREEN } LightState;

typedef struct {
    int id;
    LightState state;
    int* incoming_roads; // Array of road IDs controlled by this light
    int num_roads;
    int green_duration;
    int yellow_duration;
    int current_timer;
} TrafficLight;

typedef struct {
    int arrival_time;
    int road_id;
} Vehicle;

typedef struct Node {
    Vehicle data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} Queue;

// Function to initialize a queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    q->head = q->tail = NULL;
    return q;
}

// Function to enqueue a vehicle
void enqueue(Queue* q, Vehicle v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = v;
    newNode->next = NULL;
    if (q->tail == NULL) {
        q->head = q->tail = newNode;
        return;
    }
    q->tail->next = newNode;
    q->tail = newNode;
}

// Function to dequeue a vehicle
Vehicle dequeue(Queue* q) {
    if (q->head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = q->head;
    Vehicle v = temp->data;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    free(temp);
    return v;
}

// Function to get the size of a queue
int queueSize(Queue* q) {
    int count = 0;
    Node* current = q->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// --- Simulation Logic ---

// Simulate vehicle arrival (simplified)
void simulateArrival(Queue** road_queues, int num_roads, int current_time) {
    if (rand() % 5 == 0) { // Simulate arrival with some probability
        int road_id = rand() % num_roads;
        Vehicle new_vehicle = {current_time, road_id};
        enqueue(road_queues[road_id], new_vehicle);
        printf("Time %d: Vehicle arrived at road %d\n", current_time, road_id);
    }
}

// Update traffic light state
void updateTrafficLight(TrafficLight* light) {
    light->current_timer++;
    if (light->state == GREEN && light->current_timer >= light->green_duration) {
        light->state = YELLOW;
        light->current_timer = 0;
    } else if (light->state == YELLOW && light->current_timer >= light->yellow_duration) {
        light->state = RED;
        light->current_timer = 0;
    } else if (light->state == RED && light->current_timer >= (rand() % 5 + 5)) { // Simple logic for next green
        light->state = GREEN;
        light->current_timer = 0;
    }
}

// Process vehicles at a green light
void processVehicles(Queue* road_queue, int current_time) {
    while (road_queue->head != NULL) {
        Vehicle v = dequeue(road_queue);
        int waiting_time = current_time - v.arrival_time;
        printf("Time %d: Vehicle from road processed, waited %d\n", current_time, waiting_time);
        // Here, you would accumulate waiting times for average calculation
    }
}

int main() {
    srand(time(NULL)); // Seed random number generator

    // --- Define Intersection Setup ---
    int num_roads = 3;
    Queue** road_queues = (Queue**)malloc(num_roads * sizeof(Queue*));
    for (int i = 0; i < num_roads; i++) {
        road_queues[i] = createQueue();
    }

    TrafficLight light1 = {
        .id = 1,
        .state = RED,
        .incoming_roads = (int[]){0},
        .num_roads = 1,
        .green_duration = 10,
        .yellow_duration = 3,
        .current_timer = 0
    };

    TrafficLight light2 = {
        .id = 2,
        .state = RED,
        .incoming_roads = (int[]){1, 2},
        .num_roads = 2,
        .green_duration = 12,
        .yellow_duration = 4,
        .current_timer = 0
    };

    TrafficLight* lights[] = {&light1, &light2};
    int num_lights = sizeof(lights) / sizeof(lights[0]);

    // --- Simulation Loop ---
    int simulation_duration = 30;
    for (int current_time = 0; current_time < simulation_duration; current_time++) {
        printf("--- Time: %d ---\n", current_time);

        // Simulate vehicle arrivals
        simulateArrival(road_queues, num_roads, current_time);

        // Update traffic light states and process vehicles
        for (int i = 0; i < num_lights; i++) {
            updateTrafficLight(lights[i]);
            if (lights[i]->state == GREEN) {
                for (int j = 0; j < lights[i]->num_roads; j++) {
                    processVehicles(road_queues[lights[i]->incoming_roads[j]], current_time);
                }
            }
            printf("Light %d State: %d, Timer: %d\n", lights[i]->id, lights[i]->state, lights[i]->current_timer);
        }

        // Add logic here for dynamic traffic light adjustment based on queue lengths
        // (This is the complex part to implement)

        // Sleep for a short duration to simulate time passing
        // sleep(1); // Uncomment to slow down simulation
    }

    // --- Clean up memory ---
    for (int i = 0; i < num_roads; i++) {
        // Free nodes in the queues (not shown for brevity)
        free(road_queues[i]);
    }
    free(road_queues);

    return 0;
}
