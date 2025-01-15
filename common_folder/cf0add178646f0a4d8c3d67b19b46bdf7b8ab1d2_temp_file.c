#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Call structure
typedef struct {
    int arrival_time;
    int priority;
    int call_id;
} Call;

// Priority Queue (Max Heap) implementation for calls
typedef struct {
    Call* heap;
    int size;
    int capacity;
} CallPriorityQueue;

CallPriorityQueue* createCallPriorityQueue(int capacity) {
    CallPriorityQueue* pq = (CallPriorityQueue*)malloc(sizeof(CallPriorityQueue));
    pq->heap = (Call*)malloc(capacity * sizeof(Call));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swapCalls(Call* a, Call* b) {
    Call temp = *a;
    *a = *b;
    *b = temp;
}

void callHeapifyUp(CallPriorityQueue* pq, int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (pq->heap[index].priority > pq->heap[parentIndex].priority ||
            (pq->heap[index].priority == pq->heap[parentIndex].priority &&
             pq->heap[index].arrival_time < pq->heap[parentIndex].arrival_time)) {
            swapCalls(&pq->heap[index], &pq->heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void callInsert(CallPriorityQueue* pq, Call call) {
    if (pq->size == pq->capacity) {
        // Handle resizing if needed
        return;
    }
    pq->heap[pq->size] = call;
    callHeapifyUp(pq, pq->size);
    pq->size++;
}

void callHeapifyDown(CallPriorityQueue* pq, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    if (left < pq->size && (pq->heap[left].priority > pq->heap[largest].priority ||
                           (pq->heap[left].priority == pq->heap[largest].priority &&
                            pq->heap[left].arrival_time < pq->heap[largest].arrival_time))) {
        largest = left;
    }

    if (right < pq->size && (pq->heap[right].priority > pq->heap[largest].priority ||
                            (pq->heap[right].priority == pq->heap[largest].priority &&
                             pq->heap[right].arrival_time < pq->heap[largest].arrival_time))) {
        largest = right;
    }

    if (largest != index) {
        swapCalls(&pq->heap[index], &pq->heap[largest]);
        callHeapifyDown(pq, largest);
    }
}

Call callExtractMax(CallPriorityQueue* pq) {
    if (pq->size <= 0) {
        Call emptyCall = {-1, -1, -1}; // Indicate error or empty
        return emptyCall;
    }
    if (pq->size == 1) {
        pq->size--;
        return pq->heap[0];
    }
    Call maxCall = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    callHeapifyDown(pq, 0);
    return maxCall;
}

bool isCallPriorityQueueEmpty(CallPriorityQueue* pq) {
    return pq->size == 0;
}

// Min Heap implementation for agent availability times
typedef struct {
    int* heap;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* mh = (MinHeap*)malloc(sizeof(MinHeap));
    mh->heap = (int*)malloc(capacity * sizeof(int));
    mh->size = 0;
    mh->capacity = capacity;
    return mh;
}

void minHeapifyUp(MinHeap* mh, int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (mh->heap[index] < mh->heap[parentIndex]) {
            int temp = mh->heap[index];
            mh->heap[index] = mh->heap[parentIndex];
            mh->heap[parentIndex] = temp;
            index = parentIndex;
        } else {
            break;
        }
    }
}

void minInsert(MinHeap* mh, int key) {
    if (mh->size == mh->capacity) {
        // Handle resizing if needed
        return;
    }
    mh->heap[mh->size] = key;
    minHeapifyUp(mh, mh->size);
    mh->size++;
}

void minHeapifyDown(MinHeap* mh, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < mh->size && mh->heap[left] < mh->heap[smallest]) {
        smallest = left;
    }

    if (right < mh->size && mh->heap[right] < mh->heap[smallest]) {
        smallest = right;
    }

    if (smallest != index) {
        int temp = mh->heap[index];
        mh->heap[index] = mh->heap[smallest];
        mh->heap[smallest] = temp;
        minHeapifyDown(mh, smallest);
    }
}

int extractMin(MinHeap* mh) {
    if (mh->size <= 0) {
        return -1; // Indicate error or empty
    }
    if (mh->size == 1) {
        mh->size--;
        return mh->heap[0];
    }
    int min = mh->heap[0];
    mh->heap[0] = mh->heap[mh->size - 1];
    mh->size--;
    minHeapifyDown(mh, 0);
    return min;
}

int compareCalls(const void* a, const void* b) {
    Call* callA = (Call*)a;
    Call* callB = (Call*)b;
    if (callA->arrival_time != callB->arrival_time) {
        return callA->arrival_time - callB->arrival_time;
    }
    return callB->priority - callA->priority; // Higher priority first
}

int processHotlineCalls(Call* calls, int num_calls, int num_agents) {
    qsort(calls, num_calls, sizeof(Call), compareCalls);

    CallPriorityQueue* waiting_calls = createCallPriorityQueue(num_calls);
    MinHeap* agent_availability = createMinHeap(num_agents);
    for (int i = 0; i < num_agents; i++) {
        minInsert(agent_availability, 0); // Initialize all agents as available at time 0
    }

    int total_waiting_time = 0;
    int call_index = 0;
    int current_time = 0;

    while (call_index < num_calls || !isCallPriorityQueueEmpty(waiting_calls)) {
        // Advance time to the next relevant event (either a call arrival or an agent becoming free)
        int next_event_time = -1;
        if (call_index < num_calls) {
            next_event_time = calls[call_index].arrival_time;
        }
        if (!isCallPriorityQueueEmpty(waiting_calls) && agent_availability->size > 0) {
            int next_agent_free_time = agent_availability->heap[0];
            if (next_event_time == -1 || next_agent_free_time < next_event_time) {
                next_event_time = next_agent_free_time;
            }
        }

        if (next_event_time != -1 && next_event_time > current_time) {
            current_time = next_event_time;
        }

        // Add arrived calls to the priority queue
        while (call_index < num_calls && calls[call_index].arrival_time <= current_time) {
            callInsert(waiting_calls, calls[call_index]);
            call_index++;
        }

        // Assign calls to available agents
        while (!isCallPriorityQueueEmpty(waiting_calls) && agent_availability->size > 0) {
            Call next_call = callExtractMax(waiting_calls);
            int agent_available_time = extractMin(agent_availability);
            int start_processing_time = (current_time > agent_available_time) ? current_time : agent_available_time;
            int waiting_time = start_processing_time - next_call.arrival_time;
            total_waiting_time += waiting_time;

            // Assume processing time of 1 for simplicity
            int finish_time = start_processing_time + 1;
            minInsert(agent_availability, finish_time);
        }
    }

    free(waiting_calls->heap);
    free(waiting_calls);
    free(agent_availability->heap);
    free(agent_availability);

    return total_waiting_time;
}

int main() {
    // Test Example Input
    Call calls[] = {
        {0, 2, 1},
        {0, 1, 2},
        {1, 3, 3},
        {3, 1, 4}
    };
    int num_calls = sizeof(calls) / sizeof(calls[0]);
    int num_agents = 1;

    int total_wait = processHotlineCalls(calls, num_calls, num_agents);
    printf("Total waiting time: %d\n", total_wait); // Expected output: 2

    Call calls2[] = {
        {0, 1, 1},
        {0, 1, 2},
        {0, 1, 3}
    };
    int num_calls2 = sizeof(calls2) / sizeof(calls2[0]);
    int num_agents2 = 2;
    int total_wait2 = processHotlineCalls(calls2, num_calls2, num_agents2);
    printf("Total waiting time: %d\n", total_wait2); // Expected output: 0

    return 0;
}
