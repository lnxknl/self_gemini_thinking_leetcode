#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define Patient structure
typedef struct {
    int id;
    int arrival_time;
    int urgency;
    int waiting_time;
} Patient;

// Define a Priority Queue (Min-Heap based on urgency, then arrival time)
typedef struct {
    Patient *heap;
    int capacity;
    int size;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->heap = (Patient*)malloc(sizeof(Patient) * capacity);
    return pq;
}

void swap(Patient *a, Patient *b) {
    Patient temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify function (min-heap)
void minHeapify(PriorityQueue* pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && (pq->heap[left].urgency > pq->heap[smallest].urgency ||
                           (pq->heap[left].urgency == pq->heap[smallest].urgency && pq->heap[left].arrival_time < pq->heap[smallest].arrival_time))) {
        smallest = left;
    }

    if (right < pq->size && (pq->heap[right].urgency > pq->heap[smallest].urgency ||
                            (pq->heap[right].urgency == pq->heap[smallest].urgency && pq->heap[right].arrival_time < pq->heap[smallest].arrival_time))) {
        smallest = right;
    }

    if (smallest != i) {
        swap(&pq->heap[i], &pq->heap[smallest]);
        minHeapify(pq, smallest);
    }
}

void enqueue(PriorityQueue* pq, Patient patient) {
    if (pq->size == pq->capacity) {
        printf("Priority Queue Overflow!\n");
        return;
    }
    pq->heap[pq->size] = patient;
    int i = pq->size;
    while (i > 0 && (pq->heap[(i - 1) / 2].urgency < pq->heap[i].urgency ||
                     (pq->heap[(i - 1) / 2].urgency == pq->heap[i].urgency && pq->heap[(i - 1) / 2].arrival_time > pq->heap[i].arrival_time))) {
        swap(&pq->heap[i], &pq->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    pq->size++;
}

Patient dequeue(PriorityQueue* pq) {
    if (pq->size <= 0) {
        Patient empty_patient = {-1, -1, -1, -1};
        return empty_patient; // Indicate empty queue
    }
    if (pq->size == 1) {
        pq->size--;
        return pq->heap[0];
    }
    Patient root = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    minHeapify(pq, 0);
    return root;
}

int main() {
    srand(time(NULL)); // Seed random number generator

    int num_beds = 3;
    int simulation_time = 20;
    int next_patient_id = 0;

    PriorityQueue* waiting_queue = createPriorityQueue(100); // Adjust capacity as needed
    int occupied_beds[num_beds]; // Store patient IDs in occupied beds, -1 if empty
    int bed_finish_times[num_beds]; // Store finish times for patients in beds

    for (int i = 0; i < num_beds; i++) {
        occupied_beds[i] = -1;
        bed_finish_times[i] = 0;
    }

    int total_waiting_time = 0;
    int num_treated_patients = 0;

    // Simulate time steps
    for (int current_time = 0; current_time < simulation_time; current_time++) {
        // Simulate patient arrivals (probabilistic)
        if (rand() % 3 == 0) { // ~1/3 chance of a patient arriving
            Patient new_patient;
            new_patient.id = next_patient_id++;
            new_patient.arrival_time = current_time;
            new_patient.urgency = rand() % 5 + 1; // Urgency level 1 to 5
            new_patient.waiting_time = 0;
            printf("Time %d: Patient %d (Urgency %d) arrived.\n", current_time, new_patient.id, new_patient.urgency);

            int assigned_bed = -1;
            for (int i = 0; i < num_beds; i++) {
                if (occupied_beds[i] == -1) {
                    occupied_beds[i] = new_patient.id;
                    bed_finish_times[i] = current_time + (rand() % 5 + 3); // Treatment duration 3 to 7 time units
                    assigned_bed = i;
                    printf("Time %d: Patient %d assigned to bed %d (finish time %d).\n", current_time, new_patient.id, i, bed_finish_times[i]);
                    break;
                }
            }

            if (assigned_bed == -1) {
                enqueue(waiting_queue, new_patient);
                printf("Time %d: Patient %d added to waiting queue.\n", current_time, new_patient.id);
            }
        }

        // Handle departures and assign new patients
        for (int i = 0; i < num_beds; i++) {
            if (occupied_beds[i] != -1 && current_time >= bed_finish_times[i]) {
                printf("Time %d: Patient %d discharged from bed %d.\n", current_time, occupied_beds[i], i);
                occupied_beds[i] = -1;
                num_treated_patients++;

                if (waiting_queue->size > 0) {
                    Patient next_patient = dequeue(waiting_queue);
                    total_waiting_time += (current_time - next_patient.arrival_time);
                    occupied_beds[i] = next_patient.id;
                    bed_finish_times[i] = current_time + (rand() % 5 + 3);
                    printf("Time %d: Patient %d from queue assigned to bed %d (finish time %d).\n", current_time, next_patient.id, i, bed_finish_times[i]);
                }
            }
        }

        // Update waiting times for patients in the queue
        for (int i = 0; i < waiting_queue->size; i++) {
            waiting_queue->heap[i].waiting_time++;
        }
    }

    if (num_treated_patients > 0) {
        double average_waiting_time = (double)total_waiting_time / num_treated_patients;
        printf("\nSimulation finished.\n");
        printf("Total treated patients: %d\n", num_treated_patients);
        printf("Total waiting time: %d\n", total_waiting_time);
        printf("Average waiting time: %.2f\n", average_waiting_time);
    } else {
        printf("\nSimulation finished. No patients treated.\n");
    }

    free(waiting_queue->heap);
    free(waiting_queue);

    return 0;
}
